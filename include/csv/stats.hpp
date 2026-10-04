#pragma once
#include <map>
#include <string>
#include <vector>
#include "csv/csv.hpp"

bool is_numeric(const std::string& str);
bool column_is_numeric(const Data& data, std::size_t col);
std::vector<double> extract_column(const Data& data, std::size_t col);

struct ColumnStats {
    std::size_t count;
    double min, max, mean, stddev;
};
ColumnStats summarize(const std::vector<double>& values);

std::vector<std::size_t> numeric_columns(const Data& d);

std::map<std::string, std::vector<double>>
group_by(const Data& d, std::size_t key_col, std::size_t value_col);
