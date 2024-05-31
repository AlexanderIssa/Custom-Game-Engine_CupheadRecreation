#include "BossDamageListener.h"
#include "DamageReciever.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(BossDamageListener);

	BossDamageListener::BossDamageListener(IdType type) : Action(TypeIdClass())
	{
		auto boundDamageFunc = BIND_FUNC(this, BossDamageListener::OnDamageTaken);
		damageCallback = new EventCallback(boundDamageFunc);
		EventDispatcher::addListener<OnDamageEvent>(damageCallback);

	}

	BossDamageListener::BossDamageListener()
	{
		auto boundDamageFunc = BIND_FUNC(this, BossDamageListener::OnDamageTaken);
		damageCallback = new EventCallback(boundDamageFunc);
		EventDispatcher::addListener<OnDamageEvent>(damageCallback);
	}

	BossDamageListener* BossDamageListener::clone() const
	{
		return nullptr;
	}

	std::vector<Signature> BossDamageListener::prescribedAttributes()
	{
		return std::vector<Signature>();
	}

	bool BossDamageListener::registerSignatures()
	{
		return false;
	}

	std::string BossDamageListener::ToString() const
	{
		return std::string("BossDamageListener");
	}

	void BossDamageListener::OnDamageTaken(IEvent* eventData)
	{
		if (eventData == nullptr)
		{
			throw std::runtime_error("eventData cannot be null.");
		}

		OnDamageEvent* data = eventData->As<OnDamageEvent>();
		if (data == nullptr)
		{
			throw std::runtime_error("Invalid event data type.");
		}

		// If not a boss ignore object ignore the event
		if (getParentGameObject() != data->hit) { return; }

	}
}
