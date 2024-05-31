#include "cupheadStates.h"
#include "GamepadHelper.h"
#include "ParticleManager.h"

void Fiea::GameEngine::CupheadIdle::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		if (jumpExhausted)
		{
			GameObject* particle = ParticleManager::getParticle();

			ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
			if (pa == nullptr) { return; }

			particle->transform.position = gameObject->transform.position;
			particle->transform.position.y += -50.0f;

			CTexture tex;
			tex.textureRef = 23;
			tex.width = 3864;
			tex.height = 145;
			SpriteAnim anim;
			anim.startingCell = 0;
			anim.nFrames = 14;

			pa->SetTextureAndAnim(tex, anim);
			pa->setSize(125.0f, 125.0f);
		}

		firstFrame = false;
	}



	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		shooting = true;
		
		
		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP)
			_context->TransitionTo(new CupheadShoot_Up());
		else
			_context->TransitionTo(new CupheadShoot_Straight());
		
		return;
	}
	
	if (InputManager::GetKeyDown(VK_C) || GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		aiming = true;
		_context->TransitionTo(new CupheadAim_Straight());
		return;

	}

	if (!InputManager::GetKeyDown(VK_SPACE) && !GAMEPAD_BUTTON_A)
	{
		jumpExhausted = false;
	}

	if (InputManager::GetKeyDown(VK_TAB) || GAMEPAD_BUTTON_B)
	{
		if (canDash)
		{
			_context->TransitionTo(new CupheadDash());
			aiming = false;
			return;
		}

	}
	else if (!InputManager::GetKeyDown(VK_TAB) && !GAMEPAD_BUTTON_B)
	{
		canDash = true;
	}


	if ((InputManager::GetKeyDown(VK_S) && !aiming) || (GAMEPAD_LEFT_AXIS_DOWN && !aiming))
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}
	else if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A))
	{

	}
	else if ((InputManager::GetKeyDown(VK_D) && !aiming) || (GAMEPAD_LEFT_AXIS_RIGHT && !aiming))
	{
		_context->TransitionTo(new CupheadRun(jumpExhausted, canDash));
		return;
	}
	else if ((InputManager::GetKeyDown(VK_A) && !aiming) || (GAMEPAD_LEFT_AXIS_LEFT && !aiming))
	{
		_context->TransitionTo(new CupheadRun(jumpExhausted, canDash));
		return;
	}
	else if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{ 
		if (!jumpExhausted)
		{
			_context->TransitionTo(new CupheadJump());
			aiming = false;
			return;
		}
		
	}




}

void Fiea::GameEngine::CupheadRun::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_SPACE) && !GAMEPAD_BUTTON_A)
	{
		IdleJumpState = false;
	}
	else if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		if (!IdleJumpState)
		{
			_context->TransitionTo(new CupheadJump());
			return;
		}

	}

	if (InputManager::GetKeyDown(VK_TAB) || GAMEPAD_BUTTON_B)
	{
		if (canDash)
		{
			_context->TransitionTo(new CupheadDash());
			return;
		}
	}
	else if (!InputManager::GetKeyDown(VK_TAB) && !GAMEPAD_BUTTON_B)
	{
		canDash = true;
	}

	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadRun_Shoot_Straight());
		return;
	}


	if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
	}
	else if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A))
	{
		_context->TransitionTo(new CupheadIdle(IdleJumpState));
	}
	else if (InputManager::GetKeyDown(VK_D) || GAMEPAD_LEFT_AXIS_RIGHT)
	{
		gameObject->transform.position.x += 25.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x > 760.0f) { gameObject->transform.position.x = 760; }

		useStateMirrror = true;
		isMirror = false;
	}
	else if (InputManager::GetKeyDown(VK_A) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		gameObject->transform.position.x -= 25.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x < 40.0f) { gameObject->transform.position.x = 40; }

		useStateMirrror = true;
		isMirror = true;
	}
	else { _context->TransitionTo(new CupheadIdle(IdleJumpState)); }

}

