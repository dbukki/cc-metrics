#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>


class metrics_registry
{
public:
    struct entity_data
    {
        double value;
        std::size_t index;
    };

    typedef std::unordered_map<std::string, entity_data> value_map;
    typedef std::unordered_set<std::string> reject_set;

private:
    value_map _values;
    reject_set _rejects;

public:
    const value_map& values() const { return _values; }
    const reject_set& rejects() const { return _rejects; }

public:
    metrics_registry(const std::string& file);
};

class common_metrics
{
public:
    struct metrics_stats
    {
        std::size_t accepted;
        std::size_t rejected;
    };

    struct match_data
    {
        metrics_registry::entity_data left;
        metrics_registry::entity_data right;
    };

    typedef std::unordered_map<std::string, match_data> value_map;

private:
    metrics_stats _left_stats;
    metrics_stats _right_stats;
    value_map _values;

public:
    const metrics_stats& left_stats() const { return _left_stats; }
    const metrics_stats& right_stats() const { return _right_stats; }
    const value_map& values() const { return _values; }

public:
    common_metrics(const metrics_registry& left, const metrics_registry& right);
};

class metrics_delta
{
public:
    struct data_point
    {
        double value;
        std::size_t code;
    };

    typedef std::unordered_map<std::string, data_point> value_map;

private:
    value_map _values;

public:
    const value_map& values() const { return _values; }

public:
    metrics_delta();

public:
    bool add(const std::string& s, const data_point& dp);
    double corr() const;
};
