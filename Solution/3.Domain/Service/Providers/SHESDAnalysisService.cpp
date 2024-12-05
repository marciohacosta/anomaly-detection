#pragma once
#include <chrono>
#include "../../Model/AnalysisInfo.cpp"
#include "../../Repository/Base/ISummaryRepository.h"
#include "../../4.Infrastructure/Messaging/Base/IMessagingService.h"
#include "../../4.Infrastructure/Helpers/DateHelper.cpp"
#include "../../5.External/Include/shesd.hpp"
#include "../Base/IAnalysisService.h"

using namespace std;
using namespace chrono;
using chrono::system_clock;

namespace MSWay::IAM::Service
{
    class SHESDAnalysisService : public IAnalysisService
    {
    private:
        int minDays;
        int maxDays;
        int period;
        float alfa;
        float maxAnomalies;
        string queue;
        bool messagingEnabled;
        int publishRetryLimit;
        shared_ptr<MSWay::IAM::Repository::ISummaryRepository> ptrSummaryRepository;
        shared_ptr<MSWay::IAM::Infrastructure::IMessagingService> ptrMessagingService;

        void sendAnalysisInfo(MSWay::IAM::Model::AnalysisInfo &analysisInfo)
        {
            if (!messagingEnabled) return;
            
            nlohmann::json j;
            analysisInfo.to_json(j);

            string key = analysisInfo.getKey();
            string value = j.dump();

            int tryCount = 0;

            while (tryCount < publishRetryLimit)
            {
                bool publishSucceeded = ptrMessagingService->publish(queue, key, value);

                if (publishSucceeded) return;

                tryCount++;
            }

            cout << "Publish has failed for key " << key << endl;
        }

    public:
        SHESDAnalysisService(shared_ptr<MSWay::IAM::Repository::ISummaryRepository> ptrSummaryRepository, shared_ptr<MSWay::IAM::Infrastructure::IMessagingService> ptrMessagingService, int minDays, int maxDays, int period, float alfa, float maxAnomalies, string queue, bool messagingEnabled, int publishRetryLimit)
        {
            this->ptrSummaryRepository = ptrSummaryRepository;
            this->ptrMessagingService = ptrMessagingService;
            this->minDays = minDays;
            this->maxDays = maxDays;
            this->period = period;
            this->alfa = alfa;
            this->maxAnomalies = maxAnomalies;
            this->queue = queue;
            this->messagingEnabled = messagingEnabled;
            this->publishRetryLimit = publishRetryLimit;
        }

        int load(MSWay::IAM::Model::RequestInfo& requestInfo, string date, string hour) override
        {
            ptrSummaryRepository->loadSummary(requestInfo);

            MSWay::IAM::Model::Summary* summary = requestInfo.getSummary();

            summary->add(date, hour, maxDays);

            ptrSummaryRepository->saveSummary(requestInfo);

            MSWay::IAM::Model::AnalysisInfo analysisInfo(requestInfo, date, hour);

            sendAnalysisInfo(analysisInfo);
            
            return 202;
        }

        int analyse(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            ptrSummaryRepository->loadSummary(requestInfo);

            MSWay::IAM::Model::Summary* summary = requestInfo.getSummary();

            // Get current date and time
            system_clock::time_point moment = system_clock::now();

            time_t tt;
            tm utctm;

            tt = system_clock::to_time_t(moment);
            utctm = *gmtime(&tt);

            string date = MSWay::IAM::Helpers::DateHelper::toString(utctm);
            string hour = MSWay::IAM::Helpers::DateHelper::toHourString(utctm);

            // Add to summary
            summary->add(date, hour, maxDays);

            // Save summary
            ptrSummaryRepository->saveSummary(requestInfo);

            // Set analysis info
            MSWay::IAM::Model::AnalysisInfo analysisInfo(requestInfo, date, hour);

            if (summary->getDaysCount() < minDays)
            {
                sendAnalysisInfo(analysisInfo);
                return 202; // Accepted
            }

            // Do analysis
            shesd::SHESDResult adResult = shesd::params().alpha(alfa).direction(shesd::Direction::Positive).max_anoms(maxAnomalies).fit(summary->getSeries(), period);

            // Check detection result
            int result = 200;

            if (adResult.anomalies.empty())
            {
                sendAnalysisInfo(analysisInfo);
                return result;
            }

            vector<string> dates = summary->getDates();
            int lastIndex = adResult.anomalies.back();
            int datesSize = dates.size() - 1;

            if (lastIndex <= datesSize && dates[lastIndex] == date)
            {
                result = 417;
                analysisInfo.setIsAnomaly(true);
            }

            sendAnalysisInfo(analysisInfo);

            return result;
        }
    };
}
