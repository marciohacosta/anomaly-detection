#pragma once
#include "../../3.Domain/Model/RequestInfo.cpp"

using namespace std;

namespace MSWay::IAM::Service
{
    class IAnalysisService
    {
    public:
        virtual ~IAnalysisService() = default;

        virtual int load(MSWay::IAM::Model::RequestInfo& requestInfo, string date, string hour) = 0;
        virtual int analyse(MSWay::IAM::Model::RequestInfo& requestInfo) = 0;
    };
}