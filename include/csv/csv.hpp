#pragma once
#include <iosfwd>
#include <string>
#include <vector>


struct Data {
    std::vector<std::string> header;
    std::vector<std::vector<std::string>> rows;
};

Data parse_csv(std::istream& input);

Data load_csv(const std::string& filename);

void print_data(const Data& data);
