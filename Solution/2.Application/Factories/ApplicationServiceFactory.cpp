#pragma once
#include <fstream>
#include "../Base/IApplicationService.h"
#include "../Providers/HttpApplicationService.cpp"
#include "../../3.Domain/Service/Base/IAnalysisService.h"

using namespace std;

namespace MSWay::IAM::Application
{
    class ApplicationServiceFactory
    {
    public:
        ApplicationServiceFactory(){}
        
        shared_ptr<IApplicationService> build(const string provider, shared_ptr<MSWay::IAM::Service::IAnalysisService> ptrAnalysisService)
        {
            cout << "Application service provider: " << provider << endl;

            if (provider == "HTTP")
            {
                return make_shared<HttpApplicationService>(ptrAnalysisService);
            }

            throw "Invalid application service provider!";
        }
    };
}