#include "WeaponAction.h"
#include "TypeManager.h"
#include "ObjectPool.h"
#include "GameClock.h"
#include "GameObject.h"
#include "State.h"
#include "ProjectileAction.h"
#include "GamepadHelper.h"
#include "KeyDefinitions.h"
#include "ParticleManager.h"
#include "EventDispatcher.h"

namespace Fiea::GameEngine
{
#pragma region WeaponAction
	RTTI_DEFINITIONS(WeaponAction);

	WeaponAction::WeaponAction(IdType id) : Action(id)
	{	
	}

	WeaponAction::WeaponAction(float rate, const std::string& pref, IdType id) : Action(id), fireRate(rate), projectilePrefab(pref)
	{
	}

	WeaponAction::~WeaponAction()
	{
		if (projectilePool != nullptr)
		{
			delete projectilePool;
		}
	}

	void WeaponAction::onEnable()
	{
		//projectilePool = new ObjectPool(projectilePrefab);
	}

	void WeaponAction::onDisable()
	{
	}

	void WeaponAction::update(const GameTime& time)
	{
		if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
		{
			fire();
		}

		if (fireTimer > 0.0f)
		{
			fireTimer -= time.Frame() * 0.001f;
		}
	}

	std::vector<Signature> WeaponAction::prescribedAttributes()
	{
		return std::vector<Signature> {
			{"str_projectilePrefab", Datum::DataType::STRING, 1, offsetof(WeaponAction, projectilePrefab)},
			{"float_fireRate", Datum::DataType::FLOAT, 1, offsetof(WeaponAction, fireRate)}
		};
	}

	bool WeaponAction::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string WeaponAction::ToString() const
	{
		return std::string("WeaponAction");
	}

	/**
	 * @brief Uses the context to determine the firing direction, lazily inits 
	 * @return 
	*/
	Transform WeaponAction::getDirectionAndRotation()
	{
		// buffwe.position = direction, buffer.rotation = rotation
		Transform buffer;
		if (context == nullptr)
		{
			initContext();
		}

		const State* state = context->GetState();
		const std::string stateName = state->stateName;

		//glm::vec4 direction{1.0f, 0.0f, 0.0f, 0.0f};
		if (stateName == "cupheadShoot_Down")
		{
			//direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
			buffer.position = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, 90.0f, 0.0f);
			isCrouched = false;
		}
		else if (stateName == "cupheadShoot_Diag_Down")
		{
			//direction = glm::vec4(0.7071f, -0.7071f, 0.0f, 0.0f);
			buffer.position = glm::vec4(0.7071f, -0.7071f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, 60.0f, 0.0f);
			isCrouched = false;
		}
		else if (stateName == "cupheadShoot_Straight" || stateName == "cupheadRun_Shoot_Straight")
		{
			//direction = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			buffer.position = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			isCrouched = false;
		}
		else if (stateName == "cupheadShoot_Diag_Up" || stateName == "cupheadRun_Shoot_Diag_Up")
		{
			//direction = glm::vec4(0.7071f, 0.7071f, 0.0f, 0.0f);
			buffer.position = glm::vec4(0.7071f, 0.7071f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, -60.0f, 0.0f);
			isCrouched = false;
		}
		else if (stateName == "cupheadShoot_Up")
		{
			//direction = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			buffer.position = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, -90.0f, 0.0f);
			isCrouched = false;
		}
		else if (stateName == "CupheadDuck_Shoot")
		{
			buffer.position = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			isCrouched = true;
		}
		else if (stateName == "CupheadDuckIdle")
		{
			buffer.position = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			isCrouched = true;
		}
		else if (stateName == "cupheadJump")
		{
			buffer.position = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
			buffer.rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			isCrouched = false;
		}

		// A Key
		if (context->GetAnimRenderAction()->isMirrored())
		{
			//direction.x *= -1.0;
			buffer.position.x *= -1.0;
		}


		//return direction;
		return buffer;
	}

	/**
	 * @brief Finds the context action on the parent gameObj
	*/
	void WeaponAction::initContext()
	{
		Action* contextAction = getParentGameObject()->getAction(std::string("Context"));
		if (contextAction == nullptr) { throw std::runtime_error("Parent Gameobject doesn't have a context!"); }

		context = contextAction->As<Context>();
		if (context == nullptr) { throw std::runtime_error("Parent Gameobject doesn't have a context!"); }
	}

#pragma endregion WeaponAction

