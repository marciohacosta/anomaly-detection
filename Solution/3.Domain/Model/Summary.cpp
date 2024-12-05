#pragma once
#include <chrono>
#include <string>
#include <vector>
#include "../../4.Infrastructure/Helpers/DateHelper.cpp"
#include "../../4.Infrastructure/Helpers/VectorHelper.cpp"
#include "../../5.External/Include/json.hpp"

using namespace std;

namespace MSWay::IAM::Model
{
    class Summary
    {
    private:

        const string FILL_HOUR = "000000";
        const float FILL_COUNT = 0.0;
        const chrono::duration<int, ratio<86400>> oneDay {1};

        string lastRequestDate;
        int daysCount;
        vector<string> dates;
        vector<string> hours;
        vector<float> series;

        void fillGap(string referenceDate)
        {
            if (lastRequestDate == "")
            {
                return;
            }

            tm lastTM = MSWay::IAM::Helpers::DateHelper::toTM(lastRequestDate);
            time_t lastTT = mktime(&lastTM);
            chrono::system_clock::system_clock::time_point lastTP = chrono::system_clock::system_clock::from_time_t(lastTT);

            tm refTM = MSWay::IAM::Helpers::DateHelper::toTM(referenceDate);
            time_t refTT = mktime(&refTM);
            chrono::system_clock::system_clock::time_point refTP = chrono::system_clock::system_clock::from_time_t(refTT);
            chrono::system_clock::system_clock::time_point minRef = refTP - oneDay;

            if (lastTP >= minRef)
            {
                return;
            }

            while (lastTP < minRef)
            {
                lastTP = lastTP + oneDay;
                daysCount++;

                string lrd = MSWay::IAM::Helpers::DateHelper::toString(lastTP);

                dates.push_back(lrd);
                hours.push_back(FILL_HOUR);
                series.push_back(FILL_COUNT);
            }
        }

    public:

        Summary()
        {
            this->lastRequestDate = "";
            this->daysCount = 0;
        }

        Summary(string lastRequestDate, int daysCount, vector<string> dates, vector<string> hours, vector<float> series)
        {
            this->lastRequestDate = lastRequestDate;
            this->daysCount = daysCount;
            this->dates = dates;
            this->hours = hours;
            this->series = series;
        }

        string getLastRequestDate()
        {
            return lastRequestDate;
        }

        void setLastRequestDate(string lastRequestDate)
        {
            this->lastRequestDate = lastRequestDate;
        }

        int getDaysCount()
        {
            return daysCount;
        }

        void setDaysCount(int daysCount)
        {
            this->daysCount = daysCount;
        }

        vector<string> getDates()
        {
            return dates;
        }

        void setDates(vector<string> dates)
        {
            this->dates = dates;
        }

        vector<string> getHours()
        {
            return hours;
        }

        void setHours(vector<string> hours)
        {
            this->hours = hours;
        }

        vector<float> getSeries()
        {
            return series;
        }

        void setSeries(vector<float> series)
        {
            this->series = series;
        }

        void add(string date, string hour, int maxDays)
        {
            fillGap(date);

            int index = MSWay::IAM::Helpers::VectorHelper::getIndex(dates, date);

            if (index == -1)
            {
                lastRequestDate = date;
                daysCount++;

                dates.push_back(date);
                hours.push_back(hour);
                series.push_back(1);

                // Purge older values based on max days
                int datesDiff = daysCount - maxDays;

                if (datesDiff > 0)
                {
                    // Dates range purge
                    vector<string>::iterator datesBegin = dates.begin();
                    vector<string>::iterator datesEnd = dates.begin() + datesDiff;

                    dates.erase(datesBegin, datesEnd);
                    
                    // Hours range purge
                    vector<string>::iterator hoursBegin = hours.begin();
                    vector<string>::iterator hoursEnd = hours.begin() + datesDiff;
                    
                    hours.erase(hoursBegin, hoursEnd);
                    
                    // Series range purge
                    vector<float>::iterator seriesBegin = series.begin();
                    vector<float>::iterator seriesEnd = series.begin() + datesDiff;

                    series.erase(seriesBegin, seriesEnd);

                    // Update days count
                    daysCount -= datesDiff;
                }

                return;
            }

            hours[index] = hours[index] + ";" + hour;
            series[index] = series[index] + 1;
        }

        void to_json(nlohmann::json& j)
        {
            j = nlohmann::json
            {
                {"lastRequestDate", lastRequestDate},
                {"daysCount", daysCount},
                {"dates", dates},
                {"hours", hours},
                {"series", series}
            };
        }

        static void from_json(const nlohmann::json& j, Summary& s)
        {
            j.at("lastRequestDate").get_to(s.lastRequestDate);
            j.at("daysCount").get_to(s.daysCount);
            j.at("dates").get_to(s.dates);
            j.at("hours").get_to(s.hours);
            j.at("series").get_to(s.series);
        }
    };
}