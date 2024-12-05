#pragma once
#include <string>

using namespace std;

namespace MSWay::IAM::Infrastructure
{
    class IMessagingService
    {
    public:
        virtual ~IMessagingService() = default;

        virtual bool publish(const string queue, const string key, const string value, const int partition = -1) = 0;
    };
}