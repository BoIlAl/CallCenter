cmake_minimum_required(VERSION 3.0.0)
project(CallCenterProj)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    P7
    URL https://github.com/CMakePorts/libp7-baical/archive/refs/tags/v5.2.tar.gz
)
FetchContent_MakeAvailable(P7)

set(CALL_CENTER src/CDR.cpp src/httpServer.cpp 
    src/indexGenerator.cpp src/CDRHandlerCfg.cpp src/CDRHandler.cpp
    src/callCenter.cpp src/operator.cpp src/operatorManager.cpp
    src/operatorManagerCfg.cpp src/ICfg.cpp src/timeoutQueue.cpp)
set(MAIN main.cpp)
set(TESTS tests/cfgTest.cpp tests/commonTest.cpp
    tests/operatorTest.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/httplib)

add_executable(callCenter ${CALL_CENTER} ${MAIN})
add_executable(callCenterTests ${CALL_CENTER} ${TESTS})

target_link_libraries(callCenter PRIVATE nlohmann_json::nlohmann_json P7)
target_link_libraries(callCenterTests PRIVATE GTest::gtest_main nlohmann_json::nlohmann_json P7)

enable_testing()
include(GoogleTest)
gtest_discover_tests(callCenterTests)