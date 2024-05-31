#include "WorldCreator.h"
#include "ParseCoordinator.h"
#include "IntegerHandler.h"
#include "FloatHandler.h"
#include "StringHandler.h"
#include "Vector4Handler.h"
#include "Matrix4Handler.h"
#include "AttributedHandler.h"
#include "FactoryManager.h"
#include "GameObject.h"
#include "Collision.h"
#include "ProjectileAction.h"
#include "WeaponAction.h"
#include "CombatAction.h"
#include "DamageReciever.h"
#include "ParticleRenderAction.h"
#include "Factory.h"
#include "ChangeGameState.h"
#include "WindAction.h"
#include "GravityAction.h"
#include "HealthRenderAction.h"

#include "Reaction.h"
#include "DispatchEventAction.h"
#include "PlaySFXAction.h"

#include "CupheadEvents.h"

namespace Fiea
{
	namespace GameEngine
	{
		MAKE_FACTORY(Scope, AttributedFoo);
		MAKE_FACTORY(Scope, AttributedWarlock);
		MAKE_FACTORY(Scope, GameObject);
		MAKE_FACTORY(Scope, Monster);
		MAKE_FACTORY(Scope, ActionIncrement);
		MAKE_FACTORY(Scope, ActionList);
		MAKE_FACTORY(Scope, ActionListWhile);
		MAKE_FACTORY(Scope, ActionListSwitch);
		MAKE_FACTORY(Scope, RenderAction);
		MAKE_FACTORY(Scope, AnimRenderAction);
		MAKE_FACTORY(Scope, BoxCollider);
		MAKE_FACTORY(Scope, DamageDealer);
		MAKE_FACTORY(Scope, ProjectileDamageAction);
		MAKE_FACTORY(Scope, PeaShooter);
		MAKE_FACTORY(Scope, CombatAction);
		MAKE_FACTORY(Scope, DamageReceiver);
		MAKE_FACTORY(Scope, ParticleRenderAction);
		MAKE_FACTORY(Scope, ChangeGameState);
		MAKE_FACTORY(Scope, WindAction);
		MAKE_FACTORY(Scope, GravityAction);

		MAKE_FACTORY(Scope, Reaction);
		MAKE_FACTORY(Scope, DispatchEventAction);
		MAKE_FACTORY(Scope, PlaySFXAction);
		MAKE_FACTORY(Scope, PlaySFXRandomAction);

		MAKE_FACTORY(Scope, HealthRenderAction);

		MAKE_FACTORY(IEvent, OnGameStartEvent);
		MAKE_FACTORY(IEvent, OnTauntStartEvent);
		MAKE_FACTORY(IEvent, OnFightStartEvent);
		MAKE_FACTORY(IEvent, OnPeaSpawnEvent);
		MAKE_FACTORY(IEvent, OnStartJumpEvent);
		MAKE_FACTORY(IEvent, OnStartDashEvent);
		MAKE_FACTORY(IEvent, OnCupheadTakeDamageEvent);
		MAKE_FACTORY(IEvent, OnCupheadDieEvent);
		MAKE_FACTORY(IEvent, OnBossDefeatEvent);

		GameObject* WorldCreator::CreateTheWorld()
		{
			// Add a factory
			FactoryManager<Scope>::add(std::make_unique<GameObjectFactory>());
			FactoryManager<Scope>::add(std::make_unique<RenderActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<AnimRenderActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<BoxColliderFactory>());
			FactoryManager<Scope>::add(std::make_unique<DamageDealerFactory>());
			FactoryManager<Scope>::add(std::make_unique<ProjectileDamageActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<PeaShooterFactory>());
			FactoryManager<Scope>::add(std::make_unique<CombatActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<DamageReceiverFactory>());
			FactoryManager<Scope>::add(std::make_unique<ParticleRenderActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<ActionIncrementFactory>());
			FactoryManager<Scope>::add(std::make_unique<ReactionFactory>());
			FactoryManager<Scope>::add(std::make_unique<DispatchEventActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<PlaySFXActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<PlaySFXRandomActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<ChangeGameStateFactory>());
			FactoryManager<Scope>::add(std::make_unique<WindActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<GravityActionFactory>());
			FactoryManager<Scope>::add(std::make_unique<HealthRenderActionFactory>());

			// Add facories for events
			FactoryManager<IEvent>::add(std::make_unique<OnGameStartEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnTauntStartEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnFightStartEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnPeaSpawnEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnStartJumpEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnStartDashEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnCupheadTakeDamageEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnCupheadDieEventFactory>());
			FactoryManager<IEvent>::add(std::make_unique<OnBossDefeatEventFactory>());

			// Initialize the parser
			ParseCoordinator worldParser(worldWrapper);
			worldParser.addHandler(std::make_unique<IntegerHandler>());
			worldParser.addHandler(std::make_unique<FloatHandler>());
			worldParser.addHandler(std::make_unique<StringHandler>());
			worldParser.addHandler(std::make_unique<Vector4Handler>());
			worldParser.addHandler(std::make_unique<Matrix4Handler>());
			worldParser.addHandler(std::make_unique<AttributedHandler>());

			// Parse the file
			bool deserialized = worldParser.deserializeObjectFromFile("../FieaCuphead/JSON/GameWorld.json");
			if (!deserialized)
			{
				throw std::invalid_argument("Failed to deserialize object from file");
			}

			// Get the world object
			Scope* root = worldWrapper.getRootTable();
			world = (*root)[0].get<Scope*>(0)->As<GameObject>(); // Try to get the first JSON object as a GameObject
			if (world == nullptr)
			{
				throw std::invalid_argument("Failed to get world object");
			}

			world->enabled = true;
			return world;
		}
	}
}