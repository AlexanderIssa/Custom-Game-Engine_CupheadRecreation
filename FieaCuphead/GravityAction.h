#pragma once
#include "Action.h"

namespace Fiea::GameEngine
{
	class GravityAction : public Action
	{
		RTTI_DECLARATIONS(GravityAction, Action);

	protected:
		GravityAction(IdType type);

	public:
		GravityAction();
		GravityAction(const GravityAction& other);
		virtual ~GravityAction() = default;

		[[nodiscard]] virtual GravityAction* clone() const override;

		virtual void onEnable() override;

		virtual void onDisable() override;

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		void addImpulse(float impulse) { ySpeed += impulse; }
		void clearForce() { ySpeed = 0.0f; }

		float getYVel() const { return ySpeed; }

	private:
		float ySpeed;
		float gravity{ 40.0f};

		GameObject* parent = nullptr;
	};
}