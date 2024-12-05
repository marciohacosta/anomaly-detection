#pragma once
#include <string>

using namespace std;

namespace MSWay::IAM::Helpers
{
    const string TRIM_PATTERN = " \n\r\t";

    class StringHelper
    {
    public:
        static string trim(const string &value)
        {
            string trimmed(value);

            trimmed.erase(0, trimmed.find_first_not_of(TRIM_PATTERN));
            trimmed.erase(trimmed.find_last_not_of(TRIM_PATTERN) + 1);

            return trimmed;
        }

        static string toLowerCase(const string &value)
        {
            string lowered(value);

            transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char c) { return tolower(c); } );

            return lowered;
        }

        static string toTrimmedLowerCase(const string &value)
        {
            return toLowerCase(trim(value));
        }
    };
}