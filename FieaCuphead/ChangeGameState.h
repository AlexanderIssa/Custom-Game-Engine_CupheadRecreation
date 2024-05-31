#pragma once
#include "Action.h"
#include <vector>
#include "AnimRenderAction.h"
#include "Window.h"
#include "GameObject.h"
#include "GameClock.h"
#include "State.h"
#include "KeyDefinitions.h"
#include "EventDispatcher.h"

#include "GlobalGameStates.h"


namespace Fiea::GameEngine
{
	class OnCupheadTakeDamageEvent;

	class ChangeGameState : public Action
	{

		RTTI_DECLARATIONS(ChangeGameState, Action);
	public:
		ChangeGameState();
		void StartGame();
		ChangeGameState(GameObject* parent);
		ChangeGameState(const ChangeGameState& other) = default;
		ChangeGameState(ChangeGameState&& other) noexcept = default;
		ChangeGameState& operator=(const ChangeGameState& other) = default;
		ChangeGameState& operator=(ChangeGameState&& other) noexcept = default;
		virtual ~ChangeGameState();

		// Cloneable interface
		[[nodiscard]] virtual ChangeGameState* clone() const override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		// RTTI Overrides
		virtual std::string ToString() const override;

		void onEnable() override;
		void onDisable() override {};
		virtual void update(const GameTime& time) override;

		void setGameState(gameState* incomingState);

		int getFrogHealth() { return _frogHealth; }

		void setAllToTaunt();
		void setAllToIdle();

		void playShortFrogPhase1Attack();

		void playTallFrogPhase1Attack();

		void playShortFrogPhase2Transition();

		void playShortFrogPhase2Attack();

		void playTallFrogPhase2Attack();

		void playTallFrogPhase3Transition();

		void playShortFrogPhase3Transition();

		void playTallFrogTransformation();

		void playFrogDeath();

		void despawnShortFrog();

		GameObject* getShortFrog();
		GameObject* getTallFrog();
		GameObject* getCuphead();

		bool canStartPhase2();

		bool casnStartPhase3();

		bool isShortFrogPhase2TransitionOver();

		void OnDamageTaken(IEvent* eventData);

		bool isShortFrogPhase1Attacking();

		bool isTallFrogPhase1Attacking();

		bool isShortFrogPhase2Attacking();

		bool isTallFrogPhase2Attacking();

		bool canEnterFinalPhaseLoop();

		bool areAllFrogsDead();

	private:
		void OnBossDamageTaken(int damage);

		int _frogHealth{ 1700 };
		int _cupheadHealth{ 3 };

		GameObject* _cuphead;
		Context* _cupheadContext;

		GameObject* _shortFrog;
		Context* _shortFrogContext;

		GameObject* _tallFrog;
		Context* _tallFrogContext;


		long long startTime{ 0 };
		bool timeGrabbed = false;

		int phase1WaitTime{ 0 };

		bool phase1ShortFrogAttacking{ true };
		bool phase1Started{ false };

		bool phase2Started{ false };


		EventCallback* damageCallback;
		OnCupheadTakeDamageEvent* cupheadDamageEvent;

		gameState* _gameState;

		GameObject* _parentGameObject{};


	};
}