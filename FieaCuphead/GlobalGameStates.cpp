#include "GlobalGameStates.h"
#include "InputManager.h"
#include "ChangeGameState.h"
#include "CupheadEvents.h"
#include "EventDispatcher.h"

namespace Fiea::GameEngine
{
	//void IntroState::update(const GameTime& time, GameObject* gameObject)
	//{
	//}

	GameBeginState::GameBeginState(ChangeGameState* gm) : _gm(gm)
	{
		EventDispatcher::enqueue(OnGameStartEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
		_gm->setAllToIdle();
	}

	void GameBeginState::update(const GameTime& time, GameObject* gameObject)
	{
		if (!timeGrabbed) 
		{ 
			startTime = time.Get();
			timeGrabbed = true;
		}

		//wait for a couple seconds
		int secondsToWait = 1;

		if (time.Get() - startTime > secondsToWait * 1000)
		{
			startTime = time.Get();
			_gm->setGameState(new IntroState(_gm));
		}
	}


	IntroState::IntroState(ChangeGameState* gm) : _gm(gm)
	{
		//EventDispatcher::enqueue(OnTauntStartEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
		gm->setAllToTaunt();
	}

	void IntroState::update(const GameTime& time, GameObject* gameObject)
	{
		//On wallop animation end transition to phase 1


		if (!timeGrabbed)
		{
			startTime = time.Get();
			timeGrabbed = true;
		}

		//wait for a couple seconds
		float secondsToWaitForEvent = 1;
		int secondsToWait = 2;

		if (eventCalled == false && time.Get() - startTime > secondsToWaitForEvent * 1000)
		{
			eventCalled = true;
			EventDispatcher::enqueue(OnTauntStartEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
		}

		if (time.Get() - startTime > secondsToWait * 1000)
		{
			startTime = time.Get();
			_gm->setGameState(new Phase1Start(_gm));


		}
	
		//if (_gm->getFrogHealth() >= 1292)
		//{

		//}
	}


	Phase1Start::Phase1Start(ChangeGameState* gm) : _gm(gm)
	{
		EventDispatcher::enqueue(OnFightStartEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
		_gm->setAllToIdle();
	}


	void Phase1Start::update(const GameTime& time, GameObject* gameObject)
	{	

		if (!timeGrabbed)
		{
			startTime = time.Get();
			timeGrabbed = true;
		}

		//wait for a couple seconds
		int secondsToWait = 2;

		if (time.Get() - startTime > secondsToWait * 1000)
		{
			startTime = time.Get();
			_gm->setGameState(new Phase1Idle(_gm));


		}
	}

	Phase1Idle::Phase1Idle(ChangeGameState* gm) : _gm(gm)
	{
	}

	void Phase1Idle::update(const GameTime& time, GameObject* gameObject)
	{
		if (_gm->canStartPhase2())
		{
			_gm->setGameState(new phase2Start(_gm));
			return;
		}

		if (!timeGrabbed)
		{
			startTime = time.Get();
			timeGrabbed = true;
		}

		//wait for a couple seconds
		int secondsToWait = 6;

		if (time.Get() - startTime > secondsToWait * 1000)
		{
			startTime = time.Get();

			int randomNumber = rand() % 2;

			//randomly choose which frog to attack
			switch (randomNumber)
			{
			case 0:
				_gm->setGameState(new Phase1StateShortFrogAttack(_gm));
				break;
			case 1:
				_gm->setGameState(new phase1StateTallFrogAttack(_gm));
				break;
			default:
				break;
			}
			return;

		}

	}

	inline Phase1StateShortFrogAttack::Phase1StateShortFrogAttack(ChangeGameState* gm) : _gm(gm)
	{
		_gm->playShortFrogPhase1Attack();
	}

	void Phase1StateShortFrogAttack::update(const GameTime& time, GameObject* gameObject)
	{
		if (!_gm->isShortFrogPhase1Attacking())
		{
			_gm->setGameState(new Phase1Idle(_gm));
			return;
		}
	}

	phase1StateTallFrogAttack::phase1StateTallFrogAttack(ChangeGameState* gm) : _gm(gm)
	{
		_gm->playTallFrogPhase1Attack();

	}

	void phase1StateTallFrogAttack::update(const GameTime& time, GameObject* gameObject)
	{
		if (!_gm->isTallFrogPhase1Attacking())
		{
			_gm->setGameState(new Phase1Idle(_gm));
			return;
		}
	}

	phase2Start::phase2Start(ChangeGameState* gm) : _gm(gm)
	{
		_gm->playShortFrogPhase2Transition();
	}

	void phase2Start::update(const GameTime& time, GameObject* gameObject)
	{
		if (_gm->isShortFrogPhase2TransitionOver())
		{
			_gm->setGameState(new phase2Idle(_gm));
			return;
		}
	}

	phase2Idle::phase2Idle(ChangeGameState* gm) : _gm(gm)
	{
	}

	void phase2Idle::update(const GameTime& time, GameObject* gameObject)
	{
		if (_gm->casnStartPhase3())
		{
			_gm->setGameState(new Phase3Transition(_gm));
			return;
		}

		if (!shortFrogTimeGrabbed)
		{
			shortFrogTime = time.Get();
			shortFrogTimeGrabbed = true;
		}

		//wait for a couple seconds
		int shortFrogTimer = 3;

		if (time.Get() - shortFrogTime > shortFrogTimer * 1000)
		{
			shortFrogTime = time.Get();

			if (!_gm->isShortFrogPhase2Attacking())
			{
				_gm->playShortFrogPhase2Attack();

			}

		}

		if (!tallFrogTimeGrabbed)
		{
			tallFrogTime = time.Get();
			tallFrogTimeGrabbed = true;
		}

		int tallFrogTimer = 1;
		if (time.Get() - tallFrogTime > tallFrogTimer * 1000)
		{
			tallFrogTime = time.Get();

			if (!_gm->isTallFrogPhase2Attacking())
			{
				_gm->playTallFrogPhase2Attack();

			}

		}

	}


	Phase3Transition::Phase3Transition(ChangeGameState* gm) : _gm(gm)
	{
		_gm->playTallFrogPhase3Transition();
		_gm->playShortFrogPhase3Transition();
	}

	void Phase3Transition::update(const GameTime& time, GameObject* gameObject)
	{
		if (_gm->canEnterFinalPhaseLoop())
		{
			_gm->setGameState(new phase3Start(_gm));
			return;
		}
	}

	phase3Start::phase3Start(ChangeGameState* gm) : _gm(gm)
	{
		_gm->playTallFrogTransformation();

	}

	void phase3Start::update(const GameTime& time, GameObject* gameObject)
	{
		_gm->setGameState(new phase3Idle(_gm));
		//_gm->despawnShortFrog();
		return;
	}

	phase3Idle::phase3Idle(ChangeGameState* gm) : _gm(gm)
	{
	}

	void phase3Idle::update(const GameTime& time, GameObject* gameObject)
	{
		if (_gm->areAllFrogsDead())
		{
			_gm->setGameState(new BossDefeated(_gm));
			return;
		}
	}


	BossDefeated::BossDefeated(ChangeGameState* gm) : _gm(gm)
	{
		EventDispatcher::enqueue(OnBossDefeatEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
		_gm->playFrogDeath();
	}

	void BossDefeated::update(const GameTime& time, GameObject* gameObject)
	{
	}

}

