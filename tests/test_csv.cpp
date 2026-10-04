#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include "csv/csv.hpp"

TEST(ParseCsv, StripsCarriageReturnFromHeader) {
    std::istringstream in("a,b,humidity\r\n1,2,3\r\n");
    Data d = parse_csv(in);
    ASSERT_EQ(d.header.size(), 3u);
    EXPECT_EQ(d.header[2], "humidity");
}

TEST(ParseCsv, StripsCarriageReturnFromDataRows) {
    std::istringstream in("a,b\r\n1,2\r\n");
    Data d = parse_csv(in);
    ASSERT_EQ(d.rows.size(), 1u);
    ASSERT_EQ(d.rows[0].size(), 2u);
    EXPECT_EQ(d.rows[0][1], "2");
}

TEST(ParseCsv, SkipsBlankCrlfLine) {
    std::istringstream in("a,b\r\n1,2\r\n\r\n3,4\r\n");
    Data d = parse_csv(in);
    EXPECT_EQ(d.rows.size(), 2u);
}

TEST(ParseCsv, SkipsRaggedRows) {
    std::istringstream in("a,b\n1,2\n3\n4,5\n");
    Data d = parse_csv(in);
    ASSERT_EQ(d.rows.size(), 2u);
    EXPECT_EQ(d.rows[0][0], "1");
    EXPECT_EQ(d.rows[1][0], "4");
}

TEST(ParseCsv, KeepsTrailingEmptyField) {
    std::istringstream in("a,b,c\n1,2,\n");
    Data d = parse_csv(in);
    ASSERT_EQ(d.rows.size(), 1u);
    ASSERT_EQ(d.rows[0].size(), 3u);
    EXPECT_EQ(d.rows[0][2], "");
}

TEST(LoadCsv, MissingFileThrows) {
    EXPECT_THROW(load_csv("definitely_not_here.csv"), std::runtime_error);
}

TEST(LoadCsv, DirectoryThrows) {
    EXPECT_THROW(load_csv("."), std::runtime_error);
}

TEST(ParseCsv, KeepsTrailingEmptyFieldWithCrlf) {
    std::istringstream in("a,b,c\r\n1,2,\r\n");
    Data d = parse_csv(in);
    ASSERT_EQ(d.rows.size(), 1u);
    ASSERT_EQ(d.rows[0].size(), 3u);
    EXPECT_EQ(d.rows[0][2], "");
}

TEST(ParseCsv, EmptyInputThrows) {
    std::istringstream in("");
    EXPECT_THROW(parse_csv(in), std::runtime_error);
}

TEST(ParseCsv, BlankFirstLineThrows) {
    std::istringstream in("\r\n");
    EXPECT_THROW(parse_csv(in), std::runtime_error);
}

TEST(ParseCsv, HeaderOnlyGivesNoRows) {
    std::istringstream in("a,b\n");
    Data d = parse_csv(in);
    ASSERT_EQ(d.header.size(), 2u);
    EXPECT_TRUE(d.rows.empty());
}
