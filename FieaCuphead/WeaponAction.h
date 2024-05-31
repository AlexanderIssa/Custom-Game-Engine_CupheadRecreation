#pragma once
#include "Action.h"
#include "Transform.h"
#include "IEvent.h"
#include "CupheadEvents.h"
#include "EventDispatcher.h"

namespace Fiea::GameEngine
{
	class ObjectPool;
	class Context;

	class WeaponAction : public Action 
	{
		RTTI_DECLARATIONS(WeaponAction, Action);
	protected:
		WeaponAction(IdType id);
		WeaponAction(float rate, const std::string& pref, IdType id);

	public:
		WeaponAction();
		virtual ~WeaponAction();

		// Cloneable interface
		[[nodiscard]] virtual WeaponAction* clone() const override = 0;

		virtual void onEnable() override;

		virtual void onDisable() override;

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		virtual bool canFire() = 0;
		virtual void fire() = 0;
		virtual void exFire() = 0;

	protected:

		Transform getDirectionAndRotation();
		void initContext();

		ObjectPool* projectilePool;

		// Need a context for getting direction
		Context* context;

		std::string projectilePrefab;
		float fireRate;
		float fireTimer;
		bool isCrouched = false;

	private:
	};

	class PeaShooter final : public WeaponAction
	{
		RTTI_DECLARATIONS(PeaShooter, WeaponAction);
	private:
		OnPeaSpawnEvent* peaEventPayload = nullptr;
		EventCallback* spawnFireCallback = nullptr;
	public:
		PeaShooter();
		PeaShooter(float rate, const std::string& pref);
		virtual ~PeaShooter();

		// Cloneable interface
		[[nodiscard]] PeaShooter* clone() const override;

		virtual void onEnable() override;

		virtual void onDisable() override;

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		int currOffsetIndex = 0;
		float yOffsets[4] = { 0, 35, 0, -35 };
		float crouchYOffsets[4] = { -15, 0, -15, -35 };

		void spawnFireEffect(IEvent* payload);
		bool canFire() override;
		void fire() override;
		void exFire() override;
	};
}
