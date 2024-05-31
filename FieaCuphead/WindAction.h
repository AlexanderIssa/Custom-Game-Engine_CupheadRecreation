#pragma once
#include "CombatAction.h"

namespace Fiea::GameEngine
{
	class WindAction : public CombatAction 
	{
		RTTI_DECLARATIONS(WindAction, CombatAction);

	protected:
		WindAction(IdType id);

	public:
		WindAction();
		WindAction(const WindAction& other);
		virtual ~WindAction() = default;

		[[nodiscard]] WindAction* clone() const override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI OverridesS
		virtual std::string ToString() const override;

		void onEnable() override;
		void onDisable() override {};
		virtual void update(const GameTime& time) override;

		void setVelocity(float xVel) { _xVelocity = xVel; }

	protected:
		virtual void OnCollisionPlayer(GameObject* hit, AbstractCollider* collider);
		virtual void OnCollisionEnemy(GameObject* hit, AbstractCollider* collider);
		virtual void OnCollisionProjectile(GameObject* hit, AbstractCollider* collider);

		bool overlapCuphead{ false };
		GameObject* cuphead;

		float _xVelocity{ 15.0f };
	};
}