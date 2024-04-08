#include "parse.h"
#include "accumulate.h"

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


enum main_error
{
    all_ok,
    not_enough,
    dir_not_found,
    undef_ext,
};

main_error error(main_error e)
{
    switch (e)
    {
        case all_ok: break;
        case not_enough: cerr << "Not enough command-line arguments." << endl; break;
        case dir_not_found: cerr << "The specified directory does not exist." << endl; break;
        case undef_ext: cerr << "No filter extension was defined." << endl; break;
    }
    return e;
}

int main(int argc, const char** argv)
{
    if (argc < 3)
        return error(not_enough);

    path dir = argv[1];
    if (dir.empty() || !exists(dir))
        return error(dir_not_found);

    path ext(argv[2]);
    if (ext.empty())
        return error(undef_ext);

    table_accumulator table;
    directory_iterator it(dir);
    for (const directory_entry& entry : it)
    {
        path p = entry.path();
        if (is_regular_file(p) && p.extension() == ext)
        {
            std::ifstream ifs(p.c_str());
            if (ifs)
            {
                table.next_column(p.filename().replace_extension().string());
                string key, value;
                while (getline(get_until(ifs, key, '\t'), value))
                    table.add_value(key, value);
            }
        }
    }
    table.dump(cout);

    return error(all_ok);
}
