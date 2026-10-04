#include "csv/stats.hpp"
#include "csv/csv.hpp"
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <algorithm>

bool is_numeric(const std::string& str) {
    if (str.empty()) return false;
    try {
        std::size_t pos = 0;
        std::stod(str, &pos);
        return pos == str.size();
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

bool column_is_numeric(const Data& data, std::size_t col) {
    if (data.rows.empty()) return false;
    for (const auto& row : data.rows) {
        if (col >= row.size() || !is_numeric(row[col]))
            return false;
    }
    return true;
}

std::vector<double> extract_column(const Data& data, std::size_t col) {
    if (data.rows.empty()) {
        throw std::invalid_argument("Data is empty.");
    }

    std::vector<double> values;
    for (const auto& row : data.rows) {
        if (row.size() <= col) {
            throw std::out_of_range("Row does not have enough columns.");
        }

        const std::string& value_str = row[col];
        if (!is_numeric(value_str)) {
            throw std::invalid_argument("Non-numeric value found in the specified column.");
        }

        values.push_back(std::stod(value_str));
    }
    return values;
}

std::size_t get_count(const std::vector<double>& column){
    return column.size();
}

double get_min(const std::vector<double>& column) {
    return *std::min_element(column.begin(), column.end());
}

double get_max(const std::vector<double>& column) {
    return *std::max_element(column.begin(), column.end());
}

double calculate_sum(const std::vector<double>& column) {
    return std::accumulate(column.begin(), column.end(), 0.0);
}

double calculate_mean(double sum_val, std::size_t count_val) {
    return sum_val / count_val;
}

double calculate_stddev(const std::vector<double>& column, double mean_val) {
    double sq_sum = std::accumulate(
        column.begin(),
        column.end(),
        0.0,
        [mean_val](double running, double value) {
            return running + (value - mean_val) * (value - mean_val);
        }
    );

    return std::sqrt(sq_sum / column.size());
}

ColumnStats summarize(const std::vector<double>& values) {
    if (values.empty()) {
        throw std::invalid_argument("summarize: empty input");
    }

    std::size_t count = get_count(values);
    double min = get_min(values);
    double max = get_max(values);
    double sum = calculate_sum(values);
    double mean = calculate_mean(sum, count);
    double stddev = calculate_stddev(values, mean);

    return {count, min, max, mean, stddev};
}

std::vector<std::size_t> numeric_columns(const Data& d) {
    std::vector<std::size_t> cols;
    for (std::size_t col = 0; col < d.header.size(); ++col) {
        if (column_is_numeric(d, col)) cols.push_back(col);
    }
    return cols;
}

std::map<std::string, std::vector<double>>
group_by(const Data& d, std::size_t key_col, std::size_t value_col) {
    if (key_col >= d.header.size() || value_col >= d.header.size()) {
        throw std::out_of_range("group_by: column index out of range");
    }
    if (!column_is_numeric(d, value_col)) {
        throw std::invalid_argument("group_by: value column is not numeric");
    }

    std::map<std::string, std::vector<double>> result;
    for (const auto& row : d.rows) {
        result[row[key_col]].push_back(std::stod(row[value_col]));
    }
    return result;
}
