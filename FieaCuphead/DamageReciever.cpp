#include "DamageReciever.h"
#include "TypeManager.h"
#include "EventDispatcher.h"
#include "GameObject.h"
#include "RenderAction.h"

namespace Fiea::GameEngine
{
#pragma region OnDamageEvent
	RTTI_DEFINITIONS(OnDamageEvent);
	RTTI_DEFINITIONS(OnDeathEvent);
#pragma endregion OnDamageEvent
	RTTI_DEFINITIONS(DamageReceiver);

	DamageReceiver::DamageReceiver(IdType id) : Action(TypeIdClass())
	{
		_currentHealth = _maxHealth;
		damageEvent = new OnDamageEvent;
		ohNoIDead = new OnDeathEvent;
	}

	DamageReceiver::DamageReceiver() : Action(TypeIdClass())
	{
		damageEvent = new OnDamageEvent;
		ohNoIDead = new OnDeathEvent;
	}

	DamageReceiver::DamageReceiver(const DamageReceiver& other) : Action(other), _maxHealth(other._maxHealth), _currentHealth(other._currentHealth), _damageCooldown(other._damageCooldown),
		_damageCooldownTimer(0.0f)
	{
		damageEvent = new OnDamageEvent;
		ohNoIDead = new OnDeathEvent;
	}

	DamageReceiver::~DamageReceiver()
	{
		delete damageEvent;
		delete ohNoIDead;
	}

	DamageReceiver* DamageReceiver::clone() const
	{
		return new DamageReceiver(*this);
	}

	void DamageReceiver::onEnable()
	{
		_currentHealth = _maxHealth;
	}

	void DamageReceiver::onDisable()
	{
	}

	void DamageReceiver::update(const GameTime& time)
	{
		if (_damageCooldownTimer > 0.0f)
		{
			_damageCooldownTimer -= time.Frame() * 0.001f;
			if (_damageCooldownTimer <= 0.0f)
			{
				canTakeDamage = true;
			}
		}
	}

	std::vector<Signature> DamageReceiver::prescribedAttributes()
	{
		return std::vector<Signature> {
			{"int_maxHealth", Datum::DataType::INTEGER, 1, offsetof(DamageReceiver, _maxHealth)},
			{"int_currentHealth", Datum::DataType::INTEGER, 1, offsetof(DamageReceiver, _currentHealth)},
			{"float_damageCooldown", Datum::DataType::FLOAT, 1, offsetof(DamageReceiver, _damageCooldown)},
			{"float_damageCooldownTimer", Datum::DataType::FLOAT, 1, offsetof(DamageReceiver, _damageCooldownTimer)},
		};
	}

	bool DamageReceiver::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string DamageReceiver::ToString() const
	{
		return std::string("DamageReceiver");
	}

	/**
	 * @brief Handles taking damage
	 * @param damage amount of damage to take
	*/
	bool DamageReceiver::takeDamage(int damage)
	{
		if (!canTakeDamage) { return false; }

		_currentHealth -= damage;
		if (_currentHealth < 0)
		{
			ohNoIDead->rottingDeadThing = getParentGameObject();
			EventDispatcher::enqueue(ohNoIDead, CreateElapsedTime(0), 0.0f);
		}

		// Send damage event
		damageEvent->hit = getParentGameObject();
		damageEvent->damage = damage;
		EventDispatcher::enqueue(damageEvent, CreateElapsedTime(0), 0.0f);
		
		// Make enemy flash
		flash();

		_damageCooldownTimer = _damageCooldown;
		canTakeDamage = false;
		return true;
	}

	void DamageReceiver::reset()
	{
		_currentHealth = _maxHealth;
		_damageCooldownTimer = 0.0f;
		canTakeDamage = true;
	}

	void DamageReceiver::flash()
	{
		GameObject* parent = getParentGameObject();
		Action* action = parent->getAction(std::string("AnimRenderAction"));
		if (action == nullptr) { return; }

		RenderAction* renderAction = action->As<RenderAction>();
		if (renderAction == nullptr) { throw std::runtime_error("Couldn't find a render action on this object"); }

		renderAction->startFlash();
	}

}
