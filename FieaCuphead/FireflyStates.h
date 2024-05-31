#pragma once
#include "Action.h"
#include "AnimRenderAction.h"
#include "Window.h"
#include "GameObject.h"
#include "GameClock.h"
#include "State.h"
#include "KeyDefinitions.h"
#include "DamageDealer.h"
#include "DamageReciever.h"

#include <ctime>

/*
 		//Firefly
		GLuint fireflySpriteSheet;
		CTexture fireflyTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/Firefly/tallfrog_firefly.png", fireflySpriteSheet);
		GameObject firefly{};
		firefly.enabled = true;
		AnimRenderAction* fireflyAnimRenderAction = new AnimRenderAction(24, 1, 31, fireflyTexture);
		fireflyAnimRenderAction->setSize(600.0f, 600.0f);
		SpriteAnim fireflyAnim;
		fireflyAnim.nFrames = 10;
		fireflyAnimRenderAction->setAnim(fireflyAnim, false);
		firefly.addAction(fireflyAnimRenderAction, "fireflyAnimRenderAction");
		firefly.transform.position = glm::vec4(378.0f, 285.0f, 0.0f, 0.0f);
		firefly.transform.scale = glm::vec4(0.65f, 0.85f, 1.0f, 0.0f);

		Context* fireflyState = new Context(new FireflyLeft(), &firefly, &window, fireflyAnimRenderAction);
*/

namespace Fiea::GameEngine
{
	class FireflyIdle : public State
	{

	public:
		FireflyIdle()
		{
			stateName = "FireflyIdle";

			animData.startingCell = 1;
			animData.nFrames = 10;
			frameRate = 24;
			nRows = 1;
			nCols = 31;
			currFrame = 0;

			height = 600;
			width = 600;

			//subscribing to OnDamageEvent
			auto boundCollisionFunc = BIND_FUNC(this, FireflyIdle::OnDamage);
			onDamageEventCallback = new EventCallback(boundCollisionFunc);
			EventDispatcher::addListener<OnDamageDealtEvent>(onDamageEventCallback);

			auto boundDiedFunc = BIND_FUNC(this, FireflyIdle::OnDie);
			diedEventCallback = new EventCallback(boundDiedFunc);
			EventDispatcher::addListener<OnDeathEvent>(diedEventCallback);
		}


		void update(const GameTime& time, GameObject* gameObject) override;

		void OnDamage(IEvent* eventData);

		void OnDie(IEvent* eventData);

		~FireflyIdle()
		{
			EventDispatcher::removeListener<OnDamageDealtEvent>(onDamageEventCallback);
			EventDispatcher::removeListener<OnDeathEvent>(diedEventCallback);
		}

		EventCallback* onDamageEventCallback;
		EventCallback* diedEventCallback;

		bool isWaiting = false;                                                         
		float waitTime = 0.0f;
		float elapsedTime = 0.0f;

	};

	class FireflyLeft : public State
	{

	public:
		FireflyLeft()
		{
			stateName = "FireflyLeft";

			animData.startingCell = 11;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 1;
			nCols = 31;
			currFrame = 0;

			height = 600;
			width = 600;

			//subscribing to OnDamageEvent
			auto boundCollisionFunc = BIND_FUNC(this, FireflyLeft::OnDamage);
			onDamageEventCallback = new EventCallback(boundCollisionFunc);
			EventDispatcher::addListener<OnDamageDealtEvent>(onDamageEventCallback);

			auto boundDiedFunc = BIND_FUNC(this, FireflyLeft::OnDie);
			diedEventCallback = new EventCallback(boundDiedFunc);
			EventDispatcher::addListener<OnDeathEvent>(diedEventCallback);
		}

		float targetX = 0.0f;
		float targetY = 0.0f;
		bool isMoving = false;

		~FireflyLeft()
		{
			EventDispatcher::removeListener<OnDamageDealtEvent>(onDamageEventCallback);
			EventDispatcher::removeListener<OnDeathEvent>(diedEventCallback);
		}

		void update(const GameTime& time, GameObject* gameObject) override;

		void OnDamage(IEvent* eventData);
		void OnDie(IEvent* eventData);

		EventCallback* onDamageEventCallback;
		EventCallback* diedEventCallback;
	};

	class FireflyDown : public State
	{

	public:
		FireflyDown()
		{
			stateName = "FireflyDown";

			animData.startingCell = 19;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 1;
			nCols = 31;
			currFrame = 0;

			height = 600;
			width = 600;

			//subscribing to OnDamageEvent
			auto boundCollisionFunc = BIND_FUNC(this, FireflyDown::OnDamage);
			onDamageEventCallback = new EventCallback(boundCollisionFunc);
			EventDispatcher::addListener<OnDamageDealtEvent>(onDamageEventCallback);

			auto boundDiedFunc = BIND_FUNC(this, FireflyDown::OnDie);
			diedEventCallback = new EventCallback(boundDiedFunc);
			EventDispatcher::addListener<OnDeathEvent>(diedEventCallback);
		}


		void update(const GameTime& time, GameObject* gameObject) override;

		void OnDamage(IEvent* eventData);
		void OnDie(IEvent* eventData);

		~FireflyDown()
		{
			EventDispatcher::removeListener<OnDamageDealtEvent>(onDamageEventCallback);
			EventDispatcher::removeListener<OnDeathEvent>(diedEventCallback);
		}

		EventCallback* onDamageEventCallback;
		EventCallback* diedEventCallback;

		float targetX = 0.0f;
		float targetY = 0.0f;
		bool isMoving = false;

		size_t movesLeft = 3;

		float destination_x = 0.0f;
		float destination_y = 0.0f;
		float destination_dist = 0.0f;

	};

	class FireflyUp : public State
	{

	public:
		FireflyUp()
		{
			stateName = "FireflyUp";

			animData.startingCell = 15;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 1;
			nCols = 31;
			currFrame = 0;

			height = 600;
			width = 600;
		}


		void update(const GameTime& time, GameObject* gameObject) override {}


	};

	class FireflyDeath : public State
	{

	public:
		FireflyDeath()
		{
			stateName = "FireflyDeath";

			animData.startingCell = 23;
			animData.nFrames = 9;
			frameRate = 24;
			nRows = 1;
			nCols = 31;
			currFrame = 0;

			height = 600;
			width = 600;

			loopLimit = 1;
		}

		bool isDead = false;

		void update(const GameTime& time, GameObject* gameObject) override;

	};
}