#include "accumulate.h"

using namespace std;


table_column::table_column(const string& name) :
    _name(name),
    _records()
{

}

void table_column::add(const string& row, const string& value)
{
    _records.emplace(row, value);
}

const string& table_column::get(const string& row, const string& fallback) const
{
    auto it = _records.find(row);
    return it != _records.end() ? it->second : fallback;
}


table_accumulator::table_accumulator() :
    _cols(),
    _rows()
{

}

void table_accumulator::next_column(const string& col)
{
    _cols.emplace_back(make_unique<table_column>(col));
}

void table_accumulator::add_value(const string& row, const string& value)
{
    _cols.back()->add(row, value);
    _rows.emplace(row);
}

void table_accumulator::dump(ostream& out) const
{
    for (const table_column_ptr& col : _cols)
        out << '\t' << col->name();
    out << '\n';

    const string missing = "";
    for (const string& row : _rows)
    {
        out << row;
        for (const table_column_ptr& col : _cols)
            out << '\t' << col->get(row, missing);
        out << '\n';
    }
}
