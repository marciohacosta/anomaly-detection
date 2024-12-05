#pragma once
#include <iostream>
#include "../Base/ISummaryRepository.h"
#include "../Providers/FSSummaryRepository.cpp"
#include "../Providers/S3SummaryRepository.cpp"
#include "../Providers/BlobSummaryRepository.cpp"

using namespace std;

namespace MSWay::IAM::Repository
{
    class SummaryRepositoryFactory
    {
    public:
        SummaryRepositoryFactory(){}

        shared_ptr<ISummaryRepository> build(const string provider)
        {
            cout << "Summary repository provider: " << provider << endl;

            if (provider == "FileSystem")
            {
                string basePath = getenv("MSWAY_IAM_ANALYSER_SUMMARYREPOSITORY_BASEPATH");
                cout << "Summary repository base path: " << basePath << endl;

                return make_shared<FSSummaryRepository>(basePath);
            }

            if (provider == "AWSS3")
            {
                string accessKey = getenv("MSWAY_IAM_ANALYSER_AWSACCESSKEY");
                string secretKey = getenv("MSWAY_IAM_ANALYSER_AWSSECRETKEY");
                string region = getenv("MSWAY_IAM_ANALYSER_AWSREGION");
                string bucket = getenv("MSWAY_IAM_ANALYSER_SUMMARYREPOSITORY_AWSBUCKET");

                cout << "Summary repository region: " << region << endl;
                cout << "Summary repository bucket: " << bucket << endl;

                return make_shared<S3SummaryRepository>(region, accessKey, secretKey, bucket);
            }

            if (provider == "AzureBlob")
            {
                string blobRestApi = getenv("MSWAY_IAM_ANALYSER_AZURE_BLOBRESTAPI");
                string blobSasToken = getenv("MSWAY_IAM_ANALYSER_AZURE_BLOBSASTOKEN");
                string storageAccount = getenv("MSWAY_IAM_ANALYSER_AZURE_STORAGEACCOUNT");
                string containerName = getenv("MSWAY_IAM_ANALYSER_SUMMARYREPOSITORY_AZUREBLOB_CONTAINERNAME");

                cout << "Azure BLOB REST API              : " << blobRestApi << endl;
                cout << "Azure storage account            : " << storageAccount << endl;
                cout << "Summary repository container name: " << containerName << endl;

                return make_shared<BlobSummaryRepository>(blobRestApi, blobSasToken, storageAccount, containerName);
            }

            throw "Invalid summary repository provider!";
        }
   };
}