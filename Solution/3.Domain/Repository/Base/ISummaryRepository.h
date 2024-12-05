#pragma once
#include "../../3.Domain/Model/RequestInfo.cpp"

using namespace std;

namespace MSWay::IAM::Repository
{
    class ISummaryRepository
    {
    public:
        virtual ~ISummaryRepository() = default;

        virtual void loadSummary(MSWay::IAM::Model::RequestInfo& requestInfo) = 0;
        virtual void saveSummary(MSWay::IAM::Model::RequestInfo& requestInfo) = 0;
    };
}