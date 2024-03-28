#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

class metrics_registry
{
public:
    struct data
    {
        double value;
        std::size_t index;
    };

    typedef std::unordered_map<std::string, data> value_map;
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
    struct data
    {
        metrics_registry::data left;
        metrics_registry::data right;
    };

    typedef std::unordered_map<std::string, data> value_map;

private:
    value_map _values;

public:
    const value_map& values() const { return _values; }

public:
    common_metrics(const metrics_registry& left, const metrics_registry& right);
};

class metrics_delta
{
public:
    struct data
    {
        double value;
        std::size_t code;
    };

    typedef std::unordered_map<std::string, data> value_map;

private:
    value_map _values;

public:
    const value_map& values() const { return _values; }

public:
    metrics_delta();

public:
    bool add(const std::string&, const data& d);
    double corr() const;
};
