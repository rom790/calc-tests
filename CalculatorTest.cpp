// calculator_tests.cpp

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SimpleCalculator.h"
#include "InMemoryHistory.h"


class MockHistory : public calc::IHistory {
public:
    MOCK_METHOD(void, AddEntry, (const std::string& operation), (override));
    MOCK_METHOD(std::vector<std::string>, GetLastOperations, (size_t count), (const, override));
};

class MockCalculator : public calc::ICalculator {
public:
    MOCK_METHOD(int, Add, (int a, int b), (override));
    MOCK_METHOD(int, Subtract, (int a, int b), (override));
    MOCK_METHOD(int, Multiply, (int a, int b), (override));
    MOCK_METHOD(int, Divide, (int a, int b), (override));
    MOCK_METHOD(void, SetHistory, (calc::IHistory& history), (override));
};

namespace calc {

TEST(CalculatorTest, AddLogsCorrectly) {
    // Arrange
    MockHistory history;
    SimpleCalculator calc(history);
    EXPECT_CALL(history, AddEntry(testing::StrEq("2 + 2 = 4")));

    // Act
    int result = calc.Add(2, 2);

    // Assert
    EXPECT_EQ(result, 4);
}

TEST(CalculatorTest, DivideLogsCorrectly) {
    // Arrange
    MockHistory history;
    SimpleCalculator calc(history);
    EXPECT_CALL(history, AddEntry(testing::StrEq("10 / 2 = 5")));

    // Act
    int result = calc.Divide(10, 2);

    // Assert
    EXPECT_EQ(result, 5);
}

TEST(CalculatorTest, DivisionByZeroThrows) {
    // Arrange
    InMemoryHistory history;
    SimpleCalculator calc(history);

    // Act & Assert
    EXPECT_THROW(calc.Divide(5, 0), std::runtime_error);
}

TEST(CalculatorTest, MultiplyEdgeCase) {
    // Arrange
    InMemoryHistory history;
    SimpleCalculator calc(history);

    // Act
    int result = calc.Multiply(INT32_MAX, 0);

    // Assert
    EXPECT_EQ(result, 0);
}

TEST(CalculatorTest, SubtractNegativeResult) {
    // Arrange
    InMemoryHistory history;
    SimpleCalculator calc(history);

    // Act
    int result = calc.Subtract(5, 10);

    // Assert
    EXPECT_EQ(result, -5);
}

TEST(CalculatorTest, HistoryStoresOperations) {
    // Arrange
    InMemoryHistory history;
    SimpleCalculator calc(history);

    // Act
    calc.Add(1, 2);
    calc.Multiply(3, 4);
    auto lastOps = history.GetLastOperations(2);

    // Assert
    ASSERT_EQ(lastOps.size(), 2);
    EXPECT_EQ(lastOps[0], "1 + 2 = 3");
    EXPECT_EQ(lastOps[1], "3 * 4 = 12");
}

TEST(CalculatorTest, SetHistoryChangesDestination) {
    // Arrange
    InMemoryHistory history1;
    InMemoryHistory history2;
    SimpleCalculator calc(history1);

    // Act
    calc.Add(1, 1);  // -> history1
    calc.SetHistory(history2);
    calc.Add(2, 2);  // -> history2

    auto hist1 = history1.GetLastOperations(1);
    auto hist2 = history2.GetLastOperations(1);

    // Assert
    ASSERT_EQ(hist1.size(), 1);
    ASSERT_EQ(hist2.size(), 1);
    EXPECT_EQ(hist1[0], "1 + 1 = 2");
    EXPECT_EQ(hist2[0], "2 + 2 = 4");
}

TEST(HistoryTest, GetLastOperationsReturnsCorrectOrder) {
    // Arrange
    InMemoryHistory history;
    history.AddEntry("op1");
    history.AddEntry("op2");
    history.AddEntry("op3");

    // Act
    auto lastTwo = history.GetLastOperations(2);

    // Assert
    ASSERT_EQ(lastTwo.size(), 2);
    EXPECT_EQ(lastTwo[0], "op2");
    EXPECT_EQ(lastTwo[1], "op3");
}

}
