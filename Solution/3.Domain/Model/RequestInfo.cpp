#pragma once
#include <string>
#include <vector>
#include <regex>
#include "./Summary.cpp"
#include "../../4.Infrastructure/Helpers/StringHelper.cpp"

using namespace std;

namespace MSWay::IAM::Model
{
    const string UNDERSCORE = "_";
    const string PATTERN = "[^a-zA-Z0-9]";

    class RequestInfo
    {
    private:

        string user;
        string resource;
        string method;
        Summary summary;

    public:

        RequestInfo(string requestUser, string requestResource, string requestMethod)
        {
            user = MSWay::IAM::Helpers::StringHelper::toTrimmedLowerCase(requestUser);
            resource = MSWay::IAM::Helpers::StringHelper::toTrimmedLowerCase(requestResource);
            method = MSWay::IAM::Helpers::StringHelper::toTrimmedLowerCase(requestMethod);
        };
        
        string getUser()
        {
            return user;
        }

        string getResource()
        {
            return resource;
        }

        string getMethod()
        {
            return method;
        }

        Summary* getSummary()
        {
            return &this->summary;
        }

        void setSummary(MSWay::IAM::Model::Summary& summary)
        {
            this->summary.setLastRequestDate(summary.getLastRequestDate());
            this->summary.setDaysCount(summary.getDaysCount());
            this->summary.setDates(summary.getDates());
            this->summary.setHours(summary.getHours());
            this->summary.setSeries(summary.getSeries());
        }

        string getToken()
        {
            string plainToken = user + UNDERSCORE + resource + UNDERSCORE + method;
            
            regex pattern(PATTERN);

            return regex_replace(plainToken, pattern, UNDERSCORE);
        }
    };
}