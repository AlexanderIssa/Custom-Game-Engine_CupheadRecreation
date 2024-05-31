#pragma once
#include "Action.h"
#include "AnimRenderAction.h"
#include "AudioManager.h"
#include "Window.h"
#include "GameObject.h"
#include "GameClock.h"
#include "State.h"
#include "KeyDefinitions.h"
#include "CupheadEvents.h"
#include "Collision.h"


namespace Fiea::GameEngine
{
	class CupheadIdle : public State
	{

	public:
		/*
		CupheadIdle()
		{
			stateName = "cupheadIdle";

			animData.startingCell = 73;
			animData.nFrames = 10;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		CupheadIdle(bool hasJumped) : jumpExhausted(hasJumped)
		{
			stateName = "cupheadIdle";

			animData.startingCell = 73;
			animData.nFrames = 10;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}
		*/

		CupheadIdle(bool hasJumped = false, bool hasDashed = false) : jumpExhausted(hasJumped), canDash(!hasDashed) 
		{
			stateName = "cupheadIdle";

			animData.startingCell = 73;
			animData.nFrames = 10;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

		}

		CupheadIdle(BoxCollider* boxcollider) : jumpExhausted(false), canDash(true)
		{
			stateName = "cupheadIdle";

			animData.startingCell = 73;
			animData.nFrames = 10;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			_boxCollider = boxcollider;

			assert(_boxCollider != nullptr);
			_boxCollider->setHalfHeight(30);
			_boxCollider->setHalfWidth(15);
			_boxCollider->setPosOffset(glm::vec4(0,0,0,0));
		}
		


		void update(const GameTime& time, GameObject* gameObject) override;


		bool jumpExhausted = false;
		bool canDash = true;
		bool aiming = false;
		bool shooting = false;
	};

