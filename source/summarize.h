#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>


class table_parser
{
private:
    std::vector<std::string> _rows;
    std::vector<std::string> _cols;
    std::vector<std::vector<double>> _cells;

public:
    std::size_t rows() const { return _rows.size(); }
    const std::string& row(std::size_t row) const { return _rows[row]; }
    std::size_t cols() const { return _cols.size(); }
    const std::string& col(std::size_t col) const { return _cols[col]; }
    double at(std::size_t row, std::size_t col) const { return _cells[row][col]; }

public:
    table_parser(const std::string& raw, const std::string& col_sep, const std::string& row_sep);

public:
    double row_avg(std::size_t row) const;
    double col_avg(std::size_t col) const;
};

class table_summary
{
private:
    typedef std::vector<double> data_points;
    typedef std::unique_ptr<data_points> data_points_ptr;
    typedef std::unordered_map<std::string, data_points_ptr> data_map;

private:
    data_map _data;

public:
    table_summary();

public:
    void add(const table_parser& table);
    void dump(std::ostream& out);
};
