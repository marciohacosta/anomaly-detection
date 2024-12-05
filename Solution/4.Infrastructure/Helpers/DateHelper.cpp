#pragma once
#include <chrono>
#include <string>
#include "./FormatHelper.cpp"

using namespace std;

namespace MSWay::IAM::Helpers
{
    const int baseYear = 1900;
    const int baseMonth = 1;
    const chrono::duration<int, ratio<86400>> oneDay(1);

    class DateHelper
    {
    private:

    public:
        static string toString(chrono::system_clock::system_clock::time_point tp)
        {
            time_t tt = chrono::system_clock::system_clock::to_time_t(tp);
            return toString(*gmtime(&tt));
        }

        static string toString(const tm &date)
        {
            int year = date.tm_year + baseYear;
            int month = date.tm_mon + baseMonth;
            int day = date.tm_mday;

            string formatedDate = FormatHelper::formatZeroes(year, 4) + FormatHelper::formatZeroes(month, 2) + FormatHelper::formatZeroes(day, 2);

            return formatedDate;
        }

        static string toHourString(const tm& date, bool useTimeSeparator = false)
        {
            int hour = date.tm_hour;
            int minute = date.tm_min;
            int second = date.tm_sec;

            string separator = useTimeSeparator ? ":" : "";

            return FormatHelper::formatZeroes(hour, 2) + separator + FormatHelper::formatZeroes(minute, 2) + separator + FormatHelper::formatZeroes(second, 2);
        }
        
        static tm toTM(const string date)
        {
            tm utctm{0};

            utctm.tm_year = stoi(date.substr(0, 4)) - baseYear;
            utctm.tm_mon = stoi(date.substr(4, 2)) - baseMonth;
            utctm.tm_mday = stoi(date.substr(6, 2));

            return utctm;
        }
    };
}