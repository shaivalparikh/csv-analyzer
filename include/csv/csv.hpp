#pragma once
#include <iosfwd>
#include <optional>
#include <string>
#include <vector>


struct Data {
    std::vector<std::string> header;
    std::vector<std::vector<std::string>> rows;
};

Data parse_csv(std::istream& input);

Data load_csv(const std::string& filename);

std::optional<std::size_t> find_column(const Data& d, const std::string& name);
