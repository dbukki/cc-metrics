#include "parse.h"
#include "summarize.h"

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


enum main_error
{
    all_ok,
    not_enough,
    no_reg_file,
};

main_error error(main_error e)
{
    switch (e)
    {
        case all_ok: break;
        case not_enough: cerr << "Not enough command-line arguments." << endl; break;
        case no_reg_file: cerr << "No valid registry file provided." << endl; break;
    }
    return e;
}

int main(int argc, const char** argv)
{
    if (argc < 2)
        return error(not_enough);

    path regfile = argv[1];
    if (regfile.empty() || !exists(regfile))
        return error(no_reg_file);

    string col_sep = argc >= 3 ? unescape(argv[2]) : "\t";
    string row_sep = argc >= 4 ? unescape(argv[3]) : "\n";

    table_summary sum;
    std::ifstream ifs_reg(regfile.c_str());
    string line;
    while (getline(ifs_reg, line))
    {
        path p = line;
        if (exists(p) && is_regular_file(p))
        {
            string content = get_all(p.string());
            table_parser table(content, col_sep, row_sep);
            sum.add(table);
        }
        else if (!p.empty())
            cerr << "Table file not found: " << p.c_str() << endl;
    }
    sum.dump(cout);

    return error(all_ok);
}
