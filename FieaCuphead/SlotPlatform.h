#pragma once
#include "DamageDealer.h"

namespace Fiea::GameEngine
{
	class SlotPlatform : public DamageDealer
	{
		RTTI_DECLARATIONS(SlotPlatform, DamageDealer);
	protected:
		SlotPlatform(IdType id);

	public:
		SlotPlatform();
		SlotPlatform(const SlotPlatform& other);
		virtual ~SlotPlatform() override = default;

		// Cloneable interface
		[[nodiscard]] virtual SlotPlatform* clone() const override;

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

		float _xVel{ 0.0f };
		bool playerOverlap{ false };

		GameObject* cuphead;
	};
}