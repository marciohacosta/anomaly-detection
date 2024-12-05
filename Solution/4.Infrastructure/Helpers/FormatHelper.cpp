#pragma once
#include <string>

using namespace std;

namespace MSWay::IAM::Helpers
{
    class FormatHelper
    {
    public:
        static string formatZeroes(const int value, const int size)
        {
            string result = to_string(value);
            int diff = size - result.length();

            if (diff > 0)
            {
                result = string(diff, '0') + result;
            }

            return result;
        }
    };
}