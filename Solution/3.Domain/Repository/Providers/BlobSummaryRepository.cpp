#pragma once
#include <cstring>
#include <iostream>
#include <json.hpp>
#include <cpr/cpr.h>
#include "../../Model/RequestInfo.cpp"
#include "../Base/ISummaryRepository.h"

using namespace std;

namespace MSWay::IAM::Repository
{
    class BlobSummaryRepository : public ISummaryRepository
    {
    private:

        const string FILE_EXTENSION = ".json";

        string blobRestApi;
        string blobSasToken;
        string storageAccount;
        string containerName;

        string getObjectKey(MSWay::IAM::Model::RequestInfo& requestInfo)
        {
            return requestInfo.getToken() + FILE_EXTENSION;
        }

    public:
        BlobSummaryRepository(const string blobRestApi, const string blobSasToken, const string storageAccount, const string containerName)
        {
            this->blobRestApi = blobRestApi;
            this->blobSasToken = blobSasToken;
            this->storageAccount = storageAccount;
            this->containerName = containerName;
        }

        void loadSummary(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            string objectKey = getObjectKey(requestInfo);

            string url = "https://" + storageAccount + "." + blobRestApi + "/" + containerName + "/" + objectKey + "?" + blobSasToken;

            try
            {
                cpr::Response response = cpr::Get(cpr::Url{url});

                if (response.status_code != 200) return;

                nlohmann::json j = nlohmann::json::parse(response.text);

                MSWay::IAM::Model::Summary summary;
                MSWay::IAM::Model::Summary::from_json(j, summary);

                requestInfo.setSummary(summary);
            }
            catch(const exception& e)
            {
                cerr << e.what() << endl;
            }
        }

        void saveSummary(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            MSWay::IAM::Model::Summary* summary = requestInfo.getSummary();

            MSWay::IAM::Model::Summary sum = *summary;

            nlohmann::json j;
            sum.to_json(j);

            string objectKey = getObjectKey(requestInfo);

            string js = j.dump();

            try
            {
                string url = "https://" + storageAccount + "." + blobRestApi + "/" + containerName + "/" + objectKey + "?" + blobSasToken;

                cpr::Response response = cpr::Put(cpr::Url{url}, cpr::Body{js}, cpr::Header{{"x-ms-blob-type", "BlockBlob"}}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Header{{"Content-Length", to_string(js.length())}});

                if (response.status_code != 201)
                {
                    cerr << response.reason << endl;
                }
            }
            catch(const exception& e)
            {
                cerr << e.what() << endl;
            }
        }
    };
}