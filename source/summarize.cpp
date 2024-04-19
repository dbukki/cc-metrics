#include "summarize.h"
#include "parse.h"

#include <cmath>

using namespace std;


table_parser::table_parser(const string& raw, const string& col_sep, const string& row_sep) :
    _rows(),
    _cols(),
    _cells()
{
    vector<string> rows = split(raw, row_sep);
    size_t er = rows.size();
    while (er > 0)
        if (rows[--er].empty())
            rows.erase(rows.begin() + er);
    _rows.resize(rows.size() - 1);

    vector<string> cols = split(rows[0], col_sep);
    _cols.insert(_cols.end(), next(cols.begin()), cols.end());

    _cells.resize(_rows.size());
    for (size_t r = 0; r < _rows.size(); ++r)
    {
        cols = split(rows[r + 1], col_sep);
        _rows[r] = cols[0];
        _cells[r].resize(_cols.size());
        for (size_t c = 0; c < _cols.size(); ++c)
        {
            double& cell = _cells[r][c];
            const char* begin = cols[c + 1].c_str();
            char* end;
            cell = strtod(begin, &end);
            if (end == begin)
                cell = NAN;
        }
    }
}

double table_parser::row_avg(size_t row) const
{
    size_t count = 0;
    double sum = 0;
    for (size_t c = 0; c < cols(); ++c)
    {
        double d = at(row, c);
        if (!isnan(d))
        {
            ++count;
            sum += d;
        }
    }
    return sum / count;
}

double table_parser::col_avg(size_t col) const
{
    size_t count = 0;
    double sum = 0;
    for (size_t r = 0; r < rows(); ++r)
    {
        double d = at(r, col);
        if (!isnan(d))
        {
            ++count;
            sum += d;
        }
    }
    return sum / count;
}


table_summary::table_summary() :
    _data()
{

}

void table_summary::add(const table_parser& table)
{
    for (size_t r = 0; r < table.rows(); ++r)
    {
        auto it = _data.find(table.row(r));
        if (it == _data.end())
            it = _data.emplace(table.row(r), make_unique<data_points>()).first;
        
        double d = table.row_avg(r);
        if (!isnan(d))
            it->second->push_back(d);
    }
}

void table_summary::dump(ostream& out)
{
    for (const auto& kvp : _data)
    {
        double sum = 0;
        for (double d : *kvp.second)
            sum += d;
        out << kvp.first << '\t' << (sum / kvp.second->size()) << '\n';
    }
}
