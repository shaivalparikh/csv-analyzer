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

TEST(LoadCsv, MissingFileThrows) {
    EXPECT_THROW(load_csv("definitely_not_here.csv"), std::runtime_error);
}