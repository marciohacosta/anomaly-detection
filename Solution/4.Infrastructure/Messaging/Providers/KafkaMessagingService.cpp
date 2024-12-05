#pragma once
#include <cstdlib>
#include <iostream>
#include <string>
#include <cppkafka/cppkafka.h>
#include "../Base/IMessagingService.h"

using namespace std;
using namespace cppkafka;

namespace MSWay::IAM::Infrastructure
{
    class KafkaMessagingService : public IMessagingService
    {
    private:
        string brokers;
        chrono::milliseconds timeout;
        string lingerms;

    public:
        KafkaMessagingService(const string brokers, const chrono::milliseconds timeout, const string lingerms)
        {
            this->brokers = brokers;
            this->timeout = timeout;
            this->lingerms = lingerms;
        }

        bool publish(const string queue, const string key, const string value, const int partition = -1) override
        {
            Configuration config = {
                    { "metadata.broker.list", brokers },
                    { "linger.ms", lingerms }
                };

            Producer producer(config);

            try
            {
                producer.produce(MessageBuilder(queue).partition(partition).key(key).payload(value));
                producer.flush(timeout);

                return true;
            }
            catch(const exception& e)
            {
                cout << "Publish has failed to Kafka topic " << queue << endl;
                return false;
            }
        }
    };
}