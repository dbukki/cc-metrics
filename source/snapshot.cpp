#include "config.h"
#include "snapshot.h"
#include "codecache.h"

#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;


snapshot::snapshot(const string& dir) :
    _metrics()
{
    directory_iterator it(dir);
    for (const path& p : it)
    {
        if (is_regular_file(p) && p.extension() == REG_EXT)
        {
            _metrics.emplace(p.filename().string(), make_unique<metrics_registry>(p.string()));
        }
    }
}


common_snapshot::common_snapshot(const snapshot& left, const snapshot& right) :
    _metrics()
{
    for (const auto& l : left.metrics())
    {
        auto r = right.metrics().find(l.first);
        if (r != right.metrics().end())
            _metrics.emplace(l.first, make_unique<common_metrics>(*l.second, *r->second));
    }
}


snapshot_delta::snapshot_delta(const string& old_dir, const string& new_dir) :
    _metrics()
{
    path old_code_dir = old_dir;
    old_code_dir /= CODE_DIR;
    path new_code_dir = new_dir;
    new_code_dir /= CODE_DIR;

    snapshot old_snap(old_dir);
    snapshot new_snap(new_dir);
    common_snapshot common(old_snap, new_snap);

    code_pair_cache differ(old_code_dir.string(), new_code_dir.string());
    for (const auto& metrics : common.metrics())
    {
        metrics_delta& md = *_metrics.emplace(metrics.first, make_unique<metrics_delta>()).first->second;
        for (const auto& metric : metrics.second->values())
        {
            metrics_delta::data d;
            d.value = metric.second.right.value - metric.second.left.value;
            d.code = differ.get(metric.second.left.index, metric.second.right.index);
            md.add(metric.first, d);
        }
    }
}

void snapshot_delta::dump(ostream& out) const
{
    for (const auto& metrics : _metrics)
        out << metrics.first << ":\t" << metrics.second->corr() << '\n';
}
