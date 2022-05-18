#pragma once
#include "Components.h"
#include "SFMLSerialization.h"


namespace cereal
{

	template<class Archive>
	void serialize(Archive& archive, TimeComponent& comp)
	{
		archive(comp.speed, comp.built_up_speed);
	}

	template<class Archive>
	void serialize(Archive& archive, PositionComponent& comp)
	{
		archive(comp.layer, comp.position);
	}

	template<class Archive>
	void serialize(Archive& archive, MotionComponent& comp)
	{
		archive(comp.direction);
	}

	template<class Archive>
	void serialize(Archive& archive, RenderComponent& comp)
	{
		archive(comp.sprite_id, comp.color);
	}

	template<class Archive>
	void serialize(Archive& archive, HealthComponent& comp)
	{
		archive(comp.health, comp.max_health);
	}

	template<class Archive>
	void serialize(Archive& archive, PhysicsComponent& comp)
	{
		archive(comp.weight, comp.isBlocking);
	}

	template<class Archive>
	void serialize(Archive& archive, MyTurnComponent& comp)
	{
		
	}
	
	template<class Archive>
	void serialize(Archive& archive, PlayerAIComponent& comp)
	{
		
	}

	template<class Archive>
	void serialize(Archive& archive, AIComponent& comp)
	{
		
	}

	template<class Archive>
	void save(Archive& archive, FactionComponent const& comp)
	{
		archive(static_cast<int>(comp.faction));
	}
	template<class Archive>
	void load(Archive& archive, FactionComponent & comp)
	{
		archive(static_cast<Faction>(comp.faction));
	}

	template<class Archive>
	void serialize(Archive& archive, CameraFocusComponent& comp)
	{
		
	}

	template<class Archive>
	void serialize(Archive& archive, OnScreenComponent& comp)
	{
		
	}
}
// namespace cereal