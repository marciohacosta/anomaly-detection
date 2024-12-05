#include <iostream>
#include "crow.h"
#include "asio.hpp"
#include "./Solution/3.Domain/Repository/Base/ISummaryRepository.h"
#include "./Solution/3.Domain/Repository/Factories/SummaryRepositoryFactory.cpp"
#include "./Solution/4.Infrastructure/Messaging/Base/IMessagingService.h"
#include "./Solution/4.Infrastructure/Messaging/Factories/MessagingServiceFactory.cpp"
#include "./Solution/3.Domain/Service/Base/IAnalysisService.h"
#include "./Solution/3.Domain/Service/Factories/AnalysisServiceFactory.cpp"
#include "./Solution/2.Application/Base/IApplicationService.h"
#include "./Solution/2.Application/Factories/ApplicationServiceFactory.cpp"

using namespace std;

int main()
{
    // AWS SDK initialization
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    // Get parameters from environment
    int appPort = stoi(getenv("MSWAY_IAM_ANALYSER_APPLICATION_PORT"));
    string appVersion = getenv("MSWAY_IAM_ANALYSER_APPLICATION_VERSION");
    string summaryRepositoryProvider = getenv("MSWAY_IAM_ANALYSER_SUMMARYREPOSITORY_PROVIDER");
    string messagingServiceProvider = getenv("MSWAY_IAM_ANALYSER_MESSAGINGSERVICE_PROVIDER");
    string analysisServiceProvider = getenv("MSWAY_IAM_ANALYSER_ANALYSISSERVICE_PROVIDER");
    string applicationServiceProvider = getenv("MSWAY_IAM_ANALYSER_APPLICATIONSERVICE_PROVIDER");

    // Stdout configuration
    cout << "Analyser version: " << appVersion << endl;
    cout << "Application port: " << appPort << endl;

    MSWay::IAM::Repository::SummaryRepositoryFactory summaryRepositoryFactory;
    shared_ptr<MSWay::IAM::Repository::ISummaryRepository> ptrSummaryRepository = summaryRepositoryFactory.build(summaryRepositoryProvider);

    MSWay::IAM::Infrastructure::MessagingServiceFactory messagingServiceFavtory;
    shared_ptr<MSWay::IAM::Infrastructure::IMessagingService> ptrMessagingService = messagingServiceFavtory.build(messagingServiceProvider);

    MSWay::IAM::Service::AnalysisServiceFactory analysisServiceFactory;
    shared_ptr<MSWay::IAM::Service::IAnalysisService> ptrAnalysisService = analysisServiceFactory.build(analysisServiceProvider, ptrSummaryRepository, ptrMessagingService);

    MSWay::IAM::Application::ApplicationServiceFactory applicationServiceFactory;
    shared_ptr<MSWay::IAM::Application::IApplicationService> ptrApplicationService = applicationServiceFactory.build(applicationServiceProvider, ptrAnalysisService);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/analyzer/health/liveness")([&appVersion]()
    {
        return "Analyzer " + appVersion + " is live!";
    });

    CROW_ROUTE(app, "/analyzer/health/readiness")([&appVersion]()
    {
        return "Analyzer " + appVersion + " is ready!";
    });

    CROW_ROUTE(app, "/analyzer/anomaly/load")([&](const crow::request& httpRequest)
    {
        try
        {
            int result = ptrApplicationService->load(httpRequest);
            return crow::response(result);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return crow::response(500);
        }
    });

    CROW_ROUTE(app, "/analyzer/anomaly/analyse")([&](const crow::request& httpRequest)
    {
        try
        {
            int result = ptrApplicationService->analyse(httpRequest);
            return crow::response(result);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return crow::response(500);
        }
    });

    app.loglevel(crow::LogLevel::Critical).port(appPort).multithreaded().run();

    Aws::ShutdownAPI(options);
}