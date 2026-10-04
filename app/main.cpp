#include <algorithm>
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include "csv/csv.hpp"
#include "csv/stats.hpp"

namespace {

void print_usage(const char* prog) {
    std::cerr << "Usage: " << prog << " <file> [group_col] [agg_col]\n"
              << "  Summarizes every numeric column in <file>.\n"
              << "  With group_col and agg_col, also reports the mean of agg_col per group.\n"
              << "  Either column may be omitted; the first text and numeric columns are used.\n"
              << "  A header-only file is valid and exits 0.\n";
}

void report_missing_column(const std::string& name, const Data& d) {
    std::cerr << std::format("No column '{}'. Available:", name);
    for (std::size_t col = 0; col < d.header.size(); ++col) {
        std::cerr << (col == 0 ? " " : ", ") << d.header[col];
    }
    std::cerr << "\n";
}

std::vector<std::size_t> text_columns(const Data& d, const std::vector<std::size_t>& numeric) {
    std::vector<std::size_t> cols;
    for (std::size_t col = 0; col < d.header.size(); ++col) {
        if (std::find(numeric.begin(), numeric.end(), col) == numeric.end()) {
            cols.push_back(col);
        }
    }
    return cols;
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4) {
        print_usage(argv[0]);
        return 1;
    }

    try {
        const std::string filename = argv[1];
        Data d = load_csv(filename);

        if (d.rows.empty()) {
            std::cout << "Loaded 0 rows\n";
            return 0;
        }

        std::cout << std::format("Loaded {} rows, {} columns from '{}'.\n",
                                 d.rows.size(), d.header.size(), filename);

        const std::vector<std::size_t> num_cols = numeric_columns(d);
        const std::vector<std::size_t> txt_cols = text_columns(d, num_cols);

        for (std::size_t col : num_cols) {
            ColumnStats s = summarize(extract_column(d, col));
            std::cout << std::format(
                "{:<12} count={:<4} min={:.2f}  max={:.2f}  mean={:.2f}  stddev={:.2f}\n",
                d.header[col], s.count, s.min, s.max, s.mean, s.stddev);
        }

        if (!txt_cols.empty()) {
            std::cout << "Text columns (skipped for stats):";
            for (std::size_t col : txt_cols) std::cout << " " << d.header[col];
            std::cout << "\n";
        }

        std::optional<std::size_t> group_col;
        if (argc >= 3) {
            group_col = find_column(d, argv[2]);
            if (!group_col) {
                report_missing_column(argv[2], d);
                return 1;
            }
        } else if (!txt_cols.empty()) {
            group_col = txt_cols.front();
        }

        std::optional<std::size_t> agg_col;
        if (argc >= 4) {
            agg_col = find_column(d, argv[3]);
            if (!agg_col) {
                report_missing_column(argv[3], d);
                return 1;
            }
        } else if (!num_cols.empty()) {
            agg_col = num_cols.front();
        }

        if (!group_col || !agg_col) {
            std::cout << std::format("No group-by: the file has {}.\n",
                                     !group_col ? "no text columns to group by"
                                                : "no numeric columns to average");
            return 0;
        }

        if (!column_is_numeric(d, *agg_col)) {
            std::cerr << std::format("Column '{}' is not numeric, so it cannot be averaged.\n",
                                     d.header[*agg_col]);
            return 1;
        }

        std::cout << std::format("\nGroup by '{}' - mean {}\n",
                                 d.header[*group_col], d.header[*agg_col]);
        for (const auto& [key, values] : group_by(d, *group_col, *agg_col)) {
            ColumnStats s = summarize(values);
            std::cout << std::format("  {:<12} n={:<4} mean={:.2f}\n", key, s.count, s.mean);
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
}
