cmake_minimum_required(VERSION 3.10)
project(CalcGTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(COVERAGE_FLAGS "-g -O0 --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

add_library(Calclib
    ICalculator.h
    SimpleCalculator.h
    SimpleCalculator.cpp
)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    Calculator_test
    CalculatorTest.cpp
    InMemoryHistory.cpp
)

target_link_libraries(
    Calculator_test
    Calclib
    GTest::gtest_main
    GTest::gmock_main
    pthread
)