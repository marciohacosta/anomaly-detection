#pragma once
#include <fstream>
#include "../Base/IAnalysisService.h"
#include "../Providers/SHESDAnalysisService.cpp"

using namespace std;

namespace MSWay::IAM::Service
{
    class AnalysisServiceFactory
    {
    public:
        AnalysisServiceFactory(){}

        shared_ptr<IAnalysisService> build(const string provider, shared_ptr<MSWay::IAM::Repository::ISummaryRepository> ptrSummaryRepository, shared_ptr<MSWay::IAM::Infrastructure::IMessagingService> ptrMessagingService)
        {
            cout << "Analysis service provider: " << provider << endl;

            if (provider == "S-H-ESD")
            {
                int minDays = stoi(getenv("MSWAY_IAM_ANALYSER_DETECTION_MINDAYS"));
                int maxDays = stoi(getenv("MSWAY_IAM_ANALYSER_DETECTION_MAXDAYS"));
                int period = stoi(getenv("MSWAY_IAM_ANALYSER_DETECTION_PERIOD"));
                float alfa = stof(getenv("MSWAY_IAM_ANALYSER_DETECTION_ALFA"));
                float maxAnomalies = stof(getenv("MSWAY_IAM_ANALYSER_DETECTION_MAXANOMALIES"));
                string queue = getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_QUEUE");
                bool messagingEnabled = MSWay::IAM::Helpers::StringHelper::toTrimmedLowerCase(getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_ENABLED")) == "true";
                int publishRetryLimit = stoi(getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_RETRYLIMIT"));

                cout << "Min days         : " << minDays << endl;
                cout << "Max days         : " << maxDays << endl;
                cout << "Period           : " << period << endl;
                cout << "Alfa             : " << alfa << endl;
                cout << "Max anomalies    : " << maxAnomalies << endl;
                cout << "Queue            : " << queue << endl;
                cout << "Messaging enabled: " << messagingEnabled << endl;
                cout << "Retry limit      : " << publishRetryLimit << endl;

                return make_shared<SHESDAnalysisService>(ptrSummaryRepository, ptrMessagingService, minDays, maxDays, period, alfa, maxAnomalies, queue, messagingEnabled, publishRetryLimit);
            }

            throw "Invalid analysis service provider!";
        }
    };
}