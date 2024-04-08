#include "snapshot.h"

#include <iostream>

using namespace std;


enum main_error
{
    all_ok,
    not_enough,
    undef_old,
    undef_new,
};

main_error error(main_error e)
{
    switch (e)
    {
        case all_ok: break;
        case not_enough: cerr << "Not enough command-line arguments." << endl; break;
        case undef_old: cerr << "Old directory is not defined." << endl; break;
        case undef_new: cerr << "New directory is not defined." << endl; break;
    }
    return e;
}

int main(int argc, const char** argv)
{
    if (argc < 3)
        return error(not_enough);

    string old_dir(argv[1]);
    if (old_dir.empty())
        return error(undef_old);

    string new_dir(argv[2]);
    if (new_dir.empty())
        return error(undef_new);

    snapshot_delta delta(old_dir, new_dir);
    delta.dump(cout);

    return error(all_ok);
}
