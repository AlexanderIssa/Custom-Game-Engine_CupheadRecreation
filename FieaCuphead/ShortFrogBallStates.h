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
		//Short Frog Ball
		GLuint ballSpriteSheet;
		CTexture ballTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/ShortFrog/shortfrog_ball_new.png", ballSpriteSheet);
		GameObject ball{};
		ball.enabled = true;
		AnimRenderAction* ballAnimRenderAction = new AnimRenderAction(12, 1, 6, ballTexture);
		ballAnimRenderAction->setSize(200.0f, 200.0f);
		SpriteAnim ballAnim;
		ballAnim.nFrames = 3;
		ballAnimRenderAction->setAnim(ballAnim, false);
		ball.addAction(ballAnimRenderAction, "ballAnimRenderAction");
		ball.transform.position = glm::vec4(378.0f, 285.0f, 0.0f, 0.0f);
		ball.transform.scale = glm::vec4(0.6f, 0.8f, 1.0f, 0.0f);

		Context* ballState = new Context(new BallDown(), &ball, &window, ballAnimRenderAction);
*/

namespace Fiea::GameEngine
{
	class BallUp : public State
	{
	public:
		BallUp()
		{
			stateName = "BallUp";

			animData.startingCell = 1;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 1;
			nCols = 6;
			currFrame = 0;

			height = 200;
			width = 200;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class BallDown : public State
	{
	public:
		BallDown()
		{
			stateName = "BallDown";

			animData.startingCell = 4;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 1;
			nCols = 6;
			currFrame = 0;

			height = 200;
			width = 200;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

}