void Fiea::GameEngine::CupheadJump::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	// check if this is jumping from the ground
	if (gameObject->transform.position.y == 125.0f)
	{
		EventDispatcher::enqueue(OnStartJumpEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, time);
	}


	//if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	//{
	//	_context->TransitionTo(new CupheadShoot_Straight());
	//	return;
	//}

	if (InputManager::GetKeyDown(VK_TAB) || GAMEPAD_BUTTON_B)
	{
		if (canDash)
		{
			_context->TransitionTo(new CupheadDashAir(reachedApex));
			return;
		}

	}

	if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A))
	{
		//_context->TransitionTo(new CupheadIdle());
	}
	else if (InputManager::GetKeyDown(VK_D) || GAMEPAD_LEFT_AXIS_RIGHT)
	{
		gameObject->transform.position.x += 15.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x > 760.0f) { gameObject->transform.position.x = 760; }

		useStateMirrror = true;
		isMirror = false;
	}
	else if (InputManager::GetKeyDown(VK_A) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		gameObject->transform.position.x -= 15.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x < 40.0f) { gameObject->transform.position.x = 40; }

		useStateMirrror = true;
		isMirror = true;
	}
	//else { _context->TransitionTo(new CupheadIdle()); }

	if (!reachedApex)
	{
		yVelocity += gravity * time.Frame() * 0.01f;

		gameObject->transform.position.y += yVelocity * time.Frame() * 0.01f;


		if (gameObject->transform.position.y > jumpHeight)
		{
			reachedApex = true;
		}
	}
	else
	{
		yVelocity -= gravity * time.Frame() * 0.01f;


		gameObject->transform.position.y += yVelocity * time.Frame() * 0.01f;

		if (gameObject->transform.position.y <= 125.0f)
		{
			gameObject->transform.position.y = 125.0f;
			_context->TransitionTo(new CupheadIdle(true, false));
			return;
		}
	}

	if (!InputManager::GetKeyDown(VK_SPACE) && !GAMEPAD_BUTTON_A)
	{
		canParry = true;
		return;
	}
	else if ((InputManager::GetKeyDown(VK_SPACE) && canParry) || (GAMEPAD_BUTTON_A && canParry))
	{
		_context->TransitionTo(new CupheadParry(reachedApex, canDash, yVelocity, jumpHeight, gravity));
		return;
	}
}

void Fiea::GameEngine::CupheadParry::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_TAB) || GAMEPAD_BUTTON_B)
	{
		if (canDash)
		{
			_context->TransitionTo(new CupheadDashAir(reachedApex));
			return;
		}

	}



	if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A))
	{
		//_context->TransitionTo(new CupheadIdle());
	}
	else if (InputManager::GetKeyDown(VK_D) || GAMEPAD_LEFT_AXIS_RIGHT)
	{
		gameObject->transform.position.x += 15.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x > 760.0f) { gameObject->transform.position.x = 760; }

		useStateMirrror = true;
		isMirror = false;
	}
	else if (InputManager::GetKeyDown(VK_A) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		gameObject->transform.position.x -= 15.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x < 40.0f) { gameObject->transform.position.x = 40; }

		useStateMirrror = true;
		isMirror = true;
	}
	//else { _context->TransitionTo(new CupheadIdle()); }

	if (!reachedApex)
	{
		yVelocity += gravity * time.Frame() * 0.01f;

		gameObject->transform.position.y += yVelocity * time.Frame() * 0.01f;


		if (gameObject->transform.position.y > jumpHeight)
		{
			reachedApex = true;
		}
	}
	else
	{
		yVelocity -= gravity * time.Frame() * 0.01f;


		gameObject->transform.position.y += yVelocity * time.Frame() * 0.01f;

		if (gameObject->transform.position.y <= 125.0f)
		{
			gameObject->transform.position.y = 125.0f;
			_context->TransitionTo(new CupheadIdle(true, false));
			return;
		}
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new CupheadJump(reachedApex, canDash, yVelocity));
		return;
	}
}


void Fiea::GameEngine::CupheadDuck::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(15);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, -35.0f, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		if (_animRenderAction->isAnimComplete())
		{
			_context->TransitionTo(new CupheadDuckIdle());
			return;
		}
	}
	else if(InputManager::GetKeyDown(VK_S) && InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A))
	{
		if (_animRenderAction->isAnimComplete())
		{
			_context->TransitionTo(new CupheadDuckIdle());
			return;
		}
	}
	else
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}


	//if (InputManager::GetKeyDown(VK_S))
	//{


	//	_context->TransitionTo(new CupheadDuckIdle());
	//}
	//else
	//{
	//	
	//}
	
}

