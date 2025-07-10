cmake_minimum_required(VERSION 3.10)
project(CalcGTest)

# Настройка стандарта C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создание библиотеки проекта
add_library(Calclib
    ICalculator.h
    SimpleCalculator.h
    SimpleCalculator.cpp
)

# Загрузка Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

# Включение системы тестирования
enable_testing()

# Создание тестового исполняемого файла
add_executable(
    Calculator_test
    CalculatorTest.cpp
    InMemoryHistory.cpp
)

# Линковка тестов с библиотекой и Google Test
target_link_libraries(
    Calculator_test
    Calclib
    GTest::gtest_main
    GTest::gmock_main
)