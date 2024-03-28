#include "snapshot.h"

#include <iostream>

using namespace std;

//#define DEBUG_TEST
#ifdef DEBUG_TEST
#include "compare.h"
#include "parse.h"
#endif

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
#ifdef DEBUG_TEST
    string l = get_all("/home/dani/Artifacts/Extract/cjson-1.7.0/Code/258.ccx");
    string r = get_all("/home/dani/Artifacts/Extract/cjson-1.7.9/Code/118.ccx");
    size_t c = lcs(l, r);
    cout << "|L|: " << l.length() << endl;
    cout << "|R|: " << r.length() << endl;
    cout << "LCS: " << c << endl;
    cout << "DIF: " << (l.length() + r.length() - 2 * c) << endl;
    return 0;
#endif

#ifdef DEBUG_TEST
    string old_dir = "/home/dani/Artifacts/Extract/tiny0";
    string new_dir = "/home/dani/Artifacts/Extract/tiny1";
#else
    if (argc < 3)
        return error(not_enough);

    string old_dir(argv[1]);
    if (old_dir.empty())
        return error(undef_old);

    string new_dir(argv[2]);
    if (new_dir.empty())
        return error(undef_new);
#endif

    snapshot_delta delta(old_dir, new_dir);
    delta.dump(cout);

    return error(all_ok);
}