void Fiea::GameEngine::CupheadDuckIdle::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(15);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, -35.0f, 0, 0));

		firstFrame = false;
	}

	if ((InputManager::GetKeyDown(VK_S) && InputManager::GetKeyDown(VK_A)) || (GAMEPAD_LEFT_AXIS_DOWN && GAMEPAD_LEFT_AXIS_LEFT))
	{
		useStateMirrror = true;
		isMirror = true;
	}
	else if ((InputManager::GetKeyDown(VK_S) && InputManager::GetKeyDown(VK_D)) || (GAMEPAD_LEFT_AXIS_DOWN && GAMEPAD_LEFT_AXIS_RIGHT))
	{
		useStateMirrror = true;
		isMirror = false;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{

	}
	else
	{
		_context->TransitionTo(new CupheadIdle());
	}
	
	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadDuck_Shoot());
		return;
	}


}

void Fiea::GameEngine::CupheadDuck_Shoot::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(15);
		_boxCollider->setHalfWidth(20);
		_boxCollider->setPosOffset(glm::vec4(0, -35.0f, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_S) && !GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadDuckIdle());
		return;
	}

}

void Fiea::GameEngine::CupheadDash::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(35);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		// Event for begin dash
		EventDispatcher::enqueue(OnStartDashEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));

		// spawn vfx for dash
		GameObject* particle = ParticleManager::getParticle();

		ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
		if (pa == nullptr) { return; }

		particle->transform.position = gameObject->transform.position;	// spawn the vfx at player's last location before dash

		if (particle->transform.position.x <= 75.0f)	// we hit the far left of the screen
		{ 
			particle->transform.position.x = 140;
		}
		else if (particle->transform.position.x >= 725.0f)	// we hit the far right of the screen
		{
			particle->transform.position.x = 705;
		}

		CTexture tex;
		tex.textureRef = 22;
		tex.width = 3270;
		tex.height = 207;
		SpriteAnim anim;
		anim.startingCell = 0;
		anim.nFrames = 15;

		pa->SetTextureAndAnim(tex, anim);
		if (_animRenderAction->isMirrored())
		{
			pa->setMirror(true);
		}
		else
		{
			pa->setMirror(false);
		}
		pa->setSize(100.0f, 100.0f);
	}

	if (_animRenderAction->isMirrored())
	{
		gameObject->transform.position.x -= 50.0f * time.Frame() * 0.01f;
		if (gameObject->transform.position.x < 75.0f) { gameObject->transform.position.x = 75; }
	}
	else
	{
		gameObject->transform.position.x += 50.0f * time.Frame() * 0.01f;
		if (gameObject->transform.position.x > 725.0f) { gameObject->transform.position.x = 725; }
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new CupheadIdle(false, true));
	}
	if (firstFrame == true) { firstFrame = false; }
}

void Fiea::GameEngine::CupheadDashAir::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(35);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		// Event for begin dash
		EventDispatcher::enqueue(OnStartDashEvent::TypeIdClass(), &CupheadEvents::EmptyIEvent, CreateElapsedTime(0));

		GameObject* particle = ParticleManager::getParticle();

		ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
		if (pa == nullptr) { return; }

		particle->transform.position = gameObject->transform.position;

		if (particle->transform.position.x <= 75.0f)	// we hit the far left of the screen
		{
			particle->transform.position.x = 140;
		}
		else if (particle->transform.position.x >= 725.0f)	// we hit the far right of the screen
		{
			particle->transform.position.x = 705;
		}

		CTexture tex;
		tex.textureRef = 22;
		tex.width = 3270;
		tex.height = 207;
		SpriteAnim anim;
		anim.startingCell = 0;
		anim.nFrames = 15;

		pa->SetTextureAndAnim(tex, anim);
		if (_animRenderAction->isMirrored())
		{
			pa->setMirror(true);
		}
		else
		{
			pa->setMirror(false);
		}
		pa->setSize(100.0f, 100.0f);
	}

	if (_animRenderAction->isMirrored())
	{
		gameObject->transform.position.x -= 50.0f * time.Frame() * 0.01f;
		if (gameObject->transform.position.x < 75.0f) { gameObject->transform.position.x = 75; }

	}
	else
	{
		gameObject->transform.position.x += 50.0f * time.Frame() * 0.01f;
		if (gameObject->transform.position.x > 725.0f) { gameObject->transform.position.x = 725; }

	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new CupheadJump(true, false));
	}
	
	if (firstFrame) { firstFrame = false; }
}


