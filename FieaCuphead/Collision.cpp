#include "Collision.h"
#include "TypeManager.h"
#include "CollisionManager.h"
#include "EventDispatcher.h"
#include "GameObject.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{
#pragma  region CollisionEvent
	RTTI_DEFINITIONS(CollisionEvent);
	CollisionEvent::CollisionEvent(GameObject* body1, GameObject* body2) : body1(body1), body2(body2)
	{
	}
#pragma endregion CollisionEvent

#pragma region AbstractCollider
	RTTI_DEFINITIONS(AbstractCollider);
	AbstractCollider::AbstractCollider(IdType type) : Action(type)
	{
		CollisionManager::registerCollider(this);
		cEvent = new CollisionEvent();
	}

	AbstractCollider::AbstractCollider(ColliderTag tag, IdType type) : Action(type), _tag(tag)
	{
		CollisionManager::registerCollider(this);
		cEvent = new CollisionEvent();
	}

	AbstractCollider::~AbstractCollider()
	{
		CollisionManager::removeCollider(this);
		delete cEvent;
	}

	AbstractCollider::AbstractCollider(const AbstractCollider& other) : Action(other), _tag(other._tag), _aabb(other._aabb)
	{
		CollisionManager::registerCollider(this);
		cEvent = new CollisionEvent();
	}

	std::vector<Signature> AbstractCollider::prescribedAttributes()
	{
		return std::vector<Signature> {
			{ "int_tag"s, Datum::DataType::INTEGER, 1, offsetof(AbstractCollider, _tag)}
		};
	}

	bool AbstractCollider::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string AbstractCollider::ToString() const
	{
		return std::string("AbstractCollider");
	}
#pragma endregion AbstractCollider

#pragma region BoxCollider
	RTTI_DEFINITIONS(BoxCollider);

	BoxCollider::BoxCollider(IdType type) : AbstractCollider(TypeIdClass())
	{

	}

	BoxCollider::BoxCollider() : AbstractCollider(TypeIdClass())
	{
	}

	/**
	 * @brief BoxColliderConstructor
	 * @param halfHeight half height of box
	 * @param halfWitdth half width of the box
	*/
	BoxCollider::BoxCollider(float halfHeight, float halfWidth) : AbstractCollider(TypeIdClass()), halfHeight(halfHeight), halfWidth(halfWidth)
	{
	}

	/**
	 * @brief BoxColliderConstructor
	 * @param halfHeight 
	 * @param halfWitdth 
	 * @param tag 
	*/
	BoxCollider::BoxCollider(float halfHeight, float halfWidth, ColliderTag tag) : AbstractCollider(tag, TypeIdClass()), halfHeight(halfHeight), halfWidth(halfWidth)
	{
	}

	BoxCollider::BoxCollider(const BoxCollider& other) : AbstractCollider(other), halfHeight(other.halfHeight), halfWidth(other.halfWidth), posOffset(other.posOffset)
	{
	}

	BoxCollider* BoxCollider::clone() const
	{
		return new BoxCollider(halfHeight, halfWidth, _tag);
	}

	void BoxCollider::onEnable()
	{

	}

	void BoxCollider::onDisable()
	{

	}

	/**
	 * @brief updates the aabb of this object
	 * @param time 
	*/
	void BoxCollider::update(const GameTime& time)
	{
		_aabb.minx = posOffset.x + getParentPosition().x - halfWidth;
		_aabb.maxx = posOffset.x + getParentPosition().x + halfWidth;
		_aabb.miny = posOffset.y + getParentPosition().y - halfHeight;
		_aabb.maxy = posOffset.y + getParentPosition().y + halfHeight;
	}

	std::vector<Signature> BoxCollider::prescribedAttributes()
	{
		return std::vector<Signature>
		{
			{ "float_halfHeight"s, Datum::DataType::FLOAT, 1, offsetof(BoxCollider, halfHeight)},
			{ "float_halfWidth"s, Datum::DataType::FLOAT, 1, offsetof(BoxCollider, halfWidth) },
			{ "vec4_posOffset"s, Datum::DataType::VEC4, 1, offsetof(BoxCollider, posOffset) }
		};
	}

	bool BoxCollider::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string BoxCollider::ToString() const
	{
		return std::string("BoxCollider");
	}

	AABB BoxCollider::getAABB() const
	{
		return _aabb;
	}

	bool BoxCollider::processCollision(AbstractCollider* other, const GameTime& time)
	{
		if (!getParentGameObject()->enabled)
		{
			return false; 
		}
		if (!other->getParentGameObject()->enabled) 
		{
			return true; 
		}

		BoxCollider* otherCollider = other->As<BoxCollider>();
		if (otherCollider == nullptr) { return true; }

		const AABB& otherAABB = otherCollider->getAABB();
		const AABB& myAABB = _aabb;

		// Check if the aabbs intersect, boxes can't rotate so shouldn't have to account for angle
		if (_aabb.minx > otherAABB.maxx || _aabb.maxx < otherAABB.minx ||
		_aabb.miny > otherAABB.maxy || _aabb.maxy < otherAABB.miny) 
		{ 
			return true; 
		}

		if (this->getColliderTag() == ColliderTag::projectile)
		{
			int kljh = 1;
		}

		// Enemies shouldn't collide with one another so we can ignore this
		if (this->getColliderTag() == ColliderTag::enemy && other->getColliderTag() == enemy)
		{
			return true;
		}

		// If there is an intersection create a new event
		cEvent->body1 = getParentGameObject();
		cEvent->body1Col = this;
		cEvent->body2 = other->getParentGameObject();
		cEvent->body2Col = other;
		EventDispatcher::enqueue(cEvent, time, 0.0f);

		return true;
	}
#pragma endregion BoxCollider
}
