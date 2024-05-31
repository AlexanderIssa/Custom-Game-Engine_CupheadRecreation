#pragma once
#include "Action.h"
#include "EventDispatcher.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{
	class AbstractCollider;
	class GameObject;

	class CombatAction : public Action
	{
		RTTI_DECLARATIONS(CombatAction, Action);
	protected:
		CombatAction(IdType id);

	public:
		CombatAction();
		CombatAction(const CombatAction& other);
		virtual ~CombatAction() override;

		// Cloneable interface
		[[nodiscard]] virtual CombatAction* clone() const override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		virtual void OnCollidedEvent(IEvent* eventData);

		void onEnable() override {};
		void onDisable() override {};
		virtual void update(const GameTime& time) override {};

	protected:
		virtual void OnCollisionPlayer(GameObject* hit, AbstractCollider* collider);
		virtual void OnCollisionEnemy(GameObject* hit, AbstractCollider* collider);
		virtual void OnCollisionProjectile(GameObject* hit, AbstractCollider* collider);

		void HandleCollision(GameObject* hit, AbstractCollider* otherCollider);

	private:
		EventCallback* collidedCallback;
	};
}