void Fiea::GameEngine::CupheadAim_Straight::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}
	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}



	if (InputManager::GetKeyDown(VK_LEFT) && InputManager::GetKeyDown(VK_RIGHT))
	{
	}
	else if (InputManager::GetKeyDown(VK_UP) && InputManager::GetKeyDown(VK_DOWN))
	{
	}
	else if (InputManager::GetKeyDown(VK_LEFT) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		useStateMirrror = true;
		isMirror = true;

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadAim_Diag_Up());
			return;
		}
		else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadAim_Diag_Down());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_RIGHT) || GAMEPAD_LEFT_AXIS_RIGHT) {
		useStateMirrror = true;
		isMirror = false;
		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadAim_Diag_Up());
			return;
		}
		else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadAim_Diag_Down());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadAim_Up());
		return;
	}
	else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadAim_Down());
		return;
	}

}

void Fiea::GameEngine::CupheadAim_Diag_Up::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadShoot_Diag_Up());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_UP) && !GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadAim_Straight());
		return;
	}

	if ((InputManager::GetKeyDown(VK_LEFT) && !isMirror) || (GAMEPAD_LEFT_AXIS_LEFT && !isMirror)) {
		isMirror = true;
		useStateMirrror = true;
	}
	
	if ((InputManager::GetKeyDown(VK_RIGHT) && isMirror) || (GAMEPAD_LEFT_AXIS_RIGHT && isMirror)) {
		isMirror = false;
		useStateMirrror = true;
	}

	//Why are you not a mirror?
	if ((!InputManager::GetKeyDown(VK_LEFT) && isMirror) && (!GAMEPAD_LEFT_AXIS_LEFT && isMirror)) {
		_context->TransitionTo(new CupheadAim_Up());
		return;
	}
	else if ((!InputManager::GetKeyDown(VK_RIGHT) && !isMirror) && (!GAMEPAD_LEFT_AXIS_RIGHT && !isMirror)) {
		_context->TransitionTo(new CupheadAim_Up());
		return;
	}
}

void Fiea::GameEngine::CupheadAim_Diag_Down::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadShoot_Diag_Down());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_DOWN) && !GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadAim_Straight());
		return;
	}

	if ((InputManager::GetKeyDown(VK_LEFT) && !isMirror) || (GAMEPAD_LEFT_AXIS_LEFT && !isMirror)) {

		isMirror = true;
		useStateMirrror = true;
	}

	if ((InputManager::GetKeyDown(VK_RIGHT) && isMirror) || (GAMEPAD_LEFT_AXIS_RIGHT && isMirror)) {

		isMirror = false;
		useStateMirrror = true;
	}

	if ((!InputManager::GetKeyDown(VK_LEFT) && isMirror) && (!GAMEPAD_LEFT_AXIS_LEFT && isMirror)) {
		_context->TransitionTo(new CupheadAim_Down());
		return;
	}
	else if ((!InputManager::GetKeyDown(VK_RIGHT) && !isMirror) && (!GAMEPAD_LEFT_AXIS_RIGHT && !isMirror)) {
		_context->TransitionTo(new CupheadAim_Down());
		return;
	}
} 

void Fiea::GameEngine::CupheadAim_Down::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadShoot_Down());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_DOWN) && !GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadAim_Straight());
		return;
	}


	if (InputManager::GetKeyDown(VK_RIGHT) || GAMEPAD_LEFT_AXIS_RIGHT) {
		useStateMirrror = true;
		isMirror = false;


		_animRenderAction->setMirror(false);

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadAim_Diag_Up());
			return;
		}
		else {
			_context->TransitionTo(new CupheadAim_Diag_Down());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_LEFT) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		useStateMirrror = true;
		isMirror = true;

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadAim_Diag_Up());
			return;
		}
		else
		{
			_context->TransitionTo(new CupheadAim_Diag_Down());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadAim_Straight());
		return;
	}
}

