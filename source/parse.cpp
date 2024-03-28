#include "parse.h"
#include "config.h"
#include "compare.h"

#include <fstream>
#include <sstream>

using namespace std;

istream& get_until(istream& in, string& token, char stop)
{
    token.clear();
    int ch;
    while ((ch = in.get()) >= 0 && ch != stop)
        if (token.length() > 0 || !isspace(ch))
            token += static_cast<char>(ch);
    return in;
}

string get_all(const string& file)
{
    ifstream in(file);
    ostringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

code_cache::code_cache(const string& dir) :
    _dir(dir), _cache()
{

}

const string& code_cache::get(size_t id)
{
    auto it = _cache.find(id);
    if (it == _cache.end())
    {
        ostringstream path;
        path << _dir << id << CODE_EXT;
        it = _cache.emplace(id, get_all(path.str())).first;
    }
    return it->second;
}

code_pair_cache::code_pair_cache(const string& left, const string& right) :
    _cache(), _left_cache(left), _right_cache(right)
{

}

size_t code_pair_cache::get(size_t left, size_t right)
{
    const index_pair p(left, right);
    auto it = _cache.find(p);
    if (it == _cache.end())
    {
        const string l = _left_cache.get(p.left());
        const string r = _right_cache.get(p.right());
        size_t d = dif(l, r);
        /*if (d > l.length() + r.length())
        {
            cerr << "ERROR:\n--- Left (" << p.left() << "): ---\n" << l
                 << "\n--- Right (" << p.right() << "): ---\n" << r
                 << "\n--- Diff = " << d << " ---\n";
        }*/
        it = _cache.emplace(p, d).first;
    }
    return it->second;
}
