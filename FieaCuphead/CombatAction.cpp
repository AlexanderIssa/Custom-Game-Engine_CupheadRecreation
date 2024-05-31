#include "CombatAction.h"
#include "TypeManager.h"
#include "Collision.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(CombatAction);

	CombatAction::CombatAction(IdType id) : Action(id)
	{
		auto boundCollisionFunc = BIND_FUNC(this, CombatAction::OnCollidedEvent);
		collidedCallback = new EventCallback(boundCollisionFunc);
		EventDispatcher::addListener<CollisionEvent>(collidedCallback);
	}

	CombatAction::CombatAction() : Action(TypeIdClass())
	{
		auto boundCollisionFunc = BIND_FUNC(this, CombatAction::OnCollidedEvent);
		collidedCallback = new EventCallback(boundCollisionFunc);
		EventDispatcher::addListener<CollisionEvent>(collidedCallback);
	}

	/**
	 * @brief Combat Action Copy Constructor
	 * @param other 
	*/
	CombatAction::CombatAction(const CombatAction& other) : Action(other)
	{
		auto boundCollisionFunc = BIND_FUNC(this, CombatAction::OnCollidedEvent);
		collidedCallback = new EventCallback(boundCollisionFunc);
		EventDispatcher::addListener<CollisionEvent>(collidedCallback);
	}

	CombatAction::~CombatAction()
	{
		delete collidedCallback;
	}

	CombatAction* CombatAction::clone() const
	{
		return new CombatAction(*this);
	}

	std::vector<Signature> CombatAction::prescribedAttributes()
	{
		return std::vector<Signature>();
	}

	bool CombatAction::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string CombatAction::ToString() const
	{
		return std::string("CombatAction");
	}

	void CombatAction::OnCollidedEvent(IEvent* eventData)
	{
		if (eventData == nullptr)
		{
			throw std::runtime_error("eventData cannot be null.");
		}

		CollisionEvent* data = eventData->As<CollisionEvent>();
		if (data == nullptr)
		{
			throw std::runtime_error("Invalid event data type.");
		}

		CombatAction* temp = const_cast<CombatAction*>(this);

		GameObject* otherObj = nullptr;
		AbstractCollider* otherCollider = nullptr;
		if (temp->getParentGameObject() == data->body1)
		{
			otherObj = data->body2;
			otherCollider = data->body2Col;
		}
		else if (temp->getParentGameObject() == data->body2)
		{
			otherObj = data->body1;
			otherCollider = data->body1Col;
		}

		if (otherObj == nullptr) { return; }
		HandleCollision(otherObj, otherCollider);
	}

	void CombatAction::OnCollisionPlayer(GameObject* hit, AbstractCollider* collider)
	{
	}

	void CombatAction::OnCollisionEnemy(GameObject* hit, AbstractCollider* collider)
	{
	}

	void CombatAction::OnCollisionProjectile(GameObject* hit, AbstractCollider* collider)
	{
	}

	void CombatAction::HandleCollision(GameObject* hit, AbstractCollider* otherCollider)
	{
		switch (otherCollider->getColliderTag())
		{
		case ColliderTag::player:
			OnCollisionPlayer(hit, otherCollider);
			break;
		case ColliderTag::projectile:
			OnCollisionProjectile(hit, otherCollider);
			break;
		case ColliderTag::enemy:
			OnCollisionEnemy(hit, otherCollider);
			break;
		default:
			break;
		}
	}
}