void Fiea::GameEngine::CupheadAim_Up::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(40);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_CTRL) || GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadShoot_Up());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_UP) && !GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadAim_Straight());
		return;
	}

	if (InputManager::GetKeyDown(VK_LEFT) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		useStateMirrror = true;
		isMirror = true;

		if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadAim_Diag_Down());
			return;
		}
		else
		{
			_context->TransitionTo(new CupheadAim_Diag_Up());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_RIGHT) || GAMEPAD_LEFT_AXIS_RIGHT) {
		useStateMirrror = true;
		isMirror = false;

		_animRenderAction->setMirror(false);

		if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadAim_Diag_Down());
			return;
		}
		else {
			_context->TransitionTo(new CupheadAim_Diag_Up());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadAim_Straight());
		return;
	}
}
void Fiea::GameEngine::CupheadRun_Shoot_Straight::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadRun());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck_Shoot());
		return;
	}

	if (InputManager::GetKeyDown(VK_D) || GAMEPAD_LEFT_AXIS_RIGHT)
	{
		gameObject->transform.position.x += 25.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x > 760.0f) { gameObject->transform.position.x = 760; }

		useStateMirrror = true;
		isMirror = false;
		}
	else if (InputManager::GetKeyDown(VK_A) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		gameObject->transform.position.x -= 25.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x < 40.0f) { gameObject->transform.position.x = 40; }

		useStateMirrror = true;
		isMirror = true;
	}

	if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A)) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if ((!InputManager::GetKeyDown(VK_D) && !isMirror) && (!GAMEPAD_LEFT_AXIS_RIGHT && !isMirror)) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if ((!InputManager::GetKeyDown(VK_A) && isMirror) && (!GAMEPAD_LEFT_AXIS_LEFT && isMirror)) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if (InputManager::GetKeyDown(VK_W)) {
		_context->TransitionTo(new CupheadRun_Shoot_Diag_Up());
		return;
	}
}

void Fiea::GameEngine::CupheadRun_Shoot_Diag_Up::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (InputManager::GetKeyDown(VK_D) || GAMEPAD_LEFT_AXIS_RIGHT)
	{
		gameObject->transform.position.x += 25.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x > 760.0f) { gameObject->transform.position.x = 760; }

		useStateMirrror = true;
		isMirror = false;
	}
	else if (InputManager::GetKeyDown(VK_A) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		gameObject->transform.position.x -= 25.0f * time.Frame() * 0.01f;

		if (gameObject->transform.position.x < 40.0f) { gameObject->transform.position.x = 40; }

		useStateMirrror = true;
		isMirror = true;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck_Shoot());
		return;
	}

	if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A)) 
	{
		//_context->TransitionTo(new CupheadShoot_Up());
		//return;
	}
	else if ((!InputManager::GetKeyDown(VK_D) && !isMirror) && (!GAMEPAD_LEFT_AXIS_RIGHT && !isMirror)) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}
	else if ((!InputManager::GetKeyDown(VK_A) && isMirror) && (!GAMEPAD_LEFT_AXIS_LEFT && isMirror)) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if (!InputManager::GetKeyDown(VK_W)) {
		_context->TransitionTo(new CupheadRun_Shoot_Straight());
		return;
	}
}

void Fiea::GameEngine::CupheadShoot_Straight::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck_Shoot());
		return;
	}

	if (InputManager::GetKeyDown(VK_LEFT) && InputManager::GetKeyDown(VK_RIGHT) && InputManager::GetKeyDown(VK_C))
	{
	}
	else if (InputManager::GetKeyDown(VK_UP) && InputManager::GetKeyDown(VK_DOWN) && InputManager::GetKeyDown(VK_C))
	{
	}
	else if ((InputManager::GetKeyDown(VK_LEFT) && InputManager::GetKeyDown(VK_C)) || (GAMEPAD_LEFT_AXIS_LEFT && GAMEPAD_BUTTON_RIGHT_BUMPER))

	{
		useStateMirrror = true;
		isMirror = true;

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadShoot_Diag_Up());
			return;
		}
		else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadShoot_Diag_Down());
			return;
		}
	}
	else if ((InputManager::GetKeyDown(VK_RIGHT) && InputManager::GetKeyDown(VK_C)) || (GAMEPAD_LEFT_AXIS_RIGHT && GAMEPAD_BUTTON_RIGHT_BUMPER)) {
		useStateMirrror = true;
		isMirror = false;

		_animRenderAction->setMirror(false);

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadShoot_Diag_Up());
			return;
		}
		else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadShoot_Diag_Down());
			return;
		}
	}
	else if ((InputManager::GetKeyDown(VK_UP) && InputManager::GetKeyDown(VK_C)) || (GAMEPAD_LEFT_AXIS_UP && GAMEPAD_BUTTON_RIGHT_BUMPER)) {
		_context->TransitionTo(new CupheadShoot_Up());
		return;
	}
	else if ((InputManager::GetKeyDown(VK_DOWN) && InputManager::GetKeyDown(VK_C)) || (GAMEPAD_LEFT_AXIS_DOWN && GAMEPAD_BUTTON_RIGHT_BUMPER)) {
		_context->TransitionTo(new CupheadShoot_Down());
		return;
	}

	if (InputManager::GetKeyDown(VK_D) && InputManager::GetKeyDown(VK_A))
	{

	}
	else if (InputManager::GetKeyDown(VK_D) || GAMEPAD_LEFT_AXIS_RIGHT)
	{
		useStateMirrror = true;
		isMirror = false;
		_context->TransitionTo(new CupheadRun_Shoot_Straight());
		return;
	}
	else if (InputManager::GetKeyDown(VK_A) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		useStateMirrror = true;
		isMirror = true;
		_context->TransitionTo(new CupheadRun_Shoot_Straight());
		return;
	}
}

