#pragma once
#include <iostream>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <json.hpp>
#include "../../Model/RequestInfo.cpp"
#include "../Base/ISummaryRepository.h"

using namespace std;

namespace MSWay::IAM::Repository
{
    class S3SummaryRepository : public ISummaryRepository
    {
    private:

        const string FILE_EXTENSION = ".json";

        string accessKey;
        string secretKey;
        string region;
        string bucket;

        string getObjectKey(MSWay::IAM::Model::RequestInfo& requestInfo)
        {
            return requestInfo.getToken() + FILE_EXTENSION;
        }

    public:
        S3SummaryRepository(const string region, const string accessKey, const string secretKey, const string bucket)
        {
            this->accessKey = accessKey;
            this->secretKey = secretKey;
            this->region = region;
            this->bucket = bucket;
        }

        void loadSummary(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            Aws::Auth::AWSCredentials credentials;
            credentials.SetAWSAccessKeyId(Aws::String(accessKey));
            credentials.SetAWSSecretKey(Aws::String(secretKey));

            Aws::S3::S3ClientConfiguration clientConfig;
            clientConfig.region = region;
            Aws::S3::S3Client client(credentials, nullptr, clientConfig);
            string objectKey = getObjectKey(requestInfo);

            Aws::S3::Model::GetObjectRequest request;
            request.SetBucket(Aws::String(bucket));
            request.SetKey(Aws::String(objectKey));

            Aws::S3::Model::GetObjectOutcome outcome = client.GetObject(request);

            if (!outcome.IsSuccess())
            {
                //cout << outcome.GetError() << endl;
                return;
            }

            Aws::IOStream &body = outcome.GetResultWithOwnership().GetBody();

            stringstream ss;
            ss << body.rdbuf();

            nlohmann::json j = nlohmann::json::parse(ss.str());

            MSWay::IAM::Model::Summary summary;
            MSWay::IAM::Model::Summary::from_json(j, summary);

            requestInfo.setSummary(summary);
        }

        void saveSummary(MSWay::IAM::Model::RequestInfo& requestInfo) override
        {
            MSWay::IAM::Model::Summary* summary = requestInfo.getSummary();

            MSWay::IAM::Model::Summary sum = *summary;

            nlohmann::json j;
            sum.to_json(j);

            Aws::Auth::AWSCredentials credentials;
            credentials.SetAWSAccessKeyId(Aws::String(accessKey));
            credentials.SetAWSSecretKey(Aws::String(secretKey));

            Aws::S3::S3ClientConfiguration clientConfig;
            clientConfig.region = region;
            Aws::S3::S3Client client(credentials, nullptr, clientConfig);

            string objectKey = getObjectKey(requestInfo);

            Aws::S3::Model::PutObjectRequest request;
            request.SetBucket(Aws::String(bucket));
            request.SetKey(Aws::String(objectKey));

            const std::shared_ptr<Aws::IOStream> inputData = Aws::MakeShared<Aws::StringStream>("");
            *inputData << j.dump().c_str();

            request.SetBody(inputData);

            Aws::S3::Model::PutObjectOutcome outcome = client.PutObject(request);

            if (!outcome.IsSuccess())
            {
                cout << outcome.GetError() << endl;
            }
        }
    };
}