#pragma once
#include "Action.h"
#include "AnimRenderAction.h"
#include "Window.h"
#include "GameObject.h"
#include "GameClock.h"
#include "State.h"
#include "KeyDefinitions.h"
#include "AudioManager.h"
#include "ObjectPool.h"
#include "FireflyStates.h"

#include <ctime>

namespace Fiea::GameEngine
{
	class TallFrogIntro : public State
	{

	public:
		TallFrogIntro()
		{
			stateName = "tallFrogIntro";

			animData.startingCell = 1;
			animData.nFrames = 24;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}


		void update(const GameTime& time, GameObject* gameObject) override;

	};
	
	class TallFrogIdle : public State
	{

	public:
		TallFrogIdle()
		{
			stateName = "TallFrogIdle";

			animData.startingCell = 25;
			animData.nFrames = 36;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;
		}

		TallFrogIdle(float newStartingFrame)
		{
			stateName = "TallFrogIdle";

			animData.startingCell = 25;
			animData.nFrames = 36;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = newStartingFrame;

			height = 750;
			width = 887;
		}


		void update(const GameTime& time, GameObject* gameObject) override;



	};

	class TallFrogSpit_Charge_Start : public State
	{

	public:
		TallFrogSpit_Charge_Start()
		{
			stateName = "TallFrogSpit_Charge_Start";

			animData.startingCell = 61;
			animData.nFrames = 23;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		TallFrogSpit_Charge_Start(float startingFrame, int SpitvolleyLimit, int currentSpitVolley) : volleySpitLimit(SpitvolleyLimit), currentSpitVolley(currentSpitVolley)
		{
			stateName = "TallFrogSpit_Charge_Start";

			animData.startingCell = 61;
			animData.nFrames = 23;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = startingFrame;

			height = 750;
			width = 887;

			loopLimit = 1;
		}


		void update(const GameTime& time, GameObject* gameObject) override;


		int volleySpitLimit = 4;
		int currentSpitVolley = 0;
	};

	class TallFrogSpit_Charge_Loop : public State
	{

	public:
		TallFrogSpit_Charge_Loop()
		{
			stateName = "TallFrogSpit_Charge_Loop";

			animData.startingCell = 84;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			// Generate a random number between 1 and 4
			int randomNumber = rand() % 15 + 10;
			loopLimit = randomNumber;
		}

		TallFrogSpit_Charge_Loop(int volleySpitLimit, int currentSpitVolley) : volleySpitLimit(volleySpitLimit), currentSpitVolley(currentSpitVolley)
		{
			stateName = "TallFrogSpit_Charge_Loop";

			animData.startingCell = 84;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			// Generate a random number between 1 and 4
			int randomNumber = rand() % 15 + 10;
			loopLimit = randomNumber;
		}


		void update(const GameTime& time, GameObject* gameObject) override;

		int volleySpitLimit = 0;
		int currentSpitVolley = 0;
	};

	class TallFrogSpit_Start : public State
	{

	public:
		TallFrogSpit_Start()
		{
			stateName = "TallFrogSpit_Start";

			animData.startingCell = 88;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;


		}

		TallFrogSpit_Start(int SpitvolleyLimit, int currentSpitVolley) : volleySpitLimit(SpitvolleyLimit), currentSpitVolley(currentSpitVolley)
		{
			stateName = "TallFrogSpit_Start";

			animData.startingCell = 88;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;

		}


		void update(const GameTime& time, GameObject* gameObject) override;

		int volleySpitLimit = 0;
		int currentSpitVolley = 0;
	};

	class TallFrogSpit_Loop : public State
	{
		
	public:
		static ObjectPool* FireFlyPool;

		TallFrogSpit_Loop()
		{
			stateName = "TallFrogSpit_Loop";

			animData.startingCell = 92;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			// Generate a random number between 1 and 4
			int randomNumber = rand() % 4 + 1;
			loopLimit = randomNumber;
		}


