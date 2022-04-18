#pragma once
#include <any>
#include <cassert>

struct Parameters
{
    void clear()
    {
        any_q = {};
        count = 0;
    }

    template<typename T>
    void add(T param)
    {
        any_q[std::type_index(typeid(param))].push(param);
        ++count;
    }

    template<typename T>
    T get()
    {
        assert(!any_q[typeid(T)].empty());
        auto ret = any_q[typeid(T)].front();
        any_q[typeid(T)].pop();
        --count;
        return std::any_cast<T>(ret);
    }

    int getCount()
    {
        return count;
    }

private:
    int count = 0;
    std::unordered_map<std::type_index, std::queue<std::any>> any_q;
};



