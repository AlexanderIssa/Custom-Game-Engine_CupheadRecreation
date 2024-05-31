#include "SlotPlatform.h"
#include "TypeManager.h"
#include "GameObject.h"
#include "GravityAction.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(SlotPlatform);

	SlotPlatform::SlotPlatform(IdType id) : DamageDealer(id)
	{
	}

	SlotPlatform::SlotPlatform() : DamageDealer(TypeIdClass())
	{
	}

	SlotPlatform::SlotPlatform(const SlotPlatform& other) : DamageDealer(other)
	{
	}

	SlotPlatform* SlotPlatform::clone() const
	{
		return new SlotPlatform(*this);
	}

	std::vector<Signature> SlotPlatform::prescribedAttributes()
	{
		return std::vector<Signature> {
			{ "float_xVel", Datum::DataType::FLOAT, 1, offsetof(SlotPlatform, _xVel)}
		};
	}

	bool SlotPlatform::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string SlotPlatform::ToString() const
	{
		return std::string("SlotPlatform");
	}

	void SlotPlatform::update(const GameTime& time)
	{
		if (playerOverlap)
		{
			if (cuphead == nullptr) { return; }

			// Is the player above?
			if (cuphead->transform.position.y - 20.0f > getParentGameObject()->transform.position.y)
			{
				GravityAction* gravAction = cuphead->getAction("GravityAction")->As<GravityAction>();
				if (gravAction)
				{
					gravAction->clearForce();
				}

				// Move cuphead
				if (cuphead->transform.position.x > 0.0f)
				{
					cuphead->transform.position.x += _xVel * time.Frame() * 0.001f;
				}
			}
		}
	}

	void SlotPlatform::OnCollisionPlayer(GameObject* hit, AbstractCollider* collider)
	{
		playerOverlap = true;

		if (cuphead == nullptr)
		{
			cuphead = hit;
		}
	}

	void SlotPlatform::OnCollisionEnemy(GameObject* hit, AbstractCollider* collider)
	{
	}

	void SlotPlatform::OnCollisionProjectile(GameObject* hit, AbstractCollider* collider)
	{
	}

	void SlotPlatform::dealDamage(GameObject* hit)
	{
	}



}
