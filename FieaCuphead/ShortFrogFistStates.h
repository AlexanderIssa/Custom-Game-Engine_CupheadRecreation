#pragma once
#include "Action.h"
#include "AnimRenderAction.h"
#include "Window.h"
#include "GameObject.h"
#include "GameClock.h"
#include "State.h"
#include "KeyDefinitions.h"

#include <ctime>

/*
		//Short Frog Fist
		GLuint fistSpriteSheet;
		CTexture fistTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/ShortFrog/shortfrog_fist.png", fistSpriteSheet);
		GameObject fist{};
		fist.enabled = true;
		AnimRenderAction* fistAnimRenderAction = new AnimRenderAction(24, 1, 30, fistTexture);
		fistAnimRenderAction->setSize(120.0f, 191.0f);
		SpriteAnim fistAnim;
		fistAnim.nFrames = 2;
		fistAnimRenderAction->setAnim(fistAnim, false);
		fist.addAction(fistAnimRenderAction, "fistAnimRenderAction");
		fist.transform.position = glm::vec4(376.0f, 361.0f, 0.0f, 0.0f);
		fist.transform.scale = glm::vec4(0.8f, 0.5f, 1.0f, 0.0f);

		Context* fistState = new Context(new ShortFrogFistLoop(), &fist, &window, fistAnimRenderAction);
*/

namespace Fiea::GameEngine
{
	class ShortFrogFistStart : public State
	{

	public:
		ShortFrogFistStart()
		{
			stateName = "ShortFrogFistStart";

			animData.startingCell = 1;
			animData.nFrames = 2;
			frameRate = 24;
			nRows = 1;
			nCols = 30;
			currFrame = 0;

			height = 120;
			width = 191;
		}


		void update(const GameTime& time, GameObject* gameObject) override {}

	};

	class ShortFrogFistLoop : public State
	{

	public:
		ShortFrogFistLoop()
		{
			stateName = "ShortFrogFistLoop";

			animData.startingCell = 3;
			animData.nFrames = 13;
			frameRate = 24;
			nRows = 1;
			nCols = 30;
			currFrame = 0;

			height = 120;
			width = 191;
		}


		void update(const GameTime& time, GameObject* gameObject) override {}

	};

	class ShortFrogFistParryStart : public State
	{

	public:
		ShortFrogFistParryStart()
		{
			stateName = "ShortFrogFistParryStart";

			animData.startingCell = 16;
			animData.nFrames = 2;
			frameRate = 24;
			nRows = 1;
			nCols = 30;
			currFrame = 0;

			height = 120;
			width = 191;
		}


		void update(const GameTime& time, GameObject* gameObject) override {}
	};

	class shortFrogFistParryLoop : public State
	{

		shortFrogFistParryLoop()
		{
			stateName = "ShortFrogFistParryLoop";

			animData.startingCell = 18;
			animData.nFrames = 13;
			frameRate = 24;
			nRows = 1;
			nCols = 30;
			currFrame = 0;

			height = 120;
			width = 191;
		}


		void update(const GameTime& time, GameObject* gameObject) override {}


	};
}