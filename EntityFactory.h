#pragma once
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "ComponentFactory.h"


class EntityFactory
{
public:
	void init();
	std::unique_ptr<Entity> build(std::string entity_name);
	std::unique_ptr<Entity> build(std::string entity_name, sf::Vector2i pos);

private:
	ComponentFactory c_factory;
	rapidjson::Document document;
	std::map<std::string, std::unique_ptr<Entity>> prototypes;

	std::map<std::string, Component* > construct(std::string name);
};

