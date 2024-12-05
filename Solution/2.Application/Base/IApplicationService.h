#pragma once
#include <crow.h>

using namespace std;

namespace MSWay::IAM::Application
{
    class IApplicationService
    {
    public:
        virtual ~IApplicationService() = default;

        virtual int load(const crow::request& httpRequest) = 0;
        virtual int analyse(const crow::request& httpRequest) = 0;
    };
}