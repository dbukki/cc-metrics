#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <iostream>


class table_column
{
private:
    std::string _name;
    std::unordered_map<std::string, std::string> _records;

public:
    const std::string& name() const { return _name; }

public:
    table_column(const std::string& name);

public:
    void add(const std::string& row, const std::string& value);
    const std::string& get(const std::string& row, const std::string& fallback) const;
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
    void add_value(const std::string& row, const std::string& value);

public:
    void dump(std::ostream& out,
        const std::string& col_sep,
        const std::string& row_sep,
        const std::string& missing) const;
};
