#include <crow.h>
#include "../Base/IApplicationService.h"
#include "../../3.Domain/Service/Base/IAnalysisService.h"

using namespace std;

namespace MSWay::IAM::Application
{
    class HttpApplicationService : public IApplicationService
    {
    private:
        shared_ptr<MSWay::IAM::Service::IAnalysisService> ptrAnalysisService;

        MSWay::IAM::Model::RequestInfo createRequestInfo(const crow::request& httpRequest)
        {
            string user = httpRequest.url_params.get("user");
            string resource = httpRequest.url_params.get("resource");
            string method = httpRequest.url_params.get("method");

            MSWay::IAM::Model::RequestInfo requestInfo{user, resource, method};

            return requestInfo;
        }

    public:
        HttpApplicationService(shared_ptr<MSWay::IAM::Service::IAnalysisService> ptrAnalysisService)
        {
            this->ptrAnalysisService = ptrAnalysisService;
        }

        int load(const crow::request& httpRequest) override
        {
            MSWay::IAM::Model::RequestInfo requestInfo = this->createRequestInfo(httpRequest);

            string date = httpRequest.url_params.get("date");
            string hour = httpRequest.url_params.get("hour");

            int result = ptrAnalysisService->load(requestInfo, date, hour);

            return result;
        }

        int analyse(const crow::request& httpRequest) override
        {
           MSWay::IAM::Model::RequestInfo requestInfo = this->createRequestInfo(httpRequest);

           int result = ptrAnalysisService->analyse(requestInfo);

           return result;
        }
    };
}