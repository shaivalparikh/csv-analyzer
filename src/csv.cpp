#include "csv.hpp"
#include <fstream>
#include <iostream>
#include <sstream>



Data load_csv(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> rows;
    std::vector<std::string> header;
    if (!file.is_open()){
        std::cerr << "Error opening file: " << filename << std::endl;
        return Data{{}, {}};
    }
    std::string line;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            header.push_back(cell);
        }
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        rows.push_back(row);
    }

    return Data{header, rows};
}

void print_data(const Data& data) {
    for (const auto& col : data.header) {
        std::cout << col << " ";
    }
    std::cout << std::endl;

    for (const auto& row : data.rows) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}