void Fiea::GameEngine::CupheadShoot_Diag_Up::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(40);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadAim_Diag_Up());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_UP) && !GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if ((InputManager::GetKeyDown(VK_LEFT) && !isMirror) || (GAMEPAD_LEFT_AXIS_LEFT && !isMirror)) {
		isMirror = true;
		useStateMirrror = true;
	}

	if ((InputManager::GetKeyDown(VK_RIGHT) && isMirror) || (GAMEPAD_LEFT_AXIS_RIGHT && isMirror)) {
		isMirror = false;
		useStateMirrror = true;
	}

	if ((!InputManager::GetKeyDown(VK_LEFT) && isMirror) && (!GAMEPAD_LEFT_AXIS_LEFT && isMirror)) {
		_context->TransitionTo(new CupheadShoot_Up());
		return;
	}
	else if ((!InputManager::GetKeyDown(VK_RIGHT) && !isMirror) && (!GAMEPAD_LEFT_AXIS_RIGHT && !isMirror)) {
		_context->TransitionTo(new CupheadShoot_Up());
		return;
	}
}

void Fiea::GameEngine::CupheadShoot_Diag_Down::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadAim_Diag_Down());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_DOWN) && !GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}

	if ((InputManager::GetKeyDown(VK_LEFT) && !isMirror) || (GAMEPAD_LEFT_AXIS_LEFT && !isMirror)) {
		isMirror = true;
		useStateMirrror = true;
	}

	if ((InputManager::GetKeyDown(VK_RIGHT) && isMirror) || (GAMEPAD_LEFT_AXIS_RIGHT && isMirror)) {

		isMirror = false;
		useStateMirrror = true;
	}

	if ((!InputManager::GetKeyDown(VK_LEFT) && isMirror) && (!GAMEPAD_LEFT_AXIS_LEFT && isMirror)) {
		_context->TransitionTo(new CupheadShoot_Down());
		return;
	}
	else if ((!InputManager::GetKeyDown(VK_RIGHT) && !isMirror) && (!GAMEPAD_LEFT_AXIS_RIGHT && !isMirror)) {
		_context->TransitionTo(new CupheadShoot_Down());
		return;
	}
}

void Fiea::GameEngine::CupheadShoot_Down::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadAim_Down());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_DOWN) && !GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}


	if (InputManager::GetKeyDown(VK_LEFT) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		useStateMirrror = true;
		isMirror = true;

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadShoot_Diag_Up()); 
			return;
		}
		else
		{
			_context->TransitionTo(new CupheadShoot_Diag_Down());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_RIGHT) || GAMEPAD_LEFT_AXIS_RIGHT) {
		useStateMirrror = true;
		isMirror = false;

		_animRenderAction->setMirror(false);

		if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
			_context->TransitionTo(new CupheadShoot_Diag_Up());
			return;
		}
		else
			_context->TransitionTo(new CupheadShoot_Diag_Down());
		return;
	}
	else if (InputManager::GetKeyDown(VK_UP) || GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}
}

