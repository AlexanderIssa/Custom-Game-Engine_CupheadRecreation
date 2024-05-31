#include "ProjectileAction.h"
#include "TypeManager.h"
#include "GameObject.h"
#include "RenderAction.h"
#include "ParticleManager.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ProjectileDamageAction);

	ProjectileDamageAction::ProjectileDamageAction(IdType id) : DamageDealer(id)
	{
	}

	ProjectileDamageAction::ProjectileDamageAction() : DamageDealer(TypeIdClass())
	{
	}

	/**
	 * @brief Copy Constructor
	 * @param other 
	*/
	ProjectileDamageAction::ProjectileDamageAction(const ProjectileDamageAction& other) : DamageDealer(other), _velocity(other._velocity), _lifeTime(other._lifeTime), _speed(other._speed),
		deathAnim(other.deathAnim), projectileDeathTexture(other.projectileDeathTexture)
	{
	}

	ProjectileDamageAction::ProjectileDamageAction(glm::vec4 velocity, float lifeTime) : DamageDealer(TypeIdClass()), _velocity(velocity), _lifeTime(lifeTime), _timer(lifeTime)
	{
	}

	ProjectileDamageAction* ProjectileDamageAction::clone() const
	{
		return new ProjectileDamageAction(*this);
	}

	std::vector<Signature> ProjectileDamageAction::prescribedAttributes()
	{
		return std::vector<Signature>{
			{"vec4_velocity", Datum::DataType::VEC4, 1, offsetof(ProjectileDamageAction, _velocity)},
			{"float_lifeTime", Datum::DataType::FLOAT, 1, offsetof(ProjectileDamageAction, _lifeTime)},
			{"float_speed", Datum::DataType::FLOAT, 1, offsetof(ProjectileDamageAction, _speed)},
			{"int_deathWidth", Datum::DataType::INTEGER, 1, offsetof(ProjectileDamageAction, projectileDeathTexture.width)},
			{"int_deathHeight", Datum::DataType::INTEGER, 1, offsetof(ProjectileDamageAction, projectileDeathTexture.height)},
			{"int_deathDrawOrder", Datum::DataType::INTEGER, 1, offsetof(ProjectileDamageAction, projectileDeathTexture.drawOrder)},
			{"int_deathTexture", Datum::DataType::INTEGER, 1, offsetof(ProjectileDamageAction, projectileDeathTexture.textureRef)},
			{"int_deathNFrames", Datum::DataType::INTEGER, 1, offsetof(ProjectileDamageAction, deathAnim.nFrames)},
			{"float_deathSizeX", Datum::DataType::FLOAT, 1, offsetof(ProjectileDamageAction, deathSizeX)},
			{"float_deathSizeY", Datum::DataType::FLOAT, 1, offsetof(ProjectileDamageAction, deathSizeY)}
		};
	}

	bool ProjectileDamageAction::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string ProjectileDamageAction::ToString() const
	{
		return std::string("ProjectileDamageAction");
	}

	/**
	 * @brief onEnable sets the timer to lifetime
	*/
	void ProjectileDamageAction::onEnable()
	{
	}

	/**
	 * @brief Updates parent position and rotation according to velocity
	 * @param time 
	*/
	void ProjectileDamageAction::update(const GameTime& time)
	{
		if (_timer > 0.0f)
		{
			glm::vec4 posOffset = _velocity * (time.Frame() * 0.001f);
			glm::vec4 parentPos = getParentGameObject()->transform.position;

			getParentGameObject()->transform.position = parentPos + posOffset;
			_timer -= time.Frame() * 0.001f;
			if (_timer <= 0.0f)
			{
				disableSelf();
			}
		}
		else
		{
			//disableSelf();
		}
		
	}

	void ProjectileDamageAction::move(glm::vec4& direction)
	{
		_velocity = _speed * direction;

		if (_velocity.x < 0.0f)
		{
			Action* action = getParentGameObject()->getAction("AnimRenderAction");
			RenderAction* renderAction = action->As<RenderAction>();
			if (renderAction != nullptr)
			{
				renderAction->setMirror(true);
			}
		}

		_timer = _lifeTime;
	}

	void ProjectileDamageAction::OnCollisionPlayer(GameObject* hit, AbstractCollider* collider)
	{
		if (canDamagePlayer == 0) { return; }

		dealDamage(hit);
	}

	void ProjectileDamageAction::OnCollisionEnemy(GameObject* hit, AbstractCollider* collider)
	{
		if (canDamageEnemy == 0) { return; }

		dealDamage(hit);
		disableSelf();
	}

	void ProjectileDamageAction::OnCollisionProjectile(GameObject* hit, AbstractCollider* collider)
	{
		if (canDamageProjectile == 0) { return; }

		dealDamage(hit);
		disableSelf();
	}

	/**
	 * @brief Disables self
	*/
	void ProjectileDamageAction::disableSelf()
	{
		spawnParticle();
		getParentGameObject()->enabled = false;
		getParentGameObject()->alive = false;
	}

	void ProjectileDamageAction::spawnParticle()
	{
		if (projectileDeathTexture.textureRef == 0) { return; }

		GameObject* particle = ParticleManager::getParticle();

		ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
		if (pa == nullptr) { return; }

		particle->transform.position = getParentGameObject()->transform.position;
		pa->SetTextureAndAnim(projectileDeathTexture, deathAnim);
		pa->setSize(100.0f, 100.0f);
	}

}
