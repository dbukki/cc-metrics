#include "codecache.h"
#include "config.h"
#include "parse.h"
#include "compare.h"

#include <sstream>

using namespace std;


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
        it = _cache.emplace(p, d).first;
    }
    return it->second;
}
