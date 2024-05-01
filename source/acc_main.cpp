#include "parse.h"
#include "accumulate.h"

#include <sstream>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


enum main_error
{
    all_ok,
    not_enough,
    no_col_file,
    no_table_file,
    no_acc_file,
};

main_error error(main_error e)
{
    switch (e)
    {
        case all_ok: break;
        case not_enough: cerr << "Not enough command-line arguments." << endl; break;
        case no_col_file: cerr << "The specified column registry file does not exist." << endl; break;
        case no_table_file: cerr << "No table file specified." << endl; break;
        case no_acc_file: cerr << "No accumulation file specified." << endl; break;
    }
    return e;
}

int main(int argc, const char** argv)
{
    if (argc < 4)
        return error(not_enough);

    path colfile = argv[1];
    if (colfile.empty() || !exists(colfile))
        return error(no_col_file);

    path tablefile = argv[2];
    if (tablefile.empty())
        return error(no_table_file);

    path accfile = argv[3];
    if (accfile.empty())
        return error(no_acc_file);

    string col_sep = argc >= 5 ? unescape(argv[4]) : "\t";
    string row_sep = argc >= 6 ? unescape(argv[5]) : "\n";

    table_accumulator table;
    std::ifstream ifs_cols(colfile.c_str());
    string line;
    while (getline(ifs_cols, line))
    {
        path p = line;
        if (exists(p) && is_regular_file(p))
        {
            std::ifstream ifs_col(p.c_str());
            if (ifs_col)
            {
                table.next_column(p.filename().replace_extension().string());
                string line;
                while (getline(ifs_col, line))
                {
                    std::istringstream iss(line);

                    string metric, corr_str;
                    get_until(iss, metric, '\t');
                    get_until(iss, corr_str, '\t');

                    std::size_t left_accepted, left_rejected, right_accepted, right_rejected, common_total;
                    iss >> left_accepted >> left_rejected >> right_accepted >> right_rejected >> common_total;

                    std::size_t total_accepted = left_accepted + right_accepted;
                    std::size_t total_rejected = left_rejected + right_rejected;

                    table_column::cell_data data;
                    data.correlation = parse_double(corr_str.c_str());
                    data.common_total = common_total;
                    data.rejection_rate = static_cast<double>(total_rejected) / static_cast<double>(total_rejected + total_accepted);
                    data.matchup_rate = static_cast<double>(common_total) / static_cast<double>(left_accepted + right_accepted - common_total);

                    table.add_value(metric, data);
                }
            }
        }
        else if (!p.empty())
            cerr << "Column file not found: " << p.c_str() << endl;
    }

    std::ofstream ofs_table(tablefile.c_str());
    std::ofstream ofs_acc(accfile.c_str());
    table.dump(ofs_table, ofs_acc, col_sep, row_sep, "");

    return error(all_ok);
}
