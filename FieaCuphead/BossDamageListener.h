#pragma once
#include "Action.h"
#include "EventDispatcher.h"

namespace Fiea::GameEngine
{
	class BossDamageListener : public Action
	{
		RTTI_DECLARATIONS(BossDamageListener, Action);
	protected:
		BossDamageListener(IdType type);

	public:
		BossDamageListener();
		virtual ~BossDamageListener();

		// Cloneable interface
		[[nodiscard]] virtual BossDamageListener* clone() const override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		virtual void OnDamageTaken(IEvent* eventData);

	private:
		EventCallback* damageCallback;
	};
}