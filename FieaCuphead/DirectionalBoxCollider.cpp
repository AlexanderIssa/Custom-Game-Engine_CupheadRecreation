//#include "DirectionalBoxCollider.h"
//
//namespace Fiea::GameEngine
//{
//	RTTI_DEFINITIONS(DirectionalBoxCollider);
//
//	DirectionalBoxCollider::DirectionalBoxCollider(IdType id) : BoxCollider(id)
//	{
//	}
//
//	DirectionalBoxCollider::DirectionalBoxCollider() : BoxCollider(TypeIdClass())
//	{
//	}
//
//	DirectionalBoxCollider::DirectionalBoxCollider(const DirectionalBoxCollider& other) : BoxCollider(other)
//	{
//	}
//
//	DirectionalBoxCollider* DirectionalBoxCollider::clone() const
//	{
//		return new DirectionalBoxCollider(*this);
//	}
//
//	void DirectionalBoxCollider::onEnable()
//	{
//	}
//
//	void DirectionalBoxCollider::onDisable()
//	{
//	}
//
//	void DirectionalBoxCollider::update(const GameTime& time)
//	{
//		BoxCollider::update(time);
//	}
//
//	std::vector<Signature> DirectionalBoxCollider::prescribedAttributes()
//	{
//		return std::vector<Signature>();
//	}
//
//	bool DirectionalBoxCollider::registerSignatures()
//	{
//		static bool registered = []()
//		{
//			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
//			return true;
//		}();
//		return registered;
//	}
//
//	std::string DirectionalBoxCollider::ToString() const
//	{
//		return std::string("DirectionalBoxCollider");
//	}
//
//	AABB DirectionalBoxCollider::getAABB() const
//	{
//		return _aabb;
//	}
//
//	bool DirectionalBoxCollider::processCollision(AbstractCollider* other, const GameTime& time)
//	{
//		if (!getParentGameObject()->enabled)
//		{
//			return false;
//		}
//		if (!other->getParentGameObject()->enabled)
//		{
//			return true;
//		}
//
//		BoxCollider* otherCollider = other->As<BoxCollider>();
//		if (otherCollider == nullptr) { return true; }
//
//		const AABB& otherAABB = otherCollider->getAABB();
//		const AABB& myAABB = _aabb;
//
//		// Check if the aabbs intersect, boxes can't rotate so shouldn't have to account for angle
//		if (_aabb.minx > otherAABB.maxx || _aabb.maxx < otherAABB.minx ||
//			_aabb.miny > otherAABB.maxy || _aabb.maxy < otherAABB.miny)
//		{
//			return true;
//		}
//
//		// Enemies shouldn't collide with one another so we can ignore this
//		if (this->getColliderTag() == ColliderTag::enemy && other->getColliderTag() == enemy)
//		{
//			return true;
//		}
//
//		// If there is an intersection create a new event
//		cEvent->body1 = getParentGameObject();
//		cEvent->body1Col = this;
//		cEvent->body2 = other->getParentGameObject();
//		cEvent->body2Col = other;
//		EventDispatcher::enqueue(cEvent, time, 0.0f);
//
//		return true;
//	}
//
//
//}