void Fiea::GameEngine::CupheadShoot_Up::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(40);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (!InputManager::GetKeyDown(VK_CTRL) && !GAMEPAD_BUTTON_X)
	{
		_context->TransitionTo(new CupheadAim_Up());
		return;
	}

	if (!InputManager::GetKeyDown(VK_C) && !GAMEPAD_BUTTON_RIGHT_BUMPER)
	{
		_context->TransitionTo(new CupheadIdle());
		return;
	}

	if (InputManager::GetKeyDown(VK_SPACE) || GAMEPAD_BUTTON_A)
	{
		_context->TransitionTo(new CupheadJump());
		return;
	}
	else if (InputManager::GetKeyDown(VK_S) || GAMEPAD_LEFT_AXIS_DOWN)
	{
		_context->TransitionTo(new CupheadDuck());
		return;
	}

	if (!InputManager::GetKeyDown(VK_UP) && !GAMEPAD_LEFT_AXIS_UP) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}


	if (InputManager::GetKeyDown(VK_LEFT) || GAMEPAD_LEFT_AXIS_LEFT)
	{
		useStateMirrror = true;
		isMirror = true;

		if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadShoot_Diag_Down());
			return;
		}
		else
		{
			_context->TransitionTo(new CupheadShoot_Diag_Up());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_RIGHT) || GAMEPAD_LEFT_AXIS_RIGHT) {
		useStateMirrror = true;
		isMirror = false;

		_animRenderAction->setMirror(false);

		if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
			_context->TransitionTo(new CupheadShoot_Diag_Down());
			return;
		}
		else {
			_context->TransitionTo(new CupheadShoot_Diag_Up());
			return;
		}
	}
	else if (InputManager::GetKeyDown(VK_DOWN) || GAMEPAD_LEFT_AXIS_DOWN) {
		_context->TransitionTo(new CupheadShoot_Straight());
		return;
	}
}

void Fiea::GameEngine::CupheadIntro_1::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		firstFrame = false;
	}

	if (_animRenderAction->isAnimComplete())
	{
		_context->TransitionTo(new CupheadIdle());
	}
}

void Fiea::GameEngine::CupheadHit::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		// spawn hit VFX
		GameObject* particle = ParticleManager::getParticle();

		ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
		if (pa == nullptr) { return; }

		particle->transform.position = gameObject->transform.position;

		CTexture tex;
		tex.textureRef = 26;
		tex.width = 4158;
		tex.height = 526;
		SpriteAnim anim;
		anim.startingCell = 0;
		anim.nFrames = 9;

		pa->SetTextureAndAnim(tex, anim);
		pa->setSize(200.0f, 200.0f);

		firstFrame = false;
	}

	if (_animRenderAction->getCurrentFrame() >= 5.5f)
	{
		if (_animRenderAction->getCurrentFrame() >= 5.5f)
		{
			if (_isDead)
			{
				_context->TransitionTo(new CupheadGhost());
				return;
			}
			else
			{
				_context->TransitionTo(new CupheadIdle());
				return;
			}
		}
	}
}

void Fiea::GameEngine::CupheadHit_Air::update(const GameTime& time, GameObject* gameObject)
{
	if (firstFrame)
	{
		// update hitbox
		assert(_boxCollider != nullptr);
		_boxCollider->setHalfHeight(30);
		_boxCollider->setHalfWidth(15);
		_boxCollider->setPosOffset(glm::vec4(0, 0, 0, 0));

		// spawn hit VFX
		GameObject* particle = ParticleManager::getParticle();

		ParticleRenderAction* pa = particle->getAction("ParticleRenderAction")->As<ParticleRenderAction>();
		if (pa == nullptr) { return; }

		particle->transform.position = gameObject->transform.position;

		CTexture tex;
		tex.textureRef = 26;
		tex.width = 4158;
		tex.height = 526;
		SpriteAnim anim;
		anim.startingCell = 0;
		anim.nFrames = 9;

		pa->SetTextureAndAnim(tex, anim);
		pa->setSize(200.0f, 200.0f);

		firstFrame = false;

		firstFrame = false;
	}

	if (_animRenderAction->getCurrentFrame() >= 5.5f)
	{
		if (_isDead)
		{
			_context->TransitionTo(new CupheadGhost());
			return;
		}
		else
		{
			_context->TransitionTo(new CupheadJump());
			return;
		}
	}
}

void Fiea::GameEngine::CupheadGhost::update(const GameTime& time, GameObject* gameObject)
{
	gameObject->transform.position.y += 4.0f * time.Frame() * 0.01f;
}
