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
            path name = p.filename().replace_extension();
            _metrics.emplace(name.string(), make_unique<metrics_registry>(p.string()));
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
    common_snapshot snap_dif(old_snap, new_snap);

    code_pair_cache differ(old_code_dir.string(), new_code_dir.string());
    for (const auto& metrics : snap_dif.metrics())
    {
        const common_metrics& common = *metrics.second;
        metrics_data& md = *_metrics.emplace(metrics.first, make_unique<metrics_data>()).first->second;
        md.left_stats = common.left_stats();
        md.right_stats = common.right_stats();
        for (const auto& metric : common.values())
        {
            metrics_delta::data_point dp;
            dp.value = /*metric.second.right.value - */metric.second.left.value;
            dp.code = differ.get(metric.second.left.index, metric.second.right.index);
            md.delta.add(metric.first, dp);
        }
    }
}

void snapshot_delta::dump(ostream& out) const
{
    for (const auto& metrics : _metrics)
    {
        const metrics_data& md = *metrics.second;
        out << metrics.first << '\t' << md.delta.corr() << '\t'
            << md.left_stats.accepted << '\t' << md.left_stats.rejected << '\t'
            << md.right_stats.accepted << '\t' << md.right_stats.rejected << '\t'
            << md.delta.values().size() << '\n';
    }
}
