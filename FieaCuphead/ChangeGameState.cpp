


#include "Signature.h"
#include "TypeManager.h"
#include <vector>
#include "string.h"
#include "InputManager.h"
#include "ChangeGameState.h"

#include "cupheadStates.h"
#include "TallFrogStates.h"
#include "ShortFrogStates.h"

#include "DamageReciever.h"

using namespace std::string_literals;


namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(ChangeGameState);

	ChangeGameState::ChangeGameState()
	{
		auto boundDamageFunc = BIND_FUNC(this, ChangeGameState::OnDamageTaken);
		damageCallback = new EventCallback(boundDamageFunc);
		EventDispatcher::addListener<OnDamageEvent>(damageCallback);

		cupheadDamageEvent = new OnCupheadTakeDamageEvent();

		//_gameState = new IntroState(this);
		_gameState = nullptr;
	}

	void ChangeGameState::StartGame()
	{
		_gameState = new GameBeginState(this);
	}

	ChangeGameState::ChangeGameState(GameObject* parent) : _parentGameObject(parent)
	{
		auto boundDamageFunc = BIND_FUNC(this, ChangeGameState::OnDamageTaken);
		damageCallback = new EventCallback(boundDamageFunc);
		EventDispatcher::addListener<OnDamageEvent>(damageCallback);

		cupheadDamageEvent = new OnCupheadTakeDamageEvent();

		_gameState = new IntroState(this);
	}

	ChangeGameState::~ChangeGameState()
	{
		EventDispatcher::removeListener<OnDamageEvent>(damageCallback);
		delete damageCallback;
		delete cupheadDamageEvent;
		delete _gameState;
	}

	ChangeGameState* ChangeGameState::clone() const
	{
		return new ChangeGameState(*this);
	}

	std::vector<Signature> ChangeGameState::prescribedAttributes()
	{
		return std::vector<Signature>
		{
			{ "this"s, Datum::DataType::POINTER, 1, 0 }
		};
	}
	bool ChangeGameState::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}
	std::string ChangeGameState::ToString() const
	{
		return std::string();
	}
	void ChangeGameState::onEnable()
	{
	}
	void ChangeGameState::update(const GameTime& time)
	{
		assert(_gameState != nullptr);
		_gameState->update(time, _parentGameObject);

		//if (_frogHealth >= 1292) //Phase 1
		//{
		//	//check if either of the frogs are attacking
		//	getShortFrog();
		//	getTallFrog();

		//	if (!timeGrabbed)
		//	{
		//		startTime = time.Get();
		//		timeGrabbed = true;
		//	}

		//	//wait for a couple seconds
		//	int secondsToWait = 1;

		//	if (phase1ShortFrogAttacking)
		//	{
		//		if (_shortFrogContext->GetStateName() == "ShortFrogIdle")
		//		{
		//			phase1ShortFrogAttacking = !phase1ShortFrogAttacking;
		//		}
		//	}

		//	if (time.Get() - startTime > secondsToWait * 1000)
		//	{
		//		startTime = time.Get();

		//		if (phase1ShortFrogAttacking)
		//		{
		//			if (_shortFrogContext->GetStateName() == "ShortFrogIdle")
		//			{
		//				_shortFrogContext->TransitionTo(new ShortFrogLiftLeg());
		//			}



		//		}
		//		else
		//		{
		//			if (_tallFrogContext->GetStateName() == "TallFrogIdle" && !_tallFrogContext->GetAnimRenderAction()->isAnimComplete())
		//			{
		//				_tallFrogContext->TransitionTo(new TallFrogSpit_Charge_Start());
		//			}

		//			if (_tallFrogContext->GetAnimRenderAction()->isAnimComplete())
		//			{
		//				phase1ShortFrogAttacking = !phase1ShortFrogAttacking;
		//			}
		//		}
		//		timeGrabbed = false;

		//	}



			//	//_shortFrogContext->TransitionTo(new ShortFrogStanceUp());

			//	//randomly choose a frog to attack

			//	
			//}
			//else if (_frogHealth < 1292) //Phase 2
			//{
			//	if (!phase2Started)
			//	{
			//		getShortFrog();
			//		_shortFrogContext->TransitionTo(new ShortFrogStanceUp());

			//		getTallFrog();
			//		_tallFrogContext->TransitionTo(new TallFrogFan_Start());

			//		phase2Started = true;
			//	}

			//}
			//else if (_frogHealth < 578) // Phase 3
			//{

			//}
			//else if (_frogHealth <= 0) // Death
			//{
		//}

		//if (InputManager::GetKeyDown(VK_O))
		//{
		//	--_frogHealth;
		//}


		//if (InputManager::GetKeyDown(VK_P))
		//{
		//	getShortFrog();
		//	_shortFrogContext->TransitionTo(new ShortFrogIntro());

		//	getTallFrog();
		//	_tallFrogContext->TransitionTo(new TallFrogIntro());

		//	getCuphead();
		//	_cupheadContext->TransitionTo(new CupheadIntro_1());
		//}
	}

	void ChangeGameState::setGameState(gameState* incomingState)
	{
		if (_gameState != nullptr)
		{
			delete _gameState;
		}
		_gameState = incomingState;
	}

	void ChangeGameState::setAllToTaunt()
	{
		getShortFrog();
		_shortFrogContext->TransitionTo(new ShortFrogIntro());

		getTallFrog();
		_tallFrogContext->TransitionTo(new TallFrogIntro());

		getCuphead();
		_cupheadContext->TransitionTo(new CupheadIntro_1());
	}

	void ChangeGameState::setAllToIdle()
	{
		getShortFrog();
		_shortFrogContext->TransitionTo(new ShortFrogIdle());

		getTallFrog();
		_tallFrogContext->TransitionTo(new TallFrogIdle());

		getCuphead();
		_cupheadContext->TransitionTo(new CupheadIdle());
	}
	void ChangeGameState::playShortFrogPhase1Attack()
	{
		if (_shortFrogContext == nullptr)
		{
			getShortFrog();
		}
		_shortFrogContext->TransitionTo(new ShortFrogLiftLeg());
	}
	void ChangeGameState::playTallFrogPhase1Attack()
	{
		if (_tallFrogContext == nullptr)
		{
			getTallFrog();
		}
		_tallFrogContext->TransitionTo(new TallFrogSpit_Charge_Start());
	}
	void ChangeGameState::playShortFrogPhase2Transition()
	{
		if (_shortFrogContext == nullptr)
		{
			getShortFrog();
		}
		_shortFrogContext->TransitionTo(new ShortFrogStanceUp());
	}
	void ChangeGameState::playShortFrogPhase2Attack()
	{
		if (_shortFrogContext == nullptr)
		{
			getShortFrog();
		}
		_shortFrogContext->TransitionTo(new ShortFrogIdleToClapCharge(0));
	}

	void ChangeGameState::playTallFrogPhase2Attack()
	{
		if (_tallFrogContext == nullptr)
		{
			getTallFrog();
		}
		_tallFrogContext->TransitionTo(new TallFrogFan_Start());
	}
	void ChangeGameState::playTallFrogPhase3Transition()
	{
		if (_tallFrogContext == nullptr)
		{
			getTallFrog();
		}
		_tallFrogContext->TransitionTo(new TallFrog_Abre());
	}
	void ChangeGameState::playShortFrogPhase3Transition()
	{
		if (_shortFrogContext == nullptr)
		{
			getShortFrog();
		}
		_shortFrogContext->TransitionTo(new ShortFrogFinalStanceUpStart());
	}
	void ChangeGameState::playTallFrogTransformation()
	{
		if (_tallFrogContext == nullptr)
		{
			getTallFrog();
		}
		_tallFrogContext->TransitionTo(new TallFrog_Morph());
	}
	void ChangeGameState::playFrogDeath()
	{
		if (_tallFrogContext == nullptr)
		{
			getTallFrog();
		}
		_tallFrogContext->TransitionTo(new TallFrogMorphDie());
	}
	void ChangeGameState::despawnShortFrog()
	{
		//EventDispatcher::removeListener<OnDamageEvent>(damageCallback);

		//getShortFrog()->enabled = false;
		getShortFrog()->transform.position.x = 1000;
	}
	GameObject* ChangeGameState::getShortFrog()
	{
		if (_shortFrog == nullptr)
		{
			_shortFrog = getParentGameObject()->getChild("GameObject_ShortFrog");
			_shortFrogContext = _shortFrog->getAction("Context")->As<Context>();
		}
		
		return _shortFrog;
		
	}

	GameObject* ChangeGameState::getTallFrog()
	{
		if (_tallFrog == nullptr)
		{
			_tallFrog = getParentGameObject()->getChild("GameObject_TallFrog");
			_tallFrogContext = _tallFrog->getAction("Context")->As<Context>();
		}

		return _tallFrog;
	}

	GameObject* ChangeGameState::getCuphead()
	{
		if (_cuphead == nullptr)
		{
			_cuphead = getParentGameObject()->getChild("GameObject_Cuphead");
			_cupheadContext = _cuphead->getAction("Context")->As<Context>();

		}

		return _cuphead;
	
	}

	bool ChangeGameState::canStartPhase2()
	{
		return _frogHealth < 1292;
	}

	bool ChangeGameState::casnStartPhase3()
	{
		return _frogHealth < 578;
	}

	bool ChangeGameState::isShortFrogPhase2TransitionOver()
	{
		getShortFrog();
		return _shortFrogContext->GetStateName() == std::string("ShortFrogIdle");
	}

	void ChangeGameState::OnDamageTaken(IEvent* eventData)
	{
		if (eventData == nullptr)
		{
			throw std::runtime_error("eventData cannot be null.");
		}

		OnDamageEvent* data = eventData->As<OnDamageEvent>();
		if (data == nullptr)
		{
			throw std::runtime_error("Invalid event data type.");
		}

		if (data->hit  == getShortFrog() || data->hit == getTallFrog())
		{
			OnBossDamageTaken(data->damage);
		}
		else if (data->hit == getCuphead())
		{
			--_cupheadHealth;
			cupheadDamageEvent->health = _cupheadHealth;
			EventDispatcher::enqueue(OnCupheadTakeDamageEvent::TypeIdClass(), cupheadDamageEvent, CreateElapsedTime(0));

			getCuphead();
			if (_cupheadHealth > 0) {
				if (getCuphead()->transform.position.y == 125.0f)
					_cupheadContext->TransitionTo(new CupheadHit());
				else
					_cupheadContext->TransitionTo(new CupheadHit_Air());
			}
			else if(_cupheadHealth == 0)
			{
				EventDispatcher::enqueue(OnCupheadDieEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
				if (getCuphead()->transform.position.y == 125.0f)
					_cupheadContext->TransitionTo(new CupheadHit(true));
				else
					_cupheadContext->TransitionTo(new CupheadHit_Air(true));
			}
		}
	}

	bool ChangeGameState::isShortFrogPhase1Attacking()
	{
		getShortFrog();
		return _shortFrogContext->GetStateName() != std::string("ShortFrogIdle");
	}

	bool ChangeGameState::isTallFrogPhase1Attacking()
	{
		getTallFrog();
		return _tallFrogContext->GetStateName() != std::string("TallFrogIdle");
	}

	bool ChangeGameState::isShortFrogPhase2Attacking()
	{
		getShortFrog();
		return _shortFrogContext->GetStateName() != std::string("ShortFrogIdle");
	}

	bool ChangeGameState::isTallFrogPhase2Attacking()
	{
		getTallFrog();
		return _tallFrogContext->GetStateName() != std::string("TallFrogIdle");
	}

	bool ChangeGameState::canEnterFinalPhaseLoop()
	{
		return getShortFrog()->transform.position.x >= getTallFrog()->transform.position.x - 175;
	}

	bool ChangeGameState::areAllFrogsDead()
	{
		return _frogHealth <= 0;
	}


	void ChangeGameState::OnBossDamageTaken(int damage)
	{
		_frogHealth -= damage;
	}
}
