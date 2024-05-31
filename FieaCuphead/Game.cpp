#include "Game.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "AudioManager.h"
#include "GameObject.h"
#include "TypeManager.h"
#include "AnimRenderAction.h"
#include "HealthRenderAction.h"
#include "InputManager.h"
#include "State.h"
#include "KeyDefinitions.h"

#include "CollisionManager.h"
#include "CombatAction.h"
#include "DamageReciever.h"
#include "ProjectileAction.h"
#include "DamageDealer.h"
#include "WeaponAction.h"
#include "ParticleManager.h"

#include "ChangeGameState.h"

#include "cupheadStates.h"
#include "TallFrogStates.h"
#include "ShortFrogStates.h"
#include "FireflyStates.h"
#include "ShortFrogBallStates.h"
#include "ShortFrogFistStates.h"
#include "WindAction.h"

#include "WorldCreator.h"
#include "GameObjectManager.h"
#include "ObjectPool.h"
#include "GravityAction.h"

#include "Reaction.h"
#include "DispatchEventAction.h"
#include "PlaySFXAction.h"

#include "CupheadEvents.h"

#include <cstdlib>
#include <ctime>

namespace Fiea::GameEngine
{
#pragma region Public Methods
	void Game::Run()
	{
		// Initialize the game
		Init();
		
		// Game World
		WorldCreator worldCreator;
		GameObject* world = worldCreator.CreateTheWorld();
		GameObjectManager::SetWorld(world);
		ParticleManager::Init(std::string("../FieaCuphead/JSON/ParticlePrefab.json"));

		RenderManager::Instance()->sortRenderActions();

		// Get the clock
		GameClock clock;
		GameTime time = clock.Current();

		LoadResources();

		// *****************************************
		// CHARACTERS
		GameObject* npcs = world->getChild("GameObject_NPCS", "NPCS");

		GameObject* gameManager = world->getChild("GameObject_GameManager", "GameManager");

		// EXTREMELY cursed code to get changegamestate. This will break if ChangeGameState is not the first action of GameManager
		Scope* temp = gameManager->find("obj_actions")->get<Scope*>(0);
		ChangeGameState* changeGameState = temp->find("ChangeGameState")->get<Scope*>(0)->As<ChangeGameState>();

		// Waiter
		GameObject* waiter = npcs->getChild("GameObject_Waiter", "Waiter");
		Context* waiterState = new Context(new WaiterLeft(), waiter);
		waiter->addAction(waiterState);

		// Tallfrog
		GameObject* tallFrog = gameManager->getChild("GameObject_TallFrog", "GameObject_TallFrog");
		Action* tallFrogAction = tallFrog->getAction("AnimRenderAction");
		AnimRenderAction* tallFrogRenderAction2 = tallFrogAction->As<AnimRenderAction>();

		Context* tallFrogState = new Context(new TallFrogIntro(), tallFrog, tallFrogRenderAction2);
		tallFrog->addAction(tallFrogState, "Context");
		//gameManager->addChild(tallFrog, "GameObject_TallFrog");

		//Short Frog
		GameObject* shortFrog = gameManager->getChild("GameObject_ShortFrog", "GameObject_ShortFrog");
		Action* shortFrogAction = shortFrog->getAction("AnimRenderAction");
		AnimRenderAction* shortFrogRenderAction = shortFrogAction->As<AnimRenderAction>();

		//gameManager->addChild(shortFrog, "GameObject_ShortFrog");
		BoxCollider* shortFrogCollider = shortFrog->getAction("BoxCollider")->As<BoxCollider>();
		assert(shortFrogCollider != nullptr);
		Context* shortFrogState = new Context(new ShortFrogIntro(shortFrogCollider), shortFrog, shortFrogRenderAction, shortFrogCollider);
		shortFrog->addAction(shortFrogState, "Context");

		// Buphead
		GameObject* buphead = gameManager->getChild("GameObject_Cuphead", "Cuphead");
		Action* cupheadRA = buphead->getAction("AnimRenderAction");
		AnimRenderAction* cupheadRenderAction = cupheadRA->As<AnimRenderAction>();

		//gameManager->addChild(buphead, buphead->name);
		BoxCollider* cupheadCollider = buphead->getAction("BoxCollider")->As<BoxCollider>();
		assert(cupheadCollider != nullptr);
		Context* cupheadState = new Context(new CupheadIdle(cupheadCollider), buphead, cupheadRenderAction, cupheadCollider);
		buphead->addAction(cupheadState, "Context");
		// END CHARACTERS
		// *****************************************

		//input
		InputManager::SetupKeyInputs(window);

		// TESTING AUDIO
		size_t bgm = AudioManager::RegisterBGM("../../Audio Assets/Frog Level/BGM/AudioClip/MUS_Frogs.wav");
		AudioManager::StartBGM(bgm, 0.1f);

		// Setup Random Number Generator
		time_t t(0);
		srand((unsigned) t); //Init randomizations

		// Game loop
		changeGameState->StartGame();
		while (!window.shouldClose())
		{

			clock.Update(time);
			Update(time);
			//frogFist->update(time);

			/*--------------------------------------------------------update---------------------------------------------------*/

			PollingEvent();


			CollisionManager::processCollisions(time);
			RenderManager::Instance()->Update(time);

			// Update events
			EventDispatcher::update(time);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			/*------------------------------------------------------Render-------------------------------------------------------*/
			Render();

			/*------------------------------------------------------Audio-------------------------------------------------------*/
			AudioManager::UpdateBGMBuffersEachFrame();

		}

		// Clean up the game
		Cleanup();
	}
#pragma endregion

#pragma region Private Methods 

