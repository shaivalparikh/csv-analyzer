#include <gtest/gtest.h>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "csv/csv.hpp"
#include "csv/stats.hpp"

TEST(IsNumeric, AcceptsNumbers) {
    EXPECT_TRUE(is_numeric("3.14"));
    EXPECT_TRUE(is_numeric("-1e3"));
    EXPECT_TRUE(is_numeric("42"));
}

TEST(IsNumeric, RejectsNonNumbers) {
    EXPECT_FALSE(is_numeric(""));
    EXPECT_FALSE(is_numeric("3.14abc"));
    EXPECT_FALSE(is_numeric("N/A"));
}

TEST(Summarize, KnownValues) {
    ColumnStats s = summarize({2, 4, 4, 4, 5, 5, 7, 9});
    EXPECT_EQ(s.count, 8u);
    EXPECT_DOUBLE_EQ(s.min, 2.0);
    EXPECT_DOUBLE_EQ(s.max, 9.0);
    EXPECT_DOUBLE_EQ(s.mean, 5.0);
    EXPECT_NEAR(s.stddev, 2.0, 1e-12);
}

TEST(Summarize, SingleValue) {
    ColumnStats s = summarize({3.5});
    EXPECT_EQ(s.count, 1u);
    EXPECT_DOUBLE_EQ(s.min, 3.5);
    EXPECT_DOUBLE_EQ(s.max, 3.5);
    EXPECT_DOUBLE_EQ(s.mean, 3.5);
    EXPECT_DOUBLE_EQ(s.stddev, 0.0);
}

TEST(Summarize, EmptyThrows) {
    EXPECT_THROW(summarize({}), std::invalid_argument);
}

TEST(Summarize, NegativeValues) {
    ColumnStats s = summarize({-1, 1});
    EXPECT_EQ(s.count, 2u);
    EXPECT_DOUBLE_EQ(s.min, -1.0);
    EXPECT_DOUBLE_EQ(s.max, 1.0);
    EXPECT_DOUBLE_EQ(s.mean, 0.0);
    EXPECT_NEAR(s.stddev, 1.0, 1e-12);
}

TEST(NumericColumns, MixedColumns) {
    std::istringstream in("id,name,val\n1,a,2.5\n2,b,3.0\n");
    Data d = parse_csv(in);
    EXPECT_EQ(numeric_columns(d), (std::vector<std::size_t>{0, 2}));
}

TEST(NumericColumns, OneBadValueMakesColumnText) {
    std::istringstream in("id,name,val\n1,a,2.5\n2,b,N/A\n");
    Data d = parse_csv(in);
    EXPECT_EQ(numeric_columns(d), (std::vector<std::size_t>{0}));
}

TEST(NumericColumns, EmptyFieldMakesColumnText) {
    std::istringstream in("a,b\n1,2\n3,\n");
    Data d = parse_csv(in);
    EXPECT_EQ(numeric_columns(d), (std::vector<std::size_t>{0}));
}

TEST(NumericColumns, HeaderOnlyHasNone) {
    std::istringstream in("a,b\n");
    Data d = parse_csv(in);
    EXPECT_TRUE(numeric_columns(d).empty());
}

TEST(ColumnIsNumeric, FalseWhenNoRows) {
    std::istringstream in("a,b\n");
    Data d = parse_csv(in);
    EXPECT_FALSE(column_is_numeric(d, 0));
}

using Groups = std::map<std::string, std::vector<double>>;

TEST(GroupBy, TwoGroups) {
    std::istringstream in("id,val\nB,2\nA,1\nA,3\n");
    Data d = parse_csv(in);
    EXPECT_EQ(group_by(d, 0, 1), (Groups{{"A", {1, 3}}, {"B", {2}}}));
}

TEST(GroupBy, NonNumericValueColumnThrows) {
    std::istringstream in("id,val\nB,2\nA,N/A\nA,3\n");
    Data d = parse_csv(in);
    EXPECT_THROW(group_by(d, 0, 1), std::invalid_argument);
}

TEST(GroupBy, ColumnOutOfRangeThrows) {
    std::istringstream in("id,val\nB,2\nA,1\n");
    Data d = parse_csv(in);
    EXPECT_THROW(group_by(d, 0, 5), std::out_of_range);
}

TEST(GroupBy, HeaderOnlyThrows) {
    std::istringstream in("id,val\n");
    Data d = parse_csv(in);
    EXPECT_THROW(group_by(d, 0, 1), std::invalid_argument);
}
