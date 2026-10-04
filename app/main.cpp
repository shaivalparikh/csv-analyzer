#include <iostream>
#include <fstream>
#include <map>
#include <stdexcept>
#include "csv/csv.hpp"
#include "csv/stats.hpp"



int main(int argc, char* argv[]){
    if (argc < 3){
        std::cerr << "Usage: " << argv[0] << " <csv_file>" << " <feature>" << std::endl;
        return 1;
    }

    std::string feature = argv[2];

    Data sensor_data;
    try {
        sensor_data = load_csv(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (sensor_data.rows.empty()) {
        std::cerr << "CSV is empty." << std::endl;
        return 1;
    }

    std::size_t sensor_col = sensor_data.header.size();
    std::size_t feature_col = sensor_data.header.size();

    for (std::size_t col = 0; col < sensor_data.header.size(); ++col) {
        if (sensor_data.header[col] == "sensor_id") {
            sensor_col = col;
        }
        if (sensor_data.header[col] == feature) {
            feature_col = col;
        }
    }

    if (sensor_col == sensor_data.header.size()) {
        std::cerr << "No sensor_id column found in the data.\n";
        return 1;
    }

    if (feature_col == sensor_data.header.size()) {
        std::cerr << "No feature named '" << feature << "' in the data.\n";
        return 1;
    }

    if (feature == "sensor_id") {
        std::cerr << "Feature 'sensor_id' is not numeric and cannot be analyzed.\n";
        return 1;
    }

    std::map<std::string, std::vector<double>> grouped_data;

    for (const auto& row : sensor_data.rows) {
        if (row.size() <= sensor_col || row.size() <= feature_col) {
            continue;
        }

        std::string sensor_id = row[sensor_col];
        const std::string& value_str = row[feature_col];

        if (!is_numeric(value_str)) {
            continue;
        }

        grouped_data[sensor_id].push_back(std::stod(value_str));
    }

    for (const auto& [sensor_id, values] : grouped_data) {
        std::cout << sensor_id << "\n";

        std::size_t count = get_count(values);
        double sum = calculate_sum(values);
        double mean = calculate_mean(sum, count);
        double stddev = calculate_stddev(values, mean);

        std::cout << "  Count: " << count << "\n";
        std::cout << "  Min: " << get_min(values) << "\n";
        std::cout << "  Max: " << get_max(values) << "\n";
        std::cout << "  Sum: " << sum << "\n";
        std::cout << "  Mean: " << mean << "\n";
        std::cout << "  Standard Deviation: " << stddev << "\n";
    }
    return 0;
}