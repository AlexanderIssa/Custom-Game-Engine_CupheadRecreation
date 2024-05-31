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

#include <ctime>
#include <cstdlib>

namespace Fiea::GameEngine
{
	class ShortFrogIntro : public State
	{
	public:
		ShortFrogIntro()
		{
			stateName = "shortFrogIntro";

			animData.startingCell = 1;
			animData.nFrames = 37;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		ShortFrogIntro(BoxCollider* boxcollider)
		{
			stateName = "shortFrogIntro";

			animData.startingCell = 1;
			animData.nFrames = 37;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;

			_boxCollider = boxcollider;

			assert(_boxCollider != nullptr);
			_boxCollider->setHalfHeight(150);
			_boxCollider->setHalfWidth(75);
			_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));
		}

		void update(const GameTime& time, GameObject* gameObject) override;

	};

	class ShortFrogIntroShake : public State
	{
	public:
		ShortFrogIntroShake()
		{
			stateName = "ShortFrogIntroShake";

			animData.startingCell = 1;
			animData.nFrames = 37;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;
		}

		void update(const GameTime& time, GameObject* gameObject) override {}


	};

	class ShortFrogIdle : public State
	{
	public:

		ShortFrogIdle()
		{
			stateName = "ShortFrogIdle";

			animData.startingCell = 38;
			animData.nFrames = 36;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;
		}
		
		ShortFrogIdle(float startingFrame)
		{
			stateName = "ShortFrogIdle";

			animData.startingCell = 38;
			animData.nFrames = 36;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = startingFrame;

			height = 690;
			width = 525;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};

	class ShortFrogLiftLeg : public State
	{
	public:
		ShortFrogLiftLeg()
		{
			stateName = "ShortFrogLiftLeg";

			animData.startingCell = 74;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
	
	class ShortFrogChargeLeg : public State
	{
	public:
		ShortFrogChargeLeg()
		{
			stateName = "ShortFrogChargeLeg";

			animData.startingCell = 90;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 20;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
		
	class ShortFrogArmSpinStart : public State
	{
	public:
		ShortFrogArmSpinStart()
		{
			stateName = "ShortFrogArmSpinStart";

			animData.startingCell = 93;
			animData.nFrames = 7;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
			
	class ShortFrogArmSpinLoop : public State
	{
	public:
		ShortFrogArmSpinLoop()
		{
			stateName = "ShortFrogArmSpinLoop";

			animData.startingCell = 100;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 15;
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogRageFist/AudioClip/sfx_frogs_short_ragefist_attack_loop_01.wav", 1);

			currIndex = rand() % 4;
		}

		void update(const GameTime& time, GameObject* gameObject) override;

		float projectileDelay = 0.7f;
		float projectileTimer = 0.7f;

		float pattern[4] = { 0, 60, 0, -60 };
		int currIndex = 0;

		static ObjectPool* fistPool;
	};
				
	class ShortFrogArmSpinEnd : public State
	{
	public:
		ShortFrogArmSpinEnd()
		{
			stateName = "ShortFrogArmSpinEnd";

			animData.startingCell = 108;
			animData.nFrames = 9;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
					
	class ShortFrogStanceUp : public State
	{
	public:
		ShortFrogStanceUp()
		{
			stateName = "ShortFrogStanceUp";

			animData.startingCell = 117;
			animData.nFrames = 9;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
						
	class ShortFrogShakeBooty : public State
	{
	public:
		ShortFrogShakeBooty()
		{
			stateName = "ShortFrogShakeBooty";

			animData.startingCell = 126;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 10;
		}

		void update(const GameTime& time, GameObject* gameObject) override;



	};
							
	class ShortFrogRollStart : public State
	{
	public:
		ShortFrogRollStart()
		{
			stateName = "ShortFrogRollStart";

			animData.startingCell = 129;
			animData.nFrames = 5;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};

	class ShortFrogRollInPlace : public State
	{
	public:
		ShortFrogRollInPlace()
		{
			stateName = "ShortFrogRollInPlace";

			animData.startingCell = 135;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 6;

			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogRolling/AudioClip/sfx_frogs_short_rolling_loop_01.wav", 1);

		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
	
	class ShortFrogRollToEndOfScreen : public State
	{
	public:
		ShortFrogRollToEndOfScreen()
		{
			stateName = "ShortFrogRollInPlace";

			animData.startingCell = 135;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogRolling/AudioClip/sfx_frogs_short_rolling_start_01.wav", 1);

		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};

	class ShortFrogOffScreen : public State
	{
	public:
		ShortFrogOffScreen()
		{
			stateName = "ShortFrogOffScreen";

			animData.startingCell = 143;
			animData.nFrames = 16;
			frameRate = 1;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogRolling/AudioClip/sfx_frogs_short_rolling_crash_01.wav", 1);

		}

		void update(const GameTime& time, GameObject* gameObject) override;



		long long startTime = 0;
		bool timeGrabbed = false;

	};


	class ShortFrogRollReenterScreen : public State
	{
	public:
		ShortFrogRollReenterScreen()
		{
			stateName = "ShortFrogRollReenterScreen";

			animData.startingCell = 143;
			animData.nFrames = 16;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;

			//AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogRolling/AudioClip/sfx_frogs_short_rolling_crash_01.wav", 1);
			AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogRolling/AudioClip/sfx_frogs_short_rolling_end.wav", 1);


		}

		void update(const GameTime& time, GameObject* gameObject) override;


	};
	
	class ShortFrogIdleToClapCharge : public State
	{
	public:
		ShortFrogIdleToClapCharge(int currentClap) : currentClap(currentClap)
		{
			stateName = "ShortFrogIdleToClapCharge";

			animData.startingCell = 159;
			animData.nFrames = 9;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;

		}

		void update(const GameTime& time, GameObject* gameObject) override;


		int maxClaps = 3;
		int currentClap = 0;

	};	

	class ShortFrogClapCharge : public State
	{
	public:
		ShortFrogClapCharge(int currentClap) : currentClap(currentClap)
		{
			stateName = "ShortFrogClapCharge";

			animData.startingCell = 168;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 20;

		}

		void update(const GameTime& time, GameObject* gameObject) override;

		int maxClaps = 3;
		int currentClap = 0;

	};

	class ShortFrogClap : public State
	{
	public:
		ShortFrogClap(int currentClap) : currentClap(currentClap)
		{
			stateName = "ShortFrogClap";

			animData.startingCell = 172;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;

			// Randomize the clap sounds
			int randomNumber = rand() % 4 + 0;
			
			switch (randomNumber)
			{
			case 0:
				AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogClapShock/AudioClip/sfx_frogs_short_clap_shock_01.wav", 1);
				break;
			case 1:
				AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogClapShock/AudioClip/sfx_frogs_short_clap_shock_02.wav", 1);
				break;
			case 2:
				AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogClapShock/AudioClip/sfx_frogs_short_clap_shock_03.wav", 1);
				break;
			case 3:
				AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/Short Frog/ShortFrogClapShock/AudioClip/sfx_frogs_short_clap_shock_04.wav", 1);
				break;
			default:
				break;
			}

		}

		void update(const GameTime& time, GameObject* gameObject) override;


		int maxClaps = 3;
		int currentClap = 0;

	private:
		static inline ObjectPool* planetPool;

		float projectileDelay = 0.7f;
		float projectileTimer = 0.7f;
	};
		
	class ShortFrogClapToClapCharge: public State
	{
	public:
		ShortFrogClapToClapCharge(int currentClap) : currentClap(currentClap)
		{
			stateName = "ShortFrogClapToClapCharge";

			animData.startingCell = 175;
			animData.nFrames = 5;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;

		}

		void update(const GameTime& time, GameObject* gameObject) override;


		int maxClaps = 3;
		int currentClap = 0;

	};
			
	class ShortFrogClapToIdleTransition : public State
	{
	public:
		ShortFrogClapToIdleTransition(int currentClap) : currentClap(currentClap)
		{
			stateName = "ShortFrogClapToIdleTransition";

			animData.startingCell = 180;
			animData.nFrames = 4;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;

		}

		void update(const GameTime& time, GameObject* gameObject) override;


		int maxClaps = 3;
		int currentClap = 0;

	};

	class ShortFrogFinalStanceUpStart : public State
	{
	public:
		ShortFrogFinalStanceUpStart()
		{
			stateName = "ShortFrogFinalStanceUpStart";

			animData.startingCell = 117;
			animData.nFrames = 9;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};
				

	class ShortFrogFinalBootyShake : public State
	{
	public:
		ShortFrogFinalBootyShake()
		{
			stateName = "ShortFrogFinalBootyShake";

			animData.startingCell = 126;
			animData.nFrames = 3;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 5;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};


	class ShortFrogFinalRollStart : public State
	{
	public:
		ShortFrogFinalRollStart()
		{
			stateName = "ShortFrogFinalRollStart";

			animData.startingCell = 129;
			animData.nFrames = 5;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

			loopLimit = 1;
		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};

	class ShortFrogFinalRollOut : public State
	{
	public:
		ShortFrogFinalRollOut()
		{
			stateName = "ShortFrogFinalRollOut";

			animData.startingCell = 135;
			animData.nFrames = 8;
			frameRate = 24;
			nRows = 5;
			nCols = 40;
			currFrame = 0;

			height = 690;
			width = 525;

		}

		void update(const GameTime& time, GameObject* gameObject) override;
	};


}