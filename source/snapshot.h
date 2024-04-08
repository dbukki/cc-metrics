#pragma once
#include "metrics.h"

#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>

class snapshot
{
public:
    typedef std::unordered_map<std::string, std::unique_ptr<metrics_registry>> metrics_map;

private:
    metrics_map _metrics;

public:
    const metrics_map& metrics() const { return _metrics; }

public:
    snapshot(const std::string& dir);
};

class common_snapshot
{
public:
    typedef std::unordered_map<std::string, std::unique_ptr<common_metrics>> metrics_map;

private:
    metrics_map _metrics;

public:
    const metrics_map& metrics() const { return _metrics; }

public:
    common_snapshot(const snapshot& left, const snapshot& right);
};

class snapshot_delta
{
public:
    typedef std::unordered_map<std::string, std::unique_ptr<metrics_delta>> metrics_map;

private:
    metrics_map _metrics;

public:
    const metrics_map& metrics() const { return _metrics; }

public:
    snapshot_delta(const std::string& old_dir, const std::string& new_dir);

public:
    void dump(std::ostream& out) const;
};