	/*
	* Initialize the game
	*/
	void Game::Init()
	{
		RegisterSignatures();

		window.createWindow(std::string("Cuphead"), 1920, 1080);
		RenderManager::Instance()->Init(window.getWindow());
		CollisionManager::Init();
		InputManager::SetupKeyInputs(window);
		AudioManager::InitializeAudioManager();
		//ParticleManager::Init(std::string("blah"));
	}

	/*
	* Load resources for the game
	*/
	void Game::LoadResources()
	{
		GLuint VAO = 0, VBO = 0, EBO = 0;

		GLuint testTexture;
		CTexture tex2 = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Player/Cupehead Idle/Sprite/cuphead_idle_0001.png", testTexture);
		GLuint testTexture2;
		CTexture tex3 = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Player/Cupehead Idle/Sprite/cuphead_idle_0001-sheet.png", testTexture2);

		// ******************************************
		// LOAD TEXTURES
		// Background
		//Layer E
		GLuint bgLayerETexture;
		CTexture bgLayerETex = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Background/Background Layers/Sprite/jazzbar(E)_booths.png", bgLayerETexture);

		//Layer D
		GLuint bgLayerDTexture;
		CTexture bgLayerDTex = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Background/Background Layers/Sprite/jazzbar(D)_stage.png", bgLayerDTexture);

		//Background Dancer Girls
		GLuint dancerGirlsSpriteSheet;
		CTexture girlsTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/jazz_girls_Sheet.png", dancerGirlsSpriteSheet);

		//Jazz Crowd B
		GLuint jazzCrowdSpriteSheet;
		CTexture jazzCrowdBTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/Jazz_CrowdB/jazz_crowdB.png", jazzCrowdSpriteSheet);

		// Waiter	
		GLuint waiterSpriteSheet;
		CTexture waiterTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/Waiter/jazz_waiter_sheet.png", waiterSpriteSheet);

		//Layer C
		GLuint bgLayerCTexture;
		CTexture bgLayerCTex = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Background/Background Layers/Sprite/jazzbar(C)_mid.png", bgLayerCTexture);

		//Jazz Crowd A
		GLuint jazzCrowdASpriteSheet;
		CTexture jazzCrowdATexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/Jazz_CrowdA/jazz_crowdA.png", jazzCrowdASpriteSheet);

		//Tall Frog
		GLuint tallFrogSpriteSheet;
		//CTexture tallFrogTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/TallFrog/TallFrogNew.png", tallFrogSpriteSheet);
		CTexture tallFrogTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/TallFrog/Update1/TallFrog.png", tallFrogSpriteSheet);

		// Short Frog
		GLuint shortFrogSpriteSheet;
		CTexture shortFrogTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/ShortFrog/Update_1/Short_Frog_Sheet.png", shortFrogSpriteSheet);

		//Cuphead
		GLuint cupheadSpriteSheet;
		CTexture cupheadTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/Cuphead/Update_Dash/Cuphead_Player_Sprite_Sheet.png", cupheadSpriteSheet);

		// Bullet - 15
		GLuint peaShooterBulletMain;
		CTexture peaShooterBulletTex = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/Cuphead Weapon Peashot Main/weapon_peashot_main.png", peaShooterBulletMain);

		// Frog fist - 16
		GLuint frogFistSpriteSheet;
		CTexture frogfistTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/ShortFrog/shortfrog_fist.png", frogFistSpriteSheet);

		// firefly - 17
		GLuint fireFliesSpriteSheet;
		CTexture fireFlyTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/FireFly/tallfrog_firefly.png", fireFliesSpriteSheet);

		// frog ball - 18
		GLuint frogBallSpriteSheet;
		CTexture frogBallTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/ShortFrog/shortfrog_ball_new.png", frogBallSpriteSheet);

		// peashot spark - 19
		GLuint bulletSpawnSpriteSheet;
		CTexture bulletSpawnTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/Cuphead Weapon Peashot Spawn/weapon_peashot_spark-sheet.png", bulletSpawnSpriteSheet);

		// peashot death - 20
		GLuint bulletDeathSpriteSheet;
		CTexture bulletDeathTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/Cuphead Weapon Peashot Death/weapon_peashot_death.png", bulletDeathSpriteSheet);

		// wind - 21
		GLuint windSpriteSheet;
		CTexture windTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/TallFrog Fan Wind/tallfrog_fan_wind_new.png", windSpriteSheet);

		// dashVFX - 22
		GLuint dashVFXSpriteSheet;
		CTexture dashVFXTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/Cuphead Dash Dust A/player_dashDustA.png", dashVFXSpriteSheet);

		// landVFX - 23
		GLuint landVFXSpriteSheet;
		CTexture landVFXTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/Cuphead Alt Jump Dust A/player_alt_jump_dust_a.png", landVFXSpriteSheet);

		// Ready? WALLOP! - 24
		GLuint readyWallopSpriteSheet;
		CTexture readyWallopTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/FightText/FightText_GetReady-sheet-Long.png", readyWallopSpriteSheet);

		// A KNOCKOUT! - 25
		GLuint aKnockoutSpriteSheet;
		CTexture aKnockoutTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/FightText/FightText_KO-new-sheet.png", aKnockoutSpriteSheet);

		// Cuphead HitVFX - 26
		GLuint cupheadHitVFXSpriteSheet;
		CTexture cupheadHitVFXTexture = RenderManager::Instance()->LoadImage("../../Sprite Sheets/VFX/Cuphead Hit FX A/player_hitFXA.png", cupheadHitVFXSpriteSheet);

		//HP UI - 27
		GLuint hpUISheet;
		CTexture hpUITex = RenderManager::Instance()->LoadImage("../../Sprite Sheets/UI/UI_Health_sheet.png", hpUISheet);

		// Plants Foreground - 28
		GLuint plantsTexture;
		CTexture plantsTex = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Background/Background Layers/Sprite/jazzbar(A)_plants.png", plantsTexture);

		// Tables Foreground - 29
		GLuint tablesTexture;
		CTexture tablesTex = RenderManager::Instance()->LoadImage("../../Raw Art Assets/Raw Sprites/Background/Background Layers/Sprite/jazzbar(B)_tables.png", tablesTexture);

		// Iris effect - 30
		GLuint irisSpriteSheet;
		CTexture irisTex = RenderManager::Instance()->LoadImage("../../Sprite Sheets/FightText/iris-long.png", irisSpriteSheet);

		// END TEXTURES
		//*****************************************
	}

