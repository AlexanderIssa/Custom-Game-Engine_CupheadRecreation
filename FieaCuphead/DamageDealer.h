#pragma once
#include "CombatAction.h"
#include "EventDispatcher.h"

namespace Fiea::GameEngine
{
	class Context;

	// Collision event passes information about the collision to associated bodies
	class OnDamageDealtEvent final : public IEvent
	{
		RTTI_DECLARATIONS(OnDamageDealtEvent, IEvent);

	public:
		OnDamageDealtEvent() = default;
		virtual ~OnDamageDealtEvent() = default;

		GameObject* damagingObject;
	};

	class DamageDealer : public CombatAction
	{
		RTTI_DECLARATIONS(DamageDealer, CombatAction);
	protected:
		DamageDealer(IdType id);
		DamageDealer(int damage, IdType id);

	public:
		DamageDealer();
		DamageDealer(const DamageDealer& other);
		virtual ~DamageDealer() override;

		// Cloneable interface
		[[nodiscard]] virtual DamageDealer* clone() const override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI OverridesS
		virtual std::string ToString() const override;

		void onEnable() override {};
		void onDisable() override {};
		virtual void update(const GameTime& time) override;

	protected:
		virtual void OnCollisionPlayer(GameObject* hit, AbstractCollider* collider);
		virtual void OnCollisionEnemy(GameObject* hit, AbstractCollider* collider);
		virtual void OnCollisionProjectile(GameObject* hit, AbstractCollider* collider);

		virtual void dealDamage(GameObject* hit);

		int _damage = 1;

		int canDamagePlayer{ 1 };
		int canDamageEnemy{ 1 };
		int canDamageProjectile{ 1 };

		GameObject* cuphead;
		Context* cupheadContext;
		bool playerAbove{ false };

		OnDamageDealtEvent* onDamageDealtEvent;
	};
}