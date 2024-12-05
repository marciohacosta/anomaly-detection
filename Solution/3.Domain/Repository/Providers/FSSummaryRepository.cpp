#pragma once
#include <filesystem>
#include <fstream>
#include <json.hpp>
#include "../../Model/RequestInfo.cpp"
#include "../Base/ISummaryRepository.h"

using namespace std;

namespace MSWay::IAM::Repository
{
    class FSSummaryRepository : public ISummaryRepository
    {
    private:

        const string FILE_EXTENSION = ".json";

        string basePath;

        string getFilename(MSWay::IAM::Model::RequestInfo& requestInfo)
        {
            return basePath + "/" + requestInfo.getToken() + FILE_EXTENSION;
        }

    public:
        FSSummaryRepository(const string basePath)
        {
            this->basePath = basePath;

            filesystem::create_directory(basePath);
        }

        void loadSummary(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            string filename = getFilename(requestInfo);

            if (!filesystem::exists(filename))
            {
                return;
            }

            string strJson;
            nlohmann::json j;

            fstream fsReader;

            fsReader.open(filename, fstream::in);

            while (getline(fsReader, strJson))
            {
                j = nlohmann::json::parse(strJson);
            }

            fsReader.close();

            MSWay::IAM::Model::Summary summary;
            MSWay::IAM::Model::Summary::from_json(j, summary);

            requestInfo.setSummary(summary);
        }

        void saveSummary(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            string filename = getFilename(requestInfo);

            nlohmann::json j;
            
            MSWay::IAM::Model::Summary* summary = requestInfo.getSummary();

            MSWay::IAM::Model::Summary sum = *summary;

            sum.to_json(j);

            fstream fsWriter;

            fsWriter.open(filename, fstream::out | fstream::trunc);

            fsWriter << j << endl;

            fsWriter.flush();
            fsWriter.close();
        }
    };
}