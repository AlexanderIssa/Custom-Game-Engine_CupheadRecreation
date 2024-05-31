#include "DamageDealer.h"
#include "GameObject.h"
#include "DamageReciever.h"
#include "TypeManager.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(OnDamageDealtEvent);
	RTTI_DEFINITIONS(DamageDealer);

	DamageDealer::DamageDealer(IdType id) : CombatAction(id)
	{
		onDamageDealtEvent = new OnDamageDealtEvent();
	}

	DamageDealer::DamageDealer(int damage, IdType id) : CombatAction(id), _damage(damage)
	{
		onDamageDealtEvent = new OnDamageDealtEvent();
	}

	DamageDealer::DamageDealer() : CombatAction(TypeIdClass())
	{
		onDamageDealtEvent = new OnDamageDealtEvent();
	}

	/**
	 * @brief Copy Constructor
	 * @param other 
	*/
	DamageDealer::DamageDealer(const DamageDealer& other) : CombatAction(other), _damage(other._damage), 
		canDamagePlayer(other.canDamagePlayer), canDamageEnemy(other.canDamageEnemy), canDamageProjectile(other.canDamageProjectile)
	{
		onDamageDealtEvent = new OnDamageDealtEvent();
	}

	DamageDealer::~DamageDealer()
	{
		delete onDamageDealtEvent;
	}

	DamageDealer* DamageDealer::clone() const
	{
		return new DamageDealer(*this);
	}

	std::vector<Signature> DamageDealer::prescribedAttributes()
	{
		return std::vector<Signature> {
			{"int_damage", Datum::DataType::INTEGER, 1, offsetof(DamageDealer, _damage)},
			{"int_canDamagePlayer", Datum::DataType::INTEGER, 1, offsetof(DamageDealer, canDamagePlayer)},
			{"int_canDamageEnemy", Datum::DataType::INTEGER, 1, offsetof(DamageDealer, canDamageEnemy)},
			{"int_canDamageProjectile", Datum::DataType::INTEGER, 1, offsetof(DamageDealer, canDamageProjectile)}
		};
	}

	bool DamageDealer::registerSignatures()
	{
		TypeManager::registerType(TypeIdClass(), prescribedAttributes());
		return true;
	}

	std::string DamageDealer::ToString() const
	{
		return std::string("DamageDealer");
	}

	void DamageDealer::update(const GameTime& time)
	{
		if (playerAbove)
		{
			
		}
	}

	void DamageDealer::OnCollisionPlayer(GameObject* hit, AbstractCollider* collider)
	{
		if (canDamagePlayer == 0) { return; }

		if (hit->transform.position.y > getParentGameObject()->transform.position.y)
		{
			playerAbove = true;
			cuphead = hit;
			if (cupheadContext == nullptr)
			{

			}
			return;
		}
		dealDamage(hit);
	}

	void DamageDealer::OnCollisionEnemy(GameObject* hit, AbstractCollider* collider)
	{
		
	}

	void DamageDealer::OnCollisionProjectile(GameObject* hit, AbstractCollider* collider)
	{
	}

	void DamageDealer::dealDamage(GameObject* hit)
	{
		Action* damageReceiverAction = hit->getAction(std::string("DamageReceiver"), std::string("DamageReceiver"));
		if (damageReceiverAction == nullptr) { return; }

		DamageReceiver* damageReceiver = damageReceiverAction->As<DamageReceiver>();
		if (damageReceiver == nullptr) { throw std::runtime_error("Could not convert to damageReceiver"); }

       	if (!damageReceiver->takeDamage(_damage))
		{
			return;
		}

		// Send event
		onDamageDealtEvent->damagingObject = getParentGameObject();
		EventDispatcher::enqueue(onDamageDealtEvent, CreateElapsedTime(1), 0.0f);
	}
}
