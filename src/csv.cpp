#include "csv/csv.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {
void strip_cr(std::string& s) {
    if (!s.empty() && s.back() == '\r') s.pop_back();
}
}  // namespace

Data parse_csv(std::istream& input) {
    std::vector<std::vector<std::string>> rows;
    std::vector<std::string> header;

    std::string line;
    if (!std::getline(input, line)) {
        throw std::runtime_error("CSV has no header line");
    }
    strip_cr(line);
    if (line.empty()) {
        throw std::runtime_error("CSV header line is blank");
    }

    std::stringstream header_ss(line);
    std::string header_cell;
    while (std::getline(header_ss, header_cell, ',')) {
        header.push_back(header_cell);
    }

    while (std::getline(input, line)) {
        strip_cr(line);
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        if (!line.empty() && line.back() == ',') row.push_back("");
        if (row.size() != header.size()) continue;
        rows.push_back(row);
    }

    return Data{header, rows};
}

Data load_csv(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    if (file.peek(), file.fail()) {
        throw std::runtime_error("Could not read file: " + filename);
    }
    return parse_csv(file);
}

std::optional<std::size_t> find_column(const Data& d, const std::string& name) {
    for (std::size_t col = 0; col < d.header.size(); ++col) {
        if (d.header[col] == name) return col;
    }
    return std::nullopt;
}
