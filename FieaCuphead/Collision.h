#pragma once
#include "Action.h"
#include "IEvent.h"
#include "Factory.h"

namespace Fiea::GameEngine
{
	class GameObject;
	class AbstractCollider;

	// Collision event passes information about the collision to associated bodies
	class CollisionEvent final : public IEvent
	{
		RTTI_DECLARATIONS(CollisionEvent, IEvent);

	public:
		CollisionEvent() = default;
		CollisionEvent(GameObject* body1, GameObject* body2);
		virtual ~CollisionEvent() = default;

		GameObject* body1;
		AbstractCollider* body1Col;
		GameObject* body2;
		AbstractCollider* body2Col;
	};

	// Axis aligned bounding box
	struct AABB
	{
		float minx;
		float maxx;
		float miny;
		float maxy;
	};

	enum ColliderTag
	{
		player = 0,
		enemy = 1,
		projectile = 2,
		other = 3
	};

	// Class Abstract Collider
	class AbstractCollider : public Action
	{
		RTTI_DECLARATIONS(AbstractCollider, Action);

	protected:
		AbstractCollider(IdType type);
		AbstractCollider(ColliderTag tag, IdType type);

	public:
		virtual ~AbstractCollider();

		AbstractCollider(const AbstractCollider& other);

		// Cloneable interface
		[[nodiscard]] virtual AbstractCollider* clone() const override = 0;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		virtual AABB getAABB() const = 0;
		virtual bool processCollision(AbstractCollider* other, const GameTime& time) = 0;
		ColliderTag getColliderTag() const { return _tag; }

		void setTag(ColliderTag tag) { _tag = tag; }

	protected:
		AABB _aabb;
		CollisionEvent* cEvent;
		ColliderTag _tag{ ColliderTag::player };
	};

	

	/**
	 * @brief Class BoxCollider stores information to make a box collider
	*/
	class BoxCollider : public AbstractCollider
	{
		RTTI_DECLARATIONS(BoxCollider, AbstractCollider);

	protected:
		BoxCollider(IdType type);
		BoxCollider(float halfHeight, float halfWidth, IdType type);

	public:
		BoxCollider();
		BoxCollider(float halfHeight, float halfWidth);
		BoxCollider(float halfHeight, float halfWidth, ColliderTag tag);
		BoxCollider(const BoxCollider& other);

		// Cloneable interface
		[[nodiscard]] virtual BoxCollider* clone() const override;
		
		virtual ~BoxCollider() override = default;

		virtual void onEnable() override;

		virtual void onDisable() override;

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		virtual AABB getAABB() const override;
		virtual bool processCollision(AbstractCollider* other, const GameTime& time) override;

		// Getters and setters
		float getHalfHeight() const { return halfHeight; }
		float getHalfWidth() const { return halfWidth; }
		glm::vec4 getPosOffset() const { return posOffset; }

		void setHalfHeight(float height) { halfHeight = height; }
		void setHalfWidth(float width) { halfWidth = width; }
		void setPosOffset(glm::vec4 offset) { posOffset = offset; }

	private:
		float halfHeight{ 50.0f };
		float halfWidth{ 50.0f };
		glm::vec4 posOffset;
	};
}