	void Game::RegisterSignatures()
	{
		GameObject::registerSignatures();
		Action::registerSignatures();
		TypeManager::registerTypeAsChild(ActionList::TypeIdClass(), Action::TypeIdClass(), ActionList::prescribedAttributes());
		TypeManager::registerTypeAsChild(RenderAction::TypeIdClass(), Action::TypeIdClass(), RenderAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(AnimRenderAction::TypeIdClass(), RenderAction::TypeIdClass(), AnimRenderAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(AbstractCollider::TypeIdClass(), Action::TypeIdClass(), AbstractCollider::prescribedAttributes());
		TypeManager::registerTypeAsChild(BoxCollider::TypeIdClass(), AbstractCollider::TypeIdClass(), BoxCollider::prescribedAttributes());
		TypeManager::registerTypeAsChild(CombatAction::TypeIdClass(), Action::TypeIdClass(), CombatAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(DamageDealer::TypeIdClass(), CombatAction::TypeIdClass(), DamageDealer::prescribedAttributes());
		TypeManager::registerTypeAsChild(ProjectileDamageAction::TypeIdClass(), DamageDealer::TypeIdClass(), ProjectileDamageAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(DamageReceiver::TypeIdClass(), Action::TypeIdClass(), DamageReceiver::prescribedAttributes());
		TypeManager::registerTypeAsChild(WeaponAction::TypeIdClass(), Action::TypeIdClass(), WeaponAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(PeaShooter::TypeIdClass(), WeaponAction::TypeIdClass(), PeaShooter::prescribedAttributes());
		TypeManager::registerTypeAsChild(ParticleRenderAction::TypeIdClass(), RenderAction::TypeIdClass(), ParticleRenderAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(ChangeGameState::TypeIdClass(), Action::TypeIdClass(), ChangeGameState::prescribedAttributes());
		TypeManager::registerTypeAsChild(WindAction::TypeIdClass(), CombatAction::TypeIdClass(), WindAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(GravityAction::TypeIdClass(), Action::TypeIdClass(), GravityAction::prescribedAttributes());

		TypeManager::registerTypeAsChild(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::prescribedAttributes());

		TypeManager::registerTypeAsChild(Reaction::TypeIdClass(), ActionList::TypeIdClass(), Reaction::prescribedAttributes());
		TypeManager::registerTypeAsChild(DispatchEventAction::TypeIdClass(), Action::TypeIdClass(), DispatchEventAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(PlaySFXAction::TypeIdClass(), Action::TypeIdClass(), PlaySFXAction::prescribedAttributes());
		TypeManager::registerTypeAsChild(PlaySFXRandomAction::TypeIdClass(), Action::TypeIdClass(), PlaySFXRandomAction::prescribedAttributes());

		TypeManager::registerTypeAsChild(HealthRenderAction::TypeIdClass(), AnimRenderAction::TypeIdClass(), HealthRenderAction::prescribedAttributes());
	}

	/*
	* Poll events for the game, if any
	*/
	void Game::PollingEvent()
	{
		glfwPollEvents();

		// update gamepad if connected
		if (InputManager::_controller.contollerConnected == 1)
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &InputManager::_controller.state);
		}
	}

	/*
	* Update the game every frame
	*/
	void Game::Update(const GameTime& time)
	{
		GameObjectManager::Update(time);
	}

	/*
	* Render the game every frame
	*/
	void Game::Render()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //Background color
		glClear(GL_COLOR_BUFFER_BIT);

		RenderManager::Instance()->Render();
		//CollisionManager::drawDebugBoxes();

		RenderManager::Instance()->SwapBuffers();
		RenderManager::Instance()->PollEvents();
	}

	/*
	* Clean up the game
	*/
	void Game::Cleanup()
	{
		AudioManager::CleanupAudioManager();
		ParticleManager::shutdown();
	}
#pragma endregion
}
