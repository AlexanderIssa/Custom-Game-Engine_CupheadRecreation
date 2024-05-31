#pragma once
#include "GameClock.h" 
#include "GameObject.h"
#include "KeyDefinitions.h"


namespace Fiea::GameEngine
{

	class ChangeGameState;

	class gameState
	{
	public:
		gameState() {};
		virtual void update(const GameTime& time, GameObject* gameObject) = 0;
		virtual ~gameState() {};
	};

	class GameBeginState : public gameState
	{
	public:
		GameBeginState(ChangeGameState* gm);

		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		long long startTime = 0;
		bool timeGrabbed = false;

		ChangeGameState* _gm;
	};


	class IntroState : public gameState
	{
	public:
		IntroState(ChangeGameState* gm);

		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		long long startTime = 0;
		bool timeGrabbed = false;
		bool eventCalled = false;

		ChangeGameState* _gm;


	};

	class Phase1Start : public gameState
	{
	public:
		Phase1Start(ChangeGameState* gm);

		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		long long startTime = 0;
		bool timeGrabbed = false;

		ChangeGameState* _gm;
	
	};

	class Phase1Idle : public gameState
	{

	public:
		Phase1Idle(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		long long startTime = 0;
		bool timeGrabbed = false;

		ChangeGameState* _gm;
	};

	class Phase1StateShortFrogAttack : public gameState
	{
	public:
		Phase1StateShortFrogAttack(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;
	};

	class phase1StateTallFrogAttack : public gameState
	{
	public:
		phase1StateTallFrogAttack(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;

	};

	class phase2Start : public gameState
	{
	public:
		phase2Start(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;

	};

	class phase2Idle : public gameState
	{
	public:
		phase2Idle(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		long long shortFrogTime = 0;
		long long tallFrogTime = 0;
		bool shortFrogTimeGrabbed = false;
		bool tallFrogTimeGrabbed = false;

		ChangeGameState* _gm;
	};

	class Phase3Transition : public gameState
	{
	public:
		Phase3Transition(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;
	};


	class phase3Start : public gameState
	{
	public:
		phase3Start(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;
	};

	class phase3Idle : public gameState
	{
	public:
		phase3Idle(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;
	};

	class BossDefeated : public gameState
	{
public:
		BossDefeated(ChangeGameState* gm);
		// Inherited via State
		void update(const GameTime& time, GameObject* gameObject) override;

		ChangeGameState* _gm;
	};

}