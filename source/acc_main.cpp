#include "parse.h"
#include "accumulate.h"

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


enum main_error
{
    all_ok,
    not_enough,
    no_col_file,
};

main_error error(main_error e)
{
    switch (e)
    {
        case all_ok: break;
        case not_enough: cerr << "Not enough command-line arguments." << endl; break;
        case no_col_file: cerr << "The specified column registry file does not exist." << endl; break;
    }
    return e;
}

int main(int argc, const char** argv)
{
    if (argc < 2)
        return error(not_enough);

    path colfile = argv[1];
    if (colfile.empty() || !exists(colfile))
        return error(no_col_file);

    string col_sep = argc >= 3 ? unescape(argv[2]) : "\t";
    string row_sep = argc >= 4 ? unescape(argv[3]) : "\n";

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
                string key, value;
                while (getline(get_until(ifs_col, key, '\t'), value))
                    table.add_value(key, value);
            }
        }
        else if (!p.empty())
            cerr << "Column file not found: " << p.c_str() << endl;
    }
    table.dump(cout, col_sep, row_sep, "");

    return error(all_ok);
}