#pragma region PeaShooter
	RTTI_DEFINITIONS(PeaShooter);

	PeaShooter::PeaShooter() : WeaponAction(TypeIdClass())
	{		
		peaEventPayload = new OnPeaSpawnEvent();

		std::function<void(IEvent*)> boundFunc = BIND_FUNC(this, PeaShooter::spawnFireEffect);
		spawnFireCallback = new EventCallback(boundFunc);
		EventDispatcher::addListener<OnPeaSpawnEvent>(spawnFireCallback);
	}

	PeaShooter::PeaShooter(float rate, const std::string& pref) : WeaponAction(rate, pref, TypeIdClass())
	{

	}

	PeaShooter::~PeaShooter()
	{
		delete peaEventPayload;
		delete spawnFireCallback;
	}

	PeaShooter* PeaShooter::clone() const
	{
		return new PeaShooter(*this);
	}

	void PeaShooter::onEnable()
	{
		WeaponAction::onEnable();
	}

	void PeaShooter::onDisable()
	{
	}

	void PeaShooter::update(const GameTime& time)
	{
		WeaponAction::update(time);
	}

	std::vector<Signature> PeaShooter::prescribedAttributes()
	{
		return std::vector<Signature>{};
	}

	bool PeaShooter::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string PeaShooter::ToString() const
	{
		return std::string("PeaShooter");
	}
	
	/**
	 * @brief Spawn fire effect
	 * @param pos 
	*/
	void PeaShooter::spawnFireEffect(IEvent* payload)
	{
		OnPeaSpawnEvent* p = payload->As<OnPeaSpawnEvent>();

		GameObject* particle = ParticleManager::getParticle();

		particle->transform.position = p->pos;

		ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
		if (pa == nullptr) { return; }

		CTexture tex;
		tex.textureRef = 19;
		tex.width = 420;
		tex.height = 99;
		tex.drawOrder = 21;
		SpriteAnim anim;
		anim.startingCell = 0;
		anim.nFrames = 4;

		pa->SetTextureAndAnim(tex, anim);
		pa->setSize(50.0f, 50.0f);
	}

	/**
	 * @brief Checks if we can fire
	 * @return 
	*/
	bool PeaShooter::canFire()
	{
		// If we can't fire, bail
		if (fireTimer > 0.0f) { return false; }
		
		if (context == nullptr)
		{
			initContext();
		}

		const std::string stateName = context->GetState()->stateName;
		
		// states that cuphead can shoot from
		if (stateName == "cupheadShoot_Down" || stateName == "cupheadShoot_Diag_Down" || stateName == "cupheadShoot_Straight" || stateName == "cupheadRun_Shoot_Straight" ||
			stateName == "cupheadShoot_Diag_Up" || stateName == "cupheadRun_Shoot_Diag_Up" || stateName == "cupheadShoot_Up" || stateName == "CupheadDuck_Shoot" || stateName == "CupheadDuckIdle" ||
			stateName == "cupheadJump")
		{
			return true;
		}

		return false;

	}

	void PeaShooter::fire()
	{
		if (!canFire()) { return; }

		fireTimer = fireRate;
		if (projectilePool == nullptr)
		{
			projectilePool = new ObjectPool(projectilePrefab, 24);
		}

		GameObject* projectile = projectilePool->GetPooledObject();
		Action* projectileAction = projectile->getAction("ProjectileDamageAction");

		Transform temp = getDirectionAndRotation();
		float yOffset;
		// Set position offset
		if (isCrouched)
		{
			yOffset = crouchYOffsets[currOffsetIndex];
			++currOffsetIndex;
			if (currOffsetIndex >= 4)
			{
				currOffsetIndex = 0;
			}
		}
		else
		{
			yOffset = yOffsets[currOffsetIndex];
			++currOffsetIndex;
			if (currOffsetIndex >= 4)
			{
				currOffsetIndex = 0;
			}
		}

		ProjectileDamageAction* projectileDamageAction = projectileAction->As<ProjectileDamageAction>();

		// Set velocity here
		glm::vec4 dir = temp.position;
		glm::vec4 forwardOffset = dir * 45.0f;
		glm::vec4 spawnPos;

		if (isCrouched)
		{
			spawnPos = getParentGameObject()->transform.position + forwardOffset;
			spawnPos.y += -35.0f;	// lower spawn pos to crouch level
			peaEventPayload->pos = spawnPos;
			spawnPos = getParentGameObject()->transform.position + forwardOffset; // reset spawn pos
		}
		else
		{
			spawnPos = getParentGameObject()->transform.position + forwardOffset;
			peaEventPayload->pos = spawnPos;
			//spawnFireEffect(spawnPos);
		}
		spawnPos.y += yOffset;


		projectile->transform.position = spawnPos;
		projectile->transform.rotation = temp.rotation;
		projectileDamageAction->move(dir);
		
		// jeff code: call OnPeaSpawnEvent
		EventDispatcher::enqueue(OnPeaSpawnEvent::TypeIdClass(), peaEventPayload, CreateElapsedTime(0), 0);
	}

	void PeaShooter::exFire()
	{
	}

#pragma endregion PeaShooter
}