		TallFrogSpit_Loop(int SpitvolleyLimit, int currentSpitVolley) : volleySpitLimit(SpitvolleyLimit), currentSpitVolley(currentSpitVolley)
		{
			stateName = "TallFrogSpit_Loop";

			animData.startingCell = 92;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			// Generate a random number between 1 and 4
			int randomNumber = rand() % 4 + 1;
			loopLimit = randomNumber;		}
		void update(const GameTime& time, GameObject* gameObject) override;


		int volleySpitLimit = 0;
		int currentSpitVolley = 0;

		bool playedSound = false;
	};

	class TallFrogSpitReset : public State
	{

	public:
		TallFrogSpitReset()
		{
			stateName = "TallFrogSpitReset";

			animData.startingCell = 100;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;


		}

		TallFrogSpitReset(int SpitvolleyLimit, int currentSpitVolley) : volleySpitLimit(SpitvolleyLimit), currentSpitVolley(currentSpitVolley)
		{
			stateName = "TallFrogSpitReset";

			animData.startingCell = 100;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;


		}

		void update(const GameTime& time, GameObject* gameObject) override;


		int volleySpitLimit = 0;
		int currentSpitVolley = 0;
	};

	class TallFrogSpit_End : public State
	{

	public:
		TallFrogSpit_End()
		{
			stateName = "TallFrogSpit_End";

			animData.startingCell = 103;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;


		}


		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class TallFrogFan_Start : public State
	{

	public:
		TallFrogFan_Start()
		{
			stateName = "TallFrogFan_Start";

			animData.startingCell = 106;
			animData.nFrames = 24;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		static ObjectPool* windPool;
		static GameObject* wind;

		void update(const GameTime& time, GameObject* gameObject) override;

	};



	class TallFrogFan_Slow_Loop : public State
	{

	public:
		TallFrogFan_Slow_Loop()
		{
			stateName = "TallFrogFan_Slow_Loop";

			animData.startingCell = 130;
			animData.nFrames = 6;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 3;


		}


		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class TallFrogFan_Fast_Loop : public State
	{

	public:
		TallFrogFan_Fast_Loop()
		{
			stateName = "TallFrogFan_Fast_Loop";

			animData.startingCell = 136;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 30;


		}


		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class TallFrogFan_Stop : public State
	{

	public:
		TallFrogFan_Stop()
		{
			stateName = "TallFrogFan_Stop";

			animData.startingCell = 139;
			animData.nFrames = 14;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;


		}


		void update(const GameTime& time, GameObject* gameObject) override;


	};

	class TallFrog_Abre : public State
	{
	public:
		TallFrog_Abre()
		{
			stateName = "TallFrog_Abre";

			animData.startingCell = 153;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class TallFrog_BootyQuake : public State
	{
	public:
		TallFrog_BootyQuake()
		{
			stateName = "TallFrog_BootyQuake";

			animData.startingCell = 157;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class TallFrog_Morph : public State
	{
	public:
		TallFrog_Morph()
		{
			stateName = "TallFrog_Morph";

			animData.startingCell = 160;
			animData.nFrames = 25;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class TallFrogMorphOpen : public State
	{
	public:
		TallFrogMorphOpen()
		{
			stateName = "TallFrogMorphOpen";

			animData.startingCell = 188;
			animData.nFrames = 5;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class TallFrogMorphIdle : public State
	{
	public:
		TallFrogMorphIdle()
		{
			stateName = "TallFrogMorphIdle";

			animData.startingCell = 185;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class TallFrogMorphAttack : public State
	{
	public:
		TallFrogMorphAttack()
		{
			stateName = "TallFrogMorphAttack";

			animData.startingCell = 193;
			animData.nFrames = 6;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class TallFrogMorphDie : public State
	{
	public:
		TallFrogMorphDie()
		{
			stateName = "TallFrogMorphDie";

			animData.startingCell = 231;
			animData.nFrames = 5;
			frameRate = 24;
			nRows = 16;
			nCols = 15;
			currFrame = 0;

			height = 750;
			width = 887;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};
}