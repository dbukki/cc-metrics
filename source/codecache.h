#pragma once
#include <string>
#include <unordered_map>


class code_cache
{
private:
    std::string _dir;
    std::unordered_map<std::size_t, std::string> _cache;

public:
    code_cache(const std::string& dir);

public:
    const std::string& get(std::size_t id);
};

class index_pair
{
public:
    struct hash
    {
        std::size_t operator()(const index_pair& p) const
        {
            std::hash<std::size_t> h;
            return h(p._left) ^ h(p._right);
        }
    };

private:
    std::size_t _left;
    std::size_t _right;

public:
    std::size_t left() const { return _left; }
    std::size_t right() const { return _right; }

public:
    index_pair(std::size_t left, std::size_t right) :
        _left(left), _right(right) {}

public:
    bool operator==(const index_pair& other) const
    { return _left == other._left && _right == other._right; }
};

class code_pair_cache
{
private:
    std::unordered_map<index_pair, std::size_t, index_pair::hash> _cache;
    code_cache _left_cache;
    code_cache _right_cache;

public:
    code_pair_cache(const std::string& left, const std::string& right);

public:
    std::size_t get(std::size_t left, std::size_t right);
};
