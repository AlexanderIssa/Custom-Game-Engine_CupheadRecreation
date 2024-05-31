#include "WindAction.h"
#include "TypeManager.h"
#include "State.h"
#include "GameObject.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(WindAction);

	WindAction::WindAction(IdType id) : CombatAction(id)
	{
	}

	WindAction::WindAction() : CombatAction(TypeIdClass())
	{
	}

	WindAction::WindAction(const WindAction& other) : CombatAction(other), _xVelocity(other._xVelocity)
	{
	}

	WindAction* WindAction::clone() const
	{
		return new WindAction(*this);
	}

	std::vector<Signature> WindAction::prescribedAttributes()
	{
		return std::vector<Signature> {
			{"float_xVelocity", Datum::DataType::FLOAT, 1, offsetof(WindAction, _xVelocity)}
		};
	}

	bool WindAction::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string WindAction::ToString() const
	{
		return std::string("WindAction");
	}

	void WindAction::onEnable()
	{
	}

	void WindAction::update(const GameTime& time)
	{
		if (overlapCuphead)
		{
			cuphead->transform.position.x -= _xVelocity * time.Frame() * 0.001f;
			overlapCuphead = false;
		}
	}

	void WindAction::OnCollisionPlayer(GameObject* hit, AbstractCollider* collider)
	{
		Context* cupheadContext = hit->getAction("Context", "Context")->As<Context>();
		if(cupheadContext == nullptr) 
		{
			return;
		}

		overlapCuphead = true;
		cuphead = hit;
	}

	void WindAction::OnCollisionEnemy(GameObject* hit, AbstractCollider* collider)
	{
	}
	void WindAction::OnCollisionProjectile(GameObject* hit, AbstractCollider* collider)
	{
	}
}
