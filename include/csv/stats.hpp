#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include "csv.hpp"

bool is_numeric(const std::string& str);
bool column_is_numeric(const Data& data, std::size_t col);
std::vector<double> extract_column(const Data& data, std::size_t col);
std::size_t get_count(const std::vector<double>& column);
double get_min(const std::vector<double>& column);
double get_max(const std::vector<double>& column);
double calculate_sum(const std::vector<double>& column);
double calculate_mean(double sum_val, std::size_t count_val);
double calculate_stddev(const std::vector<double>& column, double mean_val);