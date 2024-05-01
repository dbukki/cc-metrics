#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <iostream>


class accumulator
{
private:
    double _min;
    double _max;
    double _sum;
    std::size_t _count;

public:
    double min() const { return _min; }
    double max() const { return _max; }
    double sum() const { return _sum; }
    std::size_t count() const { return _count; }
    double average() const { return _sum / _count; }

public:
    accumulator();

public:
    void add(double d);
};

class table_column
{
public:
    struct cell_data
    {
        double correlation;
        std::size_t common_total;
        double rejection_rate;
        double matchup_rate;
    };

private:
    std::string _name;
    std::unordered_map<std::string, cell_data> _records;

public:
    const std::string& name() const { return _name; }

public:
    table_column(const std::string& name);

public:
    void add(const std::string& row, const cell_data& value);
    bool get(const std::string& row, cell_data& cell) const;
};

typedef std::unique_ptr<table_column> table_column_ptr;

class table_accumulator
{
private:
    std::vector<table_column_ptr> _cols;
    std::unordered_set<std::string> _rows;

public:
    table_accumulator();

public:
    void next_column(const std::string& col);
    void add_value(const std::string& row, const table_column::cell_data& value);

public:
    void dump(std::ostream& out_table, std::ostream& out_acc,
        const std::string& col_sep,
        const std::string& row_sep,
        const std::string& missing) const;
};
