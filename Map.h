#pragma once
class Entity;

class Map
{
public:
	Map(unsigned int num_of_layers, unsigned int width, unsigned int height);

	std::list<Entity*>* getEntitiesAt(unsigned int layer, sf::Vector2u position);
	void placeEntity(Entity* entity, unsigned int layer, sf::Vector2u position);
	void removeEntity(Entity* entity, unsigned int layer, sf::Vector2u position);

private:
	unsigned int width;
	unsigned int height;

	std::vector<std::vector< std::vector< std::list<Entity*> > > > entitiesAt;
};

