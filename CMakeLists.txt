cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(SERVICE_COMPONENTS s3 sqs)

project(AnomalyDetection CXX)

find_package(Crow REQUIRED)
find_package(CppKafka REQUIRED)
find_package(aws-cpp-sdk-core REQUIRED)
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})
find_package(cpr REQUIRED)
find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME} app.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ./Solution/5.External/Include ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} Crow::Crow CppKafka::cppkafka ${AWSSDK_LINK_LIBRARIES} cpr::cpr)
