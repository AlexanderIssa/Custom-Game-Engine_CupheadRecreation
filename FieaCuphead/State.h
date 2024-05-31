#pragma once
#include "Action.h"
#include "AnimRenderAction.h"
#include "Window.h"
#include "GameObject.h"
#include "GameClock.h"
#include "Collision.h"


//using namespace Fiea::GameEngine;

namespace Fiea::GameEngine
{
	class Context;

	/**
	 * @brief Interface for the state
	*/
	class State
	{
	protected:
		Context* _context;
		bool firstFrame = true;

	public:

		virtual ~State() {};

		void SetContext(Context* context)
		{
			_context = context;
		}

		virtual void update(const GameTime& time, GameObject* gameObject) = 0;

		std::string stateName;

		SpriteAnim animData;
		int frameRate;
		int nCols;
		int nRows;
		int cellWidth;
		int cellHeight;
		float currFrame;

		int height;
		int width;

		bool useStateMirrror = false;
		bool isMirror = false;

		int loopLimit = -1;


		AnimRenderAction* _animRenderAction;
		BoxCollider* _boxCollider;


	};



	class AnimRenderAction;


	/**
	 * @brief Context class
	*/
	class Context : public Action
	{
		RTTI_DECLARATIONS(Context, Action);


	protected:
		// Specialized constructor for child classes to set the type of the object
		Context(IdType type);

	private:
		State* _state;
		AnimRenderAction* _animRenderAction;
		BoxCollider* _boxCollider;

	public:

		static std::vector<Signature> prescribedAttributes();

		static bool registerSignatures();

		virtual std::string ToString() const override;

		AnimRenderAction* GetAnimRenderAction()
		{
			return _animRenderAction;
		}

		const std::string& GetStateName() const
		{
			return _state->stateName;
		}

		/**
		 * @brief Construct a new Context object
		 * @param state the initial state of the context
		*/
		Context(State* state, GameObject* obj,  AnimRenderAction* animRenderAction, BoxCollider* colliderAction = nullptr);
		Context(State* state, GameObject* obj);

		// Cloneable interface
		[[nodiscard]] virtual Context* clone() const override;

		/**
		 * @brief Destroy the Context object
		*/
		~Context();

		void lazyInitAnimRenderAction();

		void lazyInitParentGameObject();

		/**
		 * @brief transition to another state
		 * @param state the state to transition to
		 * 	
		*/
		void TransitionTo(State* state);

		State* GetState()
		{
			return _state;
		}

		void AttachGameWindow(Window* window);

		void AttachGameObject(GameObject* gameObject);

		void SetAnimRenderAction(AnimRenderAction* animRenderAction);

		void render();

		virtual void onEnable() override {};
		virtual void onDisable() override {};

		virtual void update(const GameTime& time) override;


	private:
		Window* _gameWindow;
		GameObject* _gameObject;

			
			
	};



	class WaiterLeft : public State
	{
	public:
		WaiterLeft()
		{
			stateName = "waiterLeft";

			animData.startingCell = 1;
			animData.nFrames = 8;
			frameRate = 10;
			nRows = 1;
			nCols = 8;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 194;
			width = 229;

			useStateMirrror = true;
			isMirror = false;

		}

		void update(const GameTime& time, GameObject* gameObject) override;

		bool moveRight = true;

	};



	class WaiterRight : public State
	{
	public:
		WaiterRight()
		{
			stateName = "waiterReft";

			animData.startingCell = 1;
			animData.nFrames = 8;
			frameRate = 10;
			nRows = 1;
			nCols = 8;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 194;
			width = 229;

			useStateMirrror = true;
			isMirror = true;
			
		}

		void update(const GameTime& time, GameObject* gameObject) override;

		bool moveRight = true;
	};

	class DancerGirls : public State
	{

	public:
		DancerGirls()
		{
			stateName = "dancerGirls";

			animData.startingCell = 1;
			animData.nFrames = 18;
			frameRate = 10;
			nRows = 1;
			nCols = 18;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 125;
			width = 165;
		}


		void update(const GameTime& time, GameObject* gameObject) override
		{

		}
	};

	class JazzCrowdA : public State
	{

	public:
		JazzCrowdA()
		{
			stateName = "jazzCrowdA";

			animData.startingCell = 1;
			animData.nFrames = 24;
			frameRate = 10;
			nRows = 1;
			nCols = 24;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 335;
			width = 844;
		}


		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class JazzCrowdB : public State
	{

	public:
		JazzCrowdB()
		{
			stateName = "JazzCrowdB";

			animData.startingCell = 1;
			animData.nFrames = 24;
			frameRate = 10;
			nRows = 1;
			nCols = 24;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 180;
			width = 432;
		}


		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};



}