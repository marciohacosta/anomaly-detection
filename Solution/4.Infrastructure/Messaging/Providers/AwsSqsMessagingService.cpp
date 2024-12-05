#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <aws/core/Aws.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageRequest.h>
#include "../Base/IMessagingService.h"

using namespace std;

namespace MSWay::IAM::Infrastructure
{
    class AwsSqsMessagingService : public IMessagingService
    {
    private:
        string accessKey;
        string secretKey;
        string region;

    public:
        AwsSqsMessagingService(const string region, const string accessKey, const string secretKey)
        {
            this->accessKey = accessKey;
            this->secretKey = secretKey;
            this->region = region;
        }

        bool publish(const string queue, const string key, const string value, const int partition = -1) override
        {
            Aws::Auth::AWSCredentials credentials;
            credentials.SetAWSAccessKeyId(Aws::String(accessKey));
            credentials.SetAWSSecretKey(Aws::String(secretKey));

            Aws::Client::ClientConfiguration clientConfiguration;
            clientConfiguration.region = region;

            Aws::SQS::SQSClient sqsClient(credentials, nullptr, clientConfiguration);
            Aws::SQS::Model::SendMessageRequest request;
            request.SetQueueUrl(queue);
            request.SetMessageBody(value);

            try
            {
                const Aws::SQS::Model::SendMessageOutcome outcome = sqsClient.SendMessage(request);
                return outcome.IsSuccess();
            }
            catch(const exception& e)
            {
                cout << "Publish has failed to AWS SQS queue " << queue << endl;
                return false;
            }
        }
    };
}