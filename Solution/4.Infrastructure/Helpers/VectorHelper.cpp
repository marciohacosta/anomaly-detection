#pragma once
#include <string>
#include <vector>

using namespace std;

namespace MSWay::IAM::Helpers
{
    class VectorHelper
    {
    public:
        VectorHelper();
        ~VectorHelper();

        static int getIndex(vector<string> v, string k)
        {
            auto it = find(v.begin(), v.end(), k);

            if (it == v.end())
            {
                return -1;
            }

            return it - v.begin();
        }
    };
}