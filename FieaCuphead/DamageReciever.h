#pragma once
#include "DamageReciever.h"
#include "CombatAction.h"
#include "IEvent.h"

namespace Fiea::GameEngine
{
	class OnDamageEvent : public IEvent {
		RTTI_DECLARATIONS(OnDamageEvent, IEvent);

	public:
		OnDamageEvent() = default;
		OnDamageEvent(GameObject* obj);
		virtual ~OnDamageEvent() = default;

		GameObject* hit;
		int damage;
	};

	class OnDeathEvent : public IEvent {
		RTTI_DECLARATIONS(OnDeathEvent, IEvent);
	public:
		GameObject* rottingDeadThing;
	};


	class DamageReceiver : public Action
	{
		RTTI_DECLARATIONS(DamageReceiver, Action);
	protected:
		DamageReceiver(IdType id);

	public:
		DamageReceiver();
		DamageReceiver(const DamageReceiver& other);
		virtual ~DamageReceiver();

		// Cloneable interface
		[[nodiscard]] virtual DamageReceiver* clone() const override;

		virtual void onEnable() override;

		virtual void onDisable() override;

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		bool takeDamage(int damage);
		void reset();

	private:

		void flash();

		int _maxHealth;
		int _currentHealth;

		float _damageCooldown;
		float _damageCooldownTimer{ 2.0f };

		bool canTakeDamage{ true };
		OnDamageEvent* damageEvent;
		OnDeathEvent* ohNoIDead;
	};
}