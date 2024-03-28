#include "metrics.h"
#include "parse.h"

#include <fstream>
#include <cmath>

using namespace std;

metrics_registry::metrics_registry(const string& file) :
    _values(), _rejects()
{
    ifstream ifs(file);
    string name;
    while (get_until(ifs, name, '\t'))
    {
        data d;
        ifs >> d.value >> d.index;
        if (!_values.emplace(name, d).second)
            _rejects.insert(name);
    }

    if (!_rejects.empty())
        cerr << _rejects.size() << " record(s) rejected from " << file << ':' << endl;
    for (const string& name : _rejects)
    {
        _values.erase(name);
        cerr << '\t' << name << endl;
    }
}

common_metrics::common_metrics(const metrics_registry& left, const metrics_registry& right) :
    _values()
{
    for (const auto& l : left.values())
    {
        const auto r = right.values().find(l.first);
        if (r != right.values().end())
            _values.emplace(l.first, data{ l.second, r->second });
    }
}

metrics_delta::metrics_delta() :
    _values()
{

}

bool metrics_delta::add(const string& s, const data& d)
{
    return _values.emplace(s, d).second;
}

double metrics_delta::corr() const
{
    typedef unsigned long long dif_t;

    dif_t  sum_c  = 0;
    dif_t  sum_c2 = 0;
    double sum_m  = 0;
    double sum_m2 = 0;
    double sum_cm = 0;

    for (const auto& value : _values)
    {
        const dif_t  c = value.second.code;
        const double m = value.second.value;

        //if (c != 0 || m != 0)
        //    cout << "c=" << c << ", m=" << m << '\n';

        sum_c  += c;
        sum_c2 += c * c;
        sum_m  += m;
        sum_m2 += m * m;
        sum_cm += c * m;
    }

    const dif_t  count = _values.size();
    const double num   = sum_cm         - sum_c * sum_m;
    const dif_t  den_c = count * sum_c2 - sum_c * sum_c;
    const double den_m = count * sum_m2 - sum_m * sum_m;
    return num / sqrt(den_c * den_m);
}
