#pragma once
#include <map>
#include <string>
#include <vector>
#include "csv/csv.hpp"

bool is_numeric(const std::string& str);
bool column_is_numeric(const Data& data, std::size_t col);
std::vector<double> extract_column(const Data& data, std::size_t col);
std::size_t get_count(const std::vector<double>& column);
double get_min(const std::vector<double>& column);
double get_max(const std::vector<double>& column);
double calculate_sum(const std::vector<double>& column);
double calculate_mean(double sum_val, std::size_t count_val);
double calculate_stddev(const std::vector<double>& column, double mean_val);

struct ColumnStats {
    std::size_t count;
    double min, max, mean, stddev;
};
ColumnStats summarize(const std::vector<double>& values);

std::vector<std::size_t> numeric_columns(const Data& d);

// Values of value_col grouped by the text in key_col; keys iterate in sorted order.
// Throws std::out_of_range for a bad column index,
// std::invalid_argument if value_col is not numeric.
std::map<std::string, std::vector<double>>
group_by(const Data& d, std::size_t key_col, std::size_t value_col);
