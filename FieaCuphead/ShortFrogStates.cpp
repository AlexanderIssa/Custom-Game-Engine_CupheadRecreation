#include "ShortFrogStates.h"
#include "InputManager.h"
#include "ProjectileAction.h"
#include "AnimRenderAction.h"
#include "ShortFrogBallStates.h"
#include <cstdlib>
#include <ctime>

void Fiea::GameEngine::ShortFrogIntro::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(150);
		_boxCollider->setHalfWidth(75);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogIdle());
	}
}

void Fiea::GameEngine::ShortFrogIdle::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(150);
		_boxCollider->setHalfWidth(75);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	//if (InputManager::GetKeyDown(VK_Q))
	//{
	//	_context->TransitionTo(new ShortFrogLiftLeg());
	//}
	//else if (InputManager::GetKeyDown(VK_E))
	//{
	//	_context->TransitionTo(new ShortFrogStanceUp());
	//}
	//else if (InputManager::GetKeyDown(VK_R))
	//{
	//	_context->TransitionTo(new ShortFrogIdleToClapCharge(0));
	//}
}

void Fiea::GameEngine::ShortFrogLiftLeg::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogChargeLeg());
	}
}

void Fiea::GameEngine::ShortFrogChargeLeg::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogArmSpinStart());
	}
}

void Fiea::GameEngine::ShortFrogArmSpinStart::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogArmSpinLoop());
	}
}

Fiea::GameEngine::ObjectPool* Fiea::GameEngine::ShortFrogArmSpinLoop::fistPool = nullptr;

void Fiea::GameEngine::ShortFrogArmSpinLoop::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(150);
		_boxCollider->setHalfWidth(150);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (projectileTimer > 0.0f)
	{
		projectileTimer -= time.Frame() * 0.001f;
	}
	else
	{
		if (fistPool == nullptr)
		{
			fistPool = new ObjectPool("../FieaCuphead/JSON/FrogFistProjectile.json");
		}

		GameObject* rageFireBall = fistPool->GetPooledObject();
		Action* projectileAction = rageFireBall->getAction("ProjectileDamageAction");
		ProjectileDamageAction* p = projectileAction->As<ProjectileDamageAction>();

		glm::vec4 dir{ -1.0f, 0.0f, 0.0f, 0.0f};
		p->move(dir);

		float yPos = 180.0f;
		float currYOffset = pattern[currIndex];

		rageFireBall->transform.position = glm::vec4(500.0f, yPos + currYOffset, 0.0f, 0.0f);
		projectileTimer = projectileDelay;

		++currIndex;
		if (currIndex >= 4)
		{
			currIndex = 0;
		}
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogArmSpinEnd());
	}
}

void Fiea::GameEngine::ShortFrogArmSpinEnd::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogIdle(22.0f));
	}
}

void Fiea::GameEngine::ShortFrogStanceUp::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogShakeBooty());
	}
}

void Fiea::GameEngine::ShortFrogShakeBooty::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogRollStart());
	}
}

void Fiea::GameEngine::ShortFrogRollStart::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(100);
		_boxCollider->setHalfWidth(75);
		_boxCollider->setPosOffset(glm::vec4(-100, -50, 0, 0));

		firstFrame = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogRollInPlace());
	}
}

void Fiea::GameEngine::ShortFrogRollInPlace::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(100);
		_boxCollider->setHalfWidth(75);
		_boxCollider->setPosOffset(glm::vec4(-100, -50, 0, 0));

		firstFrame = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogRollToEndOfScreen());
	}
}

void Fiea::GameEngine::ShortFrogRollToEndOfScreen::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(100);
		_boxCollider->setHalfWidth(75);
		_boxCollider->setPosOffset(glm::vec4(-100, -50, 0, 0));

		firstFrame = false;
	}

	gameObject->transform.position.x -= 25.0f * time.Frame() * 0.01f;

	if (gameObject->transform.position.x < -50.0f) 
	{ 
		_animRenderAction->setMirror(true);
		_context->TransitionTo(new ShortFrogOffScreen());
	}

}

void Fiea::GameEngine::ShortFrogOffScreen::update(const GameTime& time, GameObject* gameObject)
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
		_context->TransitionTo(new ShortFrogRollReenterScreen());

	}


}

void Fiea::GameEngine::ShortFrogRollReenterScreen::update(const GameTime& time, GameObject* gameObject)
{

	gameObject->transform.position.x += 25.0f * time.Frame() * 0.01f;

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogIdle());
	}
}

void Fiea::GameEngine::ShortFrogIdleToClapCharge::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogClapCharge(currentClap));
	}
}

void Fiea::GameEngine::ShortFrogClapCharge::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogClap(currentClap));
	}
}

void Fiea::GameEngine::ShortFrogClap::update(const GameTime& time, GameObject* gameObject)
{
	// spawn a planet ball once on frame switch
	if (firstFrame)
	{
		// get a new object pool for planets
		if (planetPool == nullptr)
		{
			planetPool = new ObjectPool("../FieaCuphead/JSON/BallPrefab.json");
		}

		GameObject* planetBall = planetPool->GetPooledObject();

		planetBall->transform.position = glm::vec4(250.0f, 180.0, 0.0f, 0.0f);

		// mix up spawn position based on a random 50/50 chance
		// Seed the random number generator using the current time
		srand(static_cast<unsigned int>(std::time(nullptr)));

		// Generate a random number between 0 and RAND_MAX (typically at least 32767)
		int randomNumber = rand();

		// Map the random number to either 0 or 1
		int result = randomNumber % 2;
		Context* ballState;

		switch (result)
		{
		case 0:
			ballState = new Context(new BallUp(), planetBall, planetBall->getAction("AnimRenderAction")->As<AnimRenderAction>());
			planetBall->addAction(ballState, "Context");
			break;
		case 1:
			ballState = new Context(new BallDown(), planetBall, planetBall->getAction("AnimRenderAction")->As<AnimRenderAction>());
			planetBall->addAction(ballState, "Context");
			break;
		default:
			break;
		}

		//if (spawnBallUp)
		//{
		//	Context* ballState = new Context(new BallUp(), planetBall, planetBall->getAction("AnimRenderAction")->As<AnimRenderAction>());
		//	planetBall->addAction(ballState, "Context");
		//	spawnBallUp = false;
		//}
		//else
		//{
		//	Context* ballState = new Context(new BallDown(), planetBall, planetBall->getAction("AnimRenderAction")->As<AnimRenderAction>());
		//	planetBall->addAction(ballState, "Context");
		//	spawnBallUp = true;
		//}


		firstFrame = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		if (currentClap <= 3)
		{
			++currentClap;
			_context->TransitionTo(new ShortFrogClapToClapCharge(currentClap));
		}
		else
		{
			_context->TransitionTo(new ShortFrogClapToIdleTransition(currentClap));
		}
	}
}

void Fiea::GameEngine::ShortFrogClapToClapCharge::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogClapCharge(currentClap));
	}
}

void Fiea::GameEngine::ShortFrogClapToIdleTransition::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogIdle(18));
	}
}

void Fiea::GameEngine::ShortFrogFinalStanceUpStart::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogFinalBootyShake());
	}
}

void Fiea::GameEngine::ShortFrogFinalBootyShake::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogFinalRollStart());
	}
}

void Fiea::GameEngine::ShortFrogFinalRollStart::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new ShortFrogFinalRollOut());
	}
}

void Fiea::GameEngine::ShortFrogFinalRollOut::update(const GameTime& time, GameObject* gameObject)
{
	gameObject->transform.position.x += 25.0f * time.Frame() * 0.01f;

}
