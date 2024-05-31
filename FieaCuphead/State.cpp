#include "State.h"

using namespace std::string_literals;

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(Context);


	Context::Context(IdType type) : Action(type)
	{
	}

	std::vector<Signature> Context::prescribedAttributes()
	{
		return std::vector<Signature>
		{
			{ "this"s, Datum::DataType::POINTER, 1, 0 } //,
			//{ "int_frameRate"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, frameRate) },
			//{ "int_nFrames"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, anim.nFrames) },
			//{ "int_nCols"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, nCols) },
			//{ "int_nRows"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, nRows) },
			//{ "int_cellWidth"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, cellWidth) },
			//{ "int_cellHeight"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, cellHeight) },
			//{ "float_currFrame"s, Datum::DataType::FLOAT, 1, offsetof(AnimRenderAction, currFrame) },
			//{ "int_loopCount"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, loopCount) },
			//{ "int_loopLimit"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, loopLimit) }
		};
	}

	bool Context::registerSignatures()
	{
		return false;
	}

	std::string Context::ToString() const
	{
		return std::string();
	}


	Context::Context(State* state, GameObject* obj, AnimRenderAction* animRenderAction, BoxCollider* boxColliderAction) : _state(nullptr), _animRenderAction(animRenderAction), _gameObject(obj), _boxCollider(boxColliderAction)
	{
		TransitionTo(state);
	}
	Context::Context(State* state, GameObject* obj) : _state(nullptr), _gameObject(obj)
	{
		if (_animRenderAction != nullptr)
		{
			TransitionTo(state);

		}
		else
		{
			_state = state;
			_state->SetContext(this);
		}
	}

	Context* Context::clone() const
	{
		return new Context(*this);
	}

	GameEngine::Context::~Context()
	{
		delete _state;
	}
	inline void Context::lazyInitAnimRenderAction()
	{
		if (_animRenderAction == nullptr)
		{
			_animRenderAction = getParentGameObject()->getAction("AnimRenderAction")->As<AnimRenderAction>();

			if (_animRenderAction == nullptr)
			{
				throw std::runtime_error("AnimRenderAction not found");
			}
		}
	}
	inline void Context::lazyInitParentGameObject()
	{
		if (_gameObject == nullptr)
		{
			_gameObject = getParentGameObject();
		}
	}
	void GameEngine::Context::TransitionTo(State* state)
	{
		lazyInitAnimRenderAction();

		if (_state != nullptr)
		{
			delete _state;
		}
		_state = state;
		_state->SetContext(this);
		_animRenderAction->resetLoopCount();
		_animRenderAction->resetAnimCompletion();
		_animRenderAction->setAnim(_state->animData, true);
		_state->_animRenderAction = _animRenderAction;
		_state->_boxCollider = _boxCollider;
		_animRenderAction->setCurrentFrame(_state->currFrame);


	}
	inline void Context::AttachGameWindow(Window* window)
	{
		_gameWindow = window;
	}
	inline void Context::AttachGameObject(GameObject* gameObject)
	{
		_gameObject = gameObject;
	}
	inline void Context::SetAnimRenderAction(AnimRenderAction* animRenderAction)
	{
		_animRenderAction = animRenderAction;
	}
	inline void Context::render()
	{
		lazyInitAnimRenderAction();

		_animRenderAction->setAnim(_state->animData);
		_animRenderAction->setFrameRate(_state->frameRate);
		_animRenderAction->setCols(_state->nCols);
		_animRenderAction->setRows(_state->nRows);
		_animRenderAction->setWidth(_state->width);
		_animRenderAction->setHeight(_state->height);

		if (_state->useStateMirrror)
		{
			_animRenderAction->setMirror(_state->isMirror);
		}

		_animRenderAction->setLoopLimit(_state->loopLimit);


	}
	inline void Context::update(const GameTime& time)
	{
		render();
		lazyInitAnimRenderAction();

		_state->update(time, _gameObject);

	}
	void WaiterLeft::update(const GameTime& time, GameObject* gameObject)
	{
		gameObject->transform.position.x -= 10.0f * time.Frame() * 0.01f;
		if (gameObject->transform.position.x <= -400)
		{
			_context->TransitionTo(new WaiterRight());
		}

		//if (gameObject->transform.position.x >= 800)
		//{
		//	//gameObject->position.x = 400;
		//	moveRight = false;

		//}
		//else if (gameObject->transform.position.x <= 0)
		//{
		//	//gameObject->position.x = 0;
		//	//moveRight = true;
		//}
	}
	void WaiterRight::update(const GameTime& time, GameObject* gameObject)
	{


		gameObject->transform.position.x += 10.0f * time.Frame() * 0.01f;
		if (gameObject->transform.position.x >= 1200)
		{
			_context->TransitionTo(new WaiterLeft());
		}
		//if (gameObject->transform.position.x >= 800)
		//{
		//	//gameObject->position.x = 400;
		//	//moveRight = false;

		//	_context->TransitionTo(new WaiterLeft());


		//}
		//else if (gameObject->transform.position.x <= 0)
		//{
		//	//gameObject->position.x = 0;
		//	//_context->TransitionTo(new WaiterRight());

		//}
	}
}


