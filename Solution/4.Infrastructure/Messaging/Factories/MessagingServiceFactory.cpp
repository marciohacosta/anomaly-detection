#pragma once
#include <fstream>
#include "../Base/IMessagingService.h"
#include "../Providers/KafkaMessagingService.cpp"
#include "../Providers/AwsSqsMessagingService.cpp"
#include "../Providers/AzureEventGridMessagingService.cpp"

using namespace std;

namespace MSWay::IAM::Infrastructure
{
    class MessagingServiceFactory
    {
    public:
        MessagingServiceFactory(){}

        shared_ptr<IMessagingService> build(const string provider)
        {
            cout << "Messaging service provider: " << provider << endl;

            if (provider == "Kafka")
            {
                string brokers = getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_BROKERS");
                int timeout = stoi(getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_TIMEOUT"));
                string lingerms = getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_LINGERMS");

                cout << "Brokers : " << brokers << endl;
                cout << "Timeout : " << timeout << endl;
                cout << "Lingerms: " << lingerms << endl;

                return make_shared<KafkaMessagingService>(brokers, chrono::milliseconds(timeout), lingerms);
            }

            if (provider == "AWSSQS")
            {
                string accessKey = getenv("MSWAY_IAM_ANALYSER_AWSACCESSKEY");
                string secretKey = getenv("MSWAY_IAM_ANALYSER_AWSSECRETKEY");
                string region = getenv("MSWAY_IAM_ANALYSER_AWSREGION");

                cout << "AWS SQS region: " << region << endl;

                return make_shared<AwsSqsMessagingService>(region, accessKey, secretKey);
            }

            if (provider == "AzureEventGrid")
            {
                string endpoint = getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_AZUREEVENTGRID_ENDPOINT");
                string accessKey = getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_AZUREEVENTGRID_ACCESSKEY");

                cout << "Azure Event Grid endpoint: " << endpoint << endl;

                return make_shared<AzureEventGridMessagingService>(endpoint, accessKey);
            }

            throw "Invalid messaging service provider!";
        }
    };
}