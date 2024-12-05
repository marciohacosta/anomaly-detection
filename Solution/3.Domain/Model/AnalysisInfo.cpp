#pragma once
#include <string>
#include "RequestInfo.cpp"
#include "../../5.External/Include/json.hpp"

using namespace std;

namespace MSWay::IAM::Model
{
    class AnalysisInfo
    {
    private:
        string user;
        string resource;
        string method;
        string date;
        string hour;
        bool isAnomaly;
        
    public:
        AnalysisInfo(RequestInfo& requestInfo, const string date, const string hour)
        {
            this->user = requestInfo.getUser();
            this->resource = requestInfo.getResource();
            this->method = requestInfo.getMethod();
            this->date = date;
            this->hour = hour;
            this->isAnomaly = false;
        }

        ~AnalysisInfo() = default;

        string getKey()
        {
            return user + UNDERSCORE + resource + UNDERSCORE + method;
        }

        void setIsAnomaly(bool value)
        {
            this->isAnomaly = value;
        }

        void to_json(nlohmann::json& j)
        {
            j = nlohmann::json
            {
                {"user", user},
                {"resource", resource},
                {"method", method},
                {"date", date},
                {"hour", hour},
                {"isAnomaly", isAnomaly}
            };
        }
    };
}