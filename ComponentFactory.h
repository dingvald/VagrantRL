#pragma once
#include "Components.h"
#include "Parameters.h"
#include <cassert>

class BaseBuilder
{
public:
    virtual Component* build(Parameters params) = 0;
};

template<typename T>
class ComponentBuilder : public BaseBuilder
{
public:
    Component* build(Parameters params) override
    {
        return new T(params);
    }
};

class ComponentFactory
{
public:
    ~ComponentFactory()
    {
        for (auto t : type_map)
        {
            delete t.second;
        }
    }

    void registerType(std::string name, BaseBuilder* builder)
    {
        assert(!type_map.count(name));
        type_map.insert({ name, builder });
    }

    Component* build(std::string name, Parameters params)
    {
        assert(type_map.count(name));
        return type_map.at(name)->build(params);
    }

private:

    std::map<std::string, BaseBuilder*> type_map;
};