	class CupheadIntro_1 : public State
	{
	public:
		CupheadIntro_1()
		{
			stateName = "cupheadIntro_1";

			animData.startingCell = 1;
			animData.nFrames = 28;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadIntro_2 : public State
	{
	public:
		CupheadIntro_2()
		{
			stateName = "cupheadIntro_2";

			animData.startingCell = 29;
			animData.nFrames = 44;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}


	};

	class CupheadAim_Down : public State
	{
	public:
		CupheadAim_Down()
		{
			stateName = "cupheadAim_Down";

			animData.startingCell = 83;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadAim_Diag_Down : public State
	{
	public:
		CupheadAim_Diag_Down()
		{
			stateName = "cupheadAim_Diag_Down";

			animData.startingCell = 91;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadAim_Straight : public State
	{
	public:
		CupheadAim_Straight()
		{
			stateName = "cupheadAim_Straight";

			animData.startingCell = 99;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadAim_Diag_Up : public State
	{
	public:
		CupheadAim_Diag_Up()
		{
			stateName = "cupheadAim_Diag_Up";

			animData.startingCell = 107;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadAim_Up : public State
	{
	public:
		CupheadAim_Up()
		{
			stateName = "cupheadAim_Up";

			animData.startingCell = 115;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadShoot_Down : public State
	{
	public:
		CupheadShoot_Down()
		{
			stateName = "cupheadShoot_Down";

			animData.startingCell = 123;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadShoot_Diag_Down : public State
	{
	public:
		CupheadShoot_Diag_Down()
		{
			stateName = "cupheadShoot_Diag_Down";

			animData.startingCell = 126;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadShoot_Straight : public State
	{
	public:
		CupheadShoot_Straight()
		{
			stateName = "cupheadShoot_Straight";

			animData.startingCell = 129;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadShoot_Diag_Up : public State
	{
	public:
		CupheadShoot_Diag_Up()
		{
			stateName = "cupheadShoot_Diag_Up";

			animData.startingCell = 132;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadShoot_Up : public State
	{
	public:
		CupheadShoot_Up()
		{
			stateName = "cupheadShoot_Up";

			animData.startingCell = 135;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadRun : public State
	{
	public:
		/*
		CupheadRun()
		{
			stateName = "cupheadRun";

			animData.startingCell = 138;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		CupheadRun(bool idleJumpState) : IdleJumpState(idleJumpState)
		{
			stateName = "cupheadRun";

			animData.startingCell = 138;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}
		*/

		CupheadRun(bool idleJumpState = false, bool canDash = true) : IdleJumpState(idleJumpState), canDash(canDash)
		{
			stateName = "cupheadRun";

			animData.startingCell = 138;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


		bool IdleJumpState = false;
		bool canDash = true;
	};

	class CupheadRun_Shoot_Straight : public State
	{
	public:
		CupheadRun_Shoot_Straight()
		{
			stateName = "cupheadRun_Shoot_Straight";

			animData.startingCell = 154;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class CupheadRun_Shoot_Diag_Up : public State
	{
	public:
		CupheadRun_Shoot_Diag_Up()
		{
			stateName = "cupheadRun_Shoot_Diag_Up";

			animData.startingCell = 170;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadHit : public State
	{
	public:
		CupheadHit(bool isDead = false)
		{
			stateName = "cupheadHit";

			animData.startingCell = 186;
			animData.nFrames = 6;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
			_isDead = isDead;
			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

		bool _isDead;
	};

	class CupheadHit_Air : public State
	{
	public:
		CupheadHit_Air(bool isDead = false)
		{
			stateName = "cupheadHit_Air";

			animData.startingCell = 192;
			animData.nFrames = 6;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;
			_isDead = isDead;
			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
		bool _isDead;
	};

	class CupheadGhost : public State
	{
	public:
		CupheadGhost()
		{
			stateName = "cupheadGhost";

			animData.startingCell = 198;
			animData.nFrames = 24;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadJump : public State
	{
	public:
		/*
		CupheadJump()
		{
			stateName = "cupheadJump";

			animData.startingCell = 222;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			int randomNumber = rand() % 3 + 0;
		}

		CupheadJump(bool apexHit) : reachedApex(apexHit)
		{
			stateName = "cupheadJump";

			animData.startingCell = 222;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		CupheadJump(bool apexHit, bool canDash) : reachedApex(apexHit), canDash(canDash)
		{
			stateName = "cupheadJump";

			animData.startingCell = 222;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}
		*/

		CupheadJump(bool apexHit = false, bool canDash = true, float yVelocity = 0.0f) : reachedApex(apexHit), canDash(canDash), yVelocity(yVelocity)
		{
			stateName = "cupheadJump";

			animData.startingCell = 222;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


		float yVelocity = 0.0f;

		float jumpHeight = 250.0f;
		float gravity = 40.0f;
		bool reachedApex = false;

		bool canDash = true;

		bool canParry = false;

	};

	class CupheadParry : public State
	{
	public:
		CupheadParry()
		{
			stateName = "cupheadParry";

			animData.startingCell = 230;
			animData.nFrames = 7;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;
		}
		
		CupheadParry(bool apexHit, bool canDash, float yVelocity, float jumpHeight, float gravity) : reachedApex(apexHit), canDash(canDash), yVelocity(yVelocity), jumpHeight(jumpHeight), gravity(gravity)
		{
			stateName = "cupheadParry";

			animData.startingCell = 230;
			animData.nFrames = 7;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

		float yVelocity = 0.0f;

		float jumpHeight = 250.0f;
		float gravity = 40.0f;

		bool reachedApex = false;
		bool canDash = true;
	};

	class CupheadParry_Pink : public State
	{
	public:
		CupheadParry_Pink()
		{
			stateName = "cupheadParry_Pink";

			animData.startingCell = 237;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Down : public State
	{
	public:
		CupheadEx_Down()
		{
			stateName = "cupheadEx_Down";

			animData.startingCell = 245;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Diag_Down : public State
	{
	public:
		CupheadEx_Diag_Down()
		{
			stateName = "cupheadEx_Diag_Down";

			animData.startingCell = 260;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}
	};

	class CupheadEx_Straight : public State
	{
	public:
		CupheadEx_Straight()
		{
			stateName = "cupheadEx_Straight";

			animData.startingCell = 275;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Diag_Up : public State
	{
	public:
		CupheadEx_Diag_Up()
		{
			stateName = "cupheadEx_Diag_Up";

			animData.startingCell = 290;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Up : public State
	{
	public:
		CupheadEx_Up()
		{
			stateName = "cupheadEx_Up";

			animData.startingCell = 305;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Down_Air : public State
	{
	public:
		CupheadEx_Down_Air()
		{
			stateName = "cupheadEx_Down_Air";

			animData.startingCell = 320;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Diag_Down_Air : public State
	{
	public:
		CupheadEx_Diag_Down_Air()
		{
			stateName = "cupheadEx_Diag_Down_Air";

			animData.startingCell = 335;
			animData.nFrames = 14;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Straight_Air : public State
	{
	public:
		CupheadEx_Straight_Air()
		{
			stateName = "cupheadEx_Striaght_Air";

			animData.startingCell = 349;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Diag_Up_Air : public State
	{
	public:
		CupheadEx_Diag_Up_Air()
		{
			stateName = "cupheadEx_Diag_Up_Air";

			animData.startingCell = 364;
			animData.nFrames = 15;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}

	};

	class CupheadEx_Up_Air : public State
	{
	public:
		CupheadEx_Up_Air()
		{
			stateName = "cupheadEx_Up_Air";

			animData.startingCell = 379;
			animData.nFrames = 10;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override
		{

		}


	};

	class CupheadDuck : public State
	{
	public:
		CupheadDuck()
		{
			stateName = "CupheadDuck";

			animData.startingCell = 389;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;

			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadDuckIdle : public State
	{
	public:
		CupheadDuckIdle()
		{
			stateName = "CupheadDuckIdle";

			animData.startingCell = 397;
			animData.nFrames = 9;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadDuck_Shoot : public State
	{
	public:
		CupheadDuck_Shoot()
		{
			stateName = "CupheadDuck_Shoot";

			animData.startingCell = 406;
			animData.nFrames = 3;
			frameRate = 12;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class CupheadDash : public State
	{
	public:
		CupheadDash()
		{
			stateName = "CupheadDash";

			animData.startingCell = 409;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
		
		bool hasDashed = false;
	};

	class CupheadDashAir : public State
	{
	public:
		/*
		CupheadDashAir()
		{
			stateName = "CupheadDashAir";

			animData.startingCell = 417;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;
		}
		*/

		CupheadDashAir(bool reachedApex = false) : previousJumpState(reachedApex)
		{
			stateName = "CupheadDashAir";

				animData.startingCell = 417;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 11;
			nCols = 40;
			//cellWidth = 1;
			//cellHeight = 1;
			currFrame = 0;

			height = 450;
			width = 450;

			loopLimit = 1;

			// Event for begin dash
			EventDispatcher::enqueue(OnStartDashEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));
		}

		void update(const GameTime& time, GameObject* gameObject) override;


		bool previousJumpState = false;
	};
}