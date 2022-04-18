#include "pch.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "Components.h"

void EntityFactory::init()
{
    // Open and parse JSON file
    std::ifstream ifs{ "EntityBlueprints.json" };
    if (!ifs.is_open())
    {
        std::cerr << "Could not open JSON file for parsing!\n";
    }
    rapidjson::IStreamWrapper isw{ ifs };

    document.ParseStream(isw);
    assert(document.IsObject());

    // Register Components
    c_factory.registerType("Position", new ComponentBuilder<PositionComponent>());
    c_factory.registerType("Render", new ComponentBuilder<RenderComponent>());
    c_factory.registerType("Physics", new ComponentBuilder<PhysicsComponent>());
    c_factory.registerType("Health", new ComponentBuilder<HealthComponent>());
    c_factory.registerType("Time", new ComponentBuilder<TimeComponent>());
    c_factory.registerType("PlayerAI", new ComponentBuilder<PlayerAIComponent>());
    c_factory.registerType("AI", new ComponentBuilder<AIComponent>());
}

std::map<std::string, Component*> EntityFactory::construct(std::string name)
{
    std::map<std::string, Component*> comp_map;
    rapidjson::Value* found_ent = nullptr;

    for (auto& e : document["Entities"].GetArray())
    {
        auto ent = e.GetObject();

        if (ent["Name"].GetString() == name)
        {
            found_ent = &e;
            break;
        }
    }

    if (found_ent)
    {
        auto obj = found_ent->GetObject();

        for (auto& c : obj["Components"].GetArray())
        {
            auto obj = c.GetObject();

            std::string comp_name = obj["Name"].GetString();

            Parameters params;

            for (auto& data : obj["Data"].GetObject())
            {
                if (data.value.IsString())
                {
                    params.add<std::string>(data.value.GetString());
                }
                else if (data.value.IsInt())
                {
                    params.add<int>(data.value.GetInt());
                }
                else if (data.value.IsBool())
                {
                    params.add<bool>(data.value.GetBool());
                }
            }
            comp_map[comp_name] = c_factory.build(comp_name, params);
        }

        if (obj.HasMember("Inherits"))
        {
            for (auto& e : obj["Inherits"].GetArray())
            {
                comp_map.merge(construct(e.GetString()));
            }
        }
    }
    else
    {
        std::cout << "Entity " << name << " does not exist in the file!" << std::endl;
    }

    return comp_map;
}

std::unique_ptr<Entity> EntityFactory::build(std::string entity_name)
{
    auto ent = std::make_unique<Entity>(entity_name);

    if (prototypes.count(entity_name))
    {
        *ent = *(prototypes[entity_name]);
    }
    else
    {
        auto components = construct(entity_name);

        for (auto& c : components)
        {
            ent->addComponent(c.second);
        }

        prototypes.insert({ entity_name, std::move(std::make_unique<Entity>(*ent)) } );
    }

    return std::move(ent);
}

std::unique_ptr<Entity> EntityFactory::build(std::string entity_name, sf::Vector2i pos)
{
    auto ent = build(entity_name);

    auto pos_comp = ent->getComponent<PositionComponent>();

    if (pos_comp)
    {
        pos_comp->position.x = pos.x * gl::TILE_SIZE;
        pos_comp->position.y = pos.y * gl::TILE_SIZE;
    }

    return std::move(ent);
}
