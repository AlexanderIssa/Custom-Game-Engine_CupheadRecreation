#pragma once
#include "DamageDealer.h"
#include "AnimRenderAction.h"

namespace Fiea::GameEngine
{

	class ProjectileDamageAction : public DamageDealer
	{
		RTTI_DECLARATIONS(ProjectileDamageAction, DamageDealer);
	protected:
		ProjectileDamageAction(IdType id);

	public:
		ProjectileDamageAction();
		ProjectileDamageAction(const ProjectileDamageAction& other);
		ProjectileDamageAction(glm::vec4 velocity, float lifeTime);
		virtual ~ProjectileDamageAction() = default;

		[[nodiscard]] ProjectileDamageAction* clone() const override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI OverridesS
		virtual std::string ToString() const override;

		void onEnable() override;
		void onDisable() override {};
		virtual void update(const GameTime& time) override;

		void setVelocity(glm::vec4 velocity) { _velocity = velocity; }

		void move(glm::vec4& direction);

	protected:
		void OnCollisionPlayer(GameObject* hit, AbstractCollider* collider) override;
		void OnCollisionEnemy(GameObject* hit, AbstractCollider* collider) override;
		void OnCollisionProjectile(GameObject* hit, AbstractCollider* collider) override;

		void disableSelf();
		void spawnParticle();

		glm::vec4 _velocity{0.0f};
		float _lifeTime{ 1.0f };
		float _timer;
		float _speed;

		CTexture projectileDeathTexture;
		SpriteAnim deathAnim;
		float deathSizeX;
		float deathSizeY;
	};
}