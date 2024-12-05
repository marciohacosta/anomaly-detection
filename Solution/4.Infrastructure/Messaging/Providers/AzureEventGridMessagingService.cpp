#pragma once
#include <cstdlib>
#include <iostream>
#include <json.hpp>
#include <cpr/cpr.h>
#include <string>
#include "../Base/IMessagingService.h"

using namespace std;

namespace MSWay::IAM::Infrastructure
{
    class AzureEventGridMessagingService : public IMessagingService
    {
    private:
        string eventGridRestApi;
        string eventGridAccessKey;

        string buildEvent(const string data)
        {
            return "[" + data + "]";
        }

    public:
        AzureEventGridMessagingService(const string eventGridRestApi, const string eventGridAccessKey)
        {
            this->eventGridRestApi = eventGridRestApi;
            this->eventGridAccessKey = eventGridAccessKey;
        }

        bool publish(const string queue, const string key, const string value, const int partition = -1) override
        {

            string url = eventGridRestApi + "?overload=customEvent&api-version=2018-01-01";
            string js = buildEvent(value);

            //cout << "Event: " << js << endl;

            try
            {
                cpr::Response response = cpr::Post(cpr::Url{url}, cpr::Body{js}, cpr::Header{{"aeg-sas-key", eventGridAccessKey}}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Header{{"Content-Length", to_string(js.length())}});

                if (response.status_code != 200)
                {
                    cerr << "Publish has failed to Azure Event Grid " << queue << endl;
                    cerr << response.reason << endl;
                    return false;
                }

                return true;
            }
            catch(const exception& e)
            {
                cerr << "Publish has failed to Azure Event Grid " << queue << endl;
                cerr << e.what() << endl;

                return false;
            }
        }
    };
}