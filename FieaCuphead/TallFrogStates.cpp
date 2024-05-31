#include "TallFrogStates.h"
#include "InputManager.h"

void Fiea::GameEngine::TallFrogIntro::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogIdle());
	}
}

void Fiea::GameEngine::TallFrogIdle::update(const GameTime& time, GameObject* gameObject)
{
	
	//if (InputManager::GetKeyDown(VK_C))
	//{
	//	_context->TransitionTo(new TallFrogSpit_Charge_Start());
	//}
	//else if (InputManager::GetKeyDown(VK_T))
	//{
	//	_context->TransitionTo(new TallFrogFan_Start());
	//}
	//else if (InputManager::GetKeyDown(VK_I))
	//{
	//	_context->TransitionTo(new TallFrog_Abre());
	//}

	if (TallFrogFan_Start::wind != nullptr)
	{
		TallFrogFan_Start::wind->enabled = false;
	}
}

void Fiea::GameEngine::TallFrogSpit_Charge_Start::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogSpit_Charge_Loop(volleySpitLimit, currentSpitVolley));
	}
}

void Fiea::GameEngine::TallFrogSpit_Charge_Loop::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogSpit_Start(volleySpitLimit, currentSpitVolley));
	}
}

void Fiea::GameEngine::TallFrogSpit_Start::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		++currentSpitVolley;
		_context->TransitionTo(new TallFrogSpit_Loop(volleySpitLimit, currentSpitVolley));
	}
}

Fiea::GameEngine::ObjectPool* Fiea::GameEngine::TallFrogSpit_Loop::FireFlyPool = nullptr;

void Fiea::GameEngine::TallFrogSpit_Loop::update(const GameTime& time, GameObject* gameObject)
{	
	if (_animRenderAction->getCurrentFrame() > 3 && _animRenderAction->getCurrentFrame() < 3.2f)
	{
		if (FireFlyPool == nullptr) {
			FireFlyPool = new ObjectPool("JSON/FireFly.json", 12);
		}

		//Instantiate fly
		assert(FireFlyPool != nullptr);
		GameObject* firefly = FireFlyPool->GetPooledObject();
		assert(firefly != nullptr);
		AnimRenderAction* a = firefly->getAction("AnimRenderAction")->As<AnimRenderAction>();
		assert(a != nullptr);


		if (firefly->getAction("Context") == nullptr) {
			Context* fireFlyState = new Context(new FireflyLeft(), firefly, a);
			assert(fireFlyState != nullptr);
			firefly->addAction(fireFlyState, "Context");
		}
		else
		{
			int a = 5;
			firefly->getAction("Context")->As<Context>()->TransitionTo(new FireflyLeft());
		}

		firefly->transform.position = glm::vec4(570.0f, 450.0f, 0.0f, 0.0f);

		// Randomize the spit sounds
		int randomNumber = rand() % 6 + 0;

		switch (randomNumber)
		{
		case 0:
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogSpitShoot/AudioClip/sfx_frogs_tall_spit_shoot_01.wav", 1);
			break;
		case 1:
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogSpitShoot/AudioClip/sfx_frogs_tall_spit_shoot_02.wav", 1);
			break;
		case 2:
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogSpitShoot/AudioClip/sfx_frogs_tall_spit_shoot_03.wav", 1);
			break;
		case 3:
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogSpitShoot/AudioClip/sfx_frogs_tall_spit_shoot_04.wav", 1);
			break;
		case 4:
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogSpitShoot/AudioClip/sfx_frogs_tall_spit_shoot_05.wav", 1);
			break;
		case 5:
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogSpitShoot/AudioClip/sfx_frogs_tall_spit_shoot_06.wav", 1);
			break;
		default:
			break;
		}
	}
	
	if (_animRenderAction->isAnimComplete())
	{
		if (currentSpitVolley <= volleySpitLimit)
		{
			_context->TransitionTo(new TallFrogSpitReset(volleySpitLimit, currentSpitVolley));
			//_context->TransitionTo(new TallFrogSpit_Start(volleySpitLimit, currentSpitVolley));
		}
		else
		{
			_context->TransitionTo(new TallFrogSpit_End());
		}
	}
}

void Fiea::GameEngine::TallFrogSpitReset::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		++currentSpitVolley;
		_context->TransitionTo(new TallFrogSpit_Charge_Start(4.0f, volleySpitLimit, currentSpitVolley));

	}
}

void Fiea::GameEngine::TallFrogSpit_End::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogIdle(7.0f));
	}
}

Fiea::GameEngine::ObjectPool* Fiea::GameEngine::TallFrogFan_Start::windPool = nullptr;
Fiea::GameEngine::GameObject* Fiea::GameEngine::TallFrogFan_Start::wind = nullptr;

void Fiea::GameEngine::TallFrogFan_Start::update(const GameTime& time, GameObject* gameObject)
{
	if (windPool == nullptr)
	{
		windPool = new ObjectPool("JSON/WindPrefab.json", 1);
	}

	if(wind == nullptr)
	{
		wind = windPool->GetPooledObject();
		wind->transform.position = glm::vec4(500.0f, 300.0f, 0.0f, 0.0f);
	}
	else
	{
		wind->enabled = true;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogFan_Slow_Loop());
	}
}

void Fiea::GameEngine::TallFrogFan_Slow_Loop::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogFan_Fast_Loop());
	}
}

void Fiea::GameEngine::TallFrogFan_Fast_Loop::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogFan_Stop());
	}
}

void Fiea::GameEngine::TallFrogFan_Stop::update(const GameTime& time, GameObject* gameObject)
{
	if (TallFrogFan_Start::wind != nullptr)
	{
		TallFrogFan_Start::wind->enabled = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogIdle(7.0f));
	}
}

void Fiea::GameEngine::TallFrog_Abre::update(const GameTime& time, GameObject* gameObject)
{
	if (TallFrogFan_Start::wind != nullptr)
	{
		TallFrogFan_Start::wind->enabled = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrog_BootyQuake());
	}
}

void Fiea::GameEngine::TallFrog_BootyQuake::update(const GameTime& time, GameObject* gameObject)
{
	//if (_animRenderAction->isAnimComplete())
	//{
	//	_context->TransitionTo(new TallFrog_Morph());
	//}
}

void Fiea::GameEngine::TallFrog_Morph::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogMorphOpen());
	}
}

void Fiea::GameEngine::TallFrogMorphOpen::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogMorphIdle());
	}
}

void Fiea::GameEngine::TallFrogMorphIdle::update(const GameTime& time, GameObject* gameObject)
{
	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new TallFrogMorphAttack());
	}
}

void Fiea::GameEngine::TallFrogMorphAttack::update(const GameTime& time, GameObject* gameObject)
{

}

void Fiea::GameEngine::TallFrogMorphDie::update(const GameTime& time, GameObject* gameObject)
{
}
