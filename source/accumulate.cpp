#include "accumulate.h"

#include <cmath>

using namespace std;


accumulator::accumulator() :
    _min(INFINITY),
    _max(-INFINITY),
    _sum(0),
    _count(0)
{

}

void accumulator::add(double d)
{
    if (!isnan(d))
    {
        if (d < _min) _min = d;
        if (d > _max) _max = d;
        _sum += d;
        ++_count;
    }
}


table_column::table_column(const string& name) :
    _name(name),
    _records()
{

}

void table_column::add(const string& row, const cell_data& value)
{
    _records.emplace(row, value);
}

bool table_column::get(const string& row, cell_data& cell) const
{
    auto it = _records.find(row);
    if (it != _records.end())
    {
        cell = it->second;
        return true;
    }
    else return false;
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

void table_accumulator::add_value(const string& row, const table_column::cell_data& value)
{
    _cols.back()->add(row, value);
    _rows.emplace(row);
}

void table_accumulator::dump(ostream& out_table, ostream& out_acc,
    const string& col_sep,
    const string& row_sep,
    const string& missing) const
{
    for (const table_column_ptr& col : _cols)
        out_table << col_sep << col->name();
    out_table << row_sep;

    out_acc << col_sep << "Count";
    out_acc << col_sep << "Avg.Corr";
    out_acc << col_sep << "Avg.Common";
    out_acc << col_sep << "Max.Common";
    out_acc << col_sep << "Avg.Reject";
    out_acc << col_sep << "Avg.Match";
    out_acc << row_sep;

    for (const string& row : _rows)
    {
        accumulator acc_corr;
        accumulator acc_total;
        accumulator acc_reject;
        accumulator acc_match;

        out_table << row;
        out_acc << row;
        for (const table_column_ptr& col : _cols)
        {
            out_table << col_sep;

            table_column::cell_data cell;
            if (col->get(row, cell))
            {
                out_table << cell.correlation;

                acc_corr.add(cell.correlation);
                acc_total.add(cell.common_total);
                acc_reject.add(cell.rejection_rate);
                acc_match.add(cell.matchup_rate);
            }
            else
            {
                out_table << missing;
            }
        }
        out_table << row_sep;

        out_acc << col_sep << acc_corr.count();
        out_acc << col_sep << acc_corr.average();
        out_acc << col_sep << acc_total.average();
        out_acc << col_sep << acc_total.max();
        out_acc << col_sep << acc_reject.average();
        out_acc << col_sep << acc_match.average();
        out_acc << row_sep;
    }
}
