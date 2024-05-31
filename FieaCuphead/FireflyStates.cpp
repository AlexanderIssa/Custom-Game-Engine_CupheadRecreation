#include "FireflyStates.h"
//#include "PlayerManager.h"
#include "EventDispatcher.h"
#include "DamageDealer.h"
#include "AudioManager.h"

void Fiea::GameEngine::FireflyLeft::update(const GameTime& time, GameObject* gameObject)
{
    
    if (!isMoving) {
        srand(static_cast<unsigned int>(time.Game()));
        // Generate random target position within the specified range
        targetX = static_cast<float>(rand() % 401 + 150); // Random x in range [150, 750]
        targetY = static_cast<float>(rand() % 101 + 450); // Random y in range [500, 600]
        isMoving = true;
    }

    // Calculate the direction towards the target position
    float deltaX = targetX - gameObject->transform.position.x;
    float deltaY = targetY - gameObject->transform.position.y;
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    if (distance <= 1.0f) {
        // Object reached the target position
        //isMoving = false;
        _context->TransitionTo(new FireflyIdle());
        return;
    }
    else {
        // Calculate the LERP factor based on speed
        float lerpFactor = 10.0f * time.Frame() * 0.01f; // Speed of 10
        if (lerpFactor > 1.0f) {
            lerpFactor = 1.0f; // Ensure lerpFactor doesn't exceed 1
        }

        // Update the position using LERP
        gameObject->transform.position.x += deltaX * lerpFactor * 0.082f;
        gameObject->transform.position.y += deltaY * lerpFactor * 0.082f;
    }
}

void Fiea::GameEngine::FireflyIdle::update(const GameTime& time, GameObject* gameObject)
{
    if (!isWaiting) {
        // Seed the random number generator with the current time
        srand(static_cast<unsigned int>(time.Game()));

        // Generate random wait time between 3 to 6 seconds
        waitTime = static_cast<float>(rand() % 301 + 300) / 100.0f; // Random time between 3 to 6 seconds
        isWaiting = true;
        elapsedTime = 0.0f;
    }

    if (isWaiting) {
        elapsedTime += time.Frame() * 0.001f;
        if (elapsedTime >= waitTime) {
            // Do something after the wait time has passed
            // For example: // do something
            isWaiting = false;
            elapsedTime = 0.0f;
            _context->TransitionTo(new FireflyDown());
            return;
        }
    }

    if (_context->getParentGameObject()->transform.position.y < 125.0)
    {
        _context->TransitionTo(new FireflyDeath());
    }
}

void Fiea::GameEngine::FireflyDown::update(const GameTime& time, GameObject* gameObject)
{

    if (!isMoving) {
        srand(static_cast<unsigned int>(time.Game()));
        // Generate random target position within the specified range
        GameObject* cuphead = gameObject->getParentGameObject()->getChild("GameObject_GameManager", "GameObject_GameManager")->getChild("GameObject_Cuphead", "GameObject_Cuphead");
        if (cuphead == nullptr)
        {
            throw std::runtime_error("CANT FIND CUPHEAD");
        }


        // Calculate direction vector towards the Cuphead
        float cupheadX = cuphead->transform.position.x;
        float cupheadY = cuphead->transform.position.y;
        float directionX = cupheadX - gameObject->transform.position.x;
        float directionY = cupheadY - gameObject->transform.position.y;
        float directionMagnitude = sqrt(directionX * directionX + directionY * directionY);

        // Normalize the direction vector
        directionX /= directionMagnitude;
        directionY /= directionMagnitude;

        // Set the target position
        targetX = gameObject->transform.position.x + directionX * 100.0f; // Move in the general direction by 50 pixels
        targetY = gameObject->transform.position.y - 100.0f; // Move up by 100 pixels
        isMoving = true;

        if (gameObject->transform.position.y < 100.0f)
        {
            _context->TransitionTo(new FireflyDeath());
        }
    }

    // Calculate the direction towards the target position
    float deltaX = (targetX - gameObject->transform.position.x);
    float deltaY = (targetY - gameObject->transform.position.y);
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    if (distance <= 1.0f) {
        // Object reached the target position
        //isMoving = false;
        _context->TransitionTo(new FireflyIdle());
        return;
    }
    else {
        // Calculate the LERP factor based on speed
        float lerpFactor = 10.0f * time.Frame() * 0.01f; // Speed of 10
        if (lerpFactor > 1.0f) {
            lerpFactor = 1.0f; // Ensure lerpFactor doesn't exceed 1
        }

        // Update the position using LERP
        gameObject->transform.position.x += deltaX * lerpFactor * 0.082f;
        gameObject->transform.position.y += deltaY * lerpFactor * 0.082f;
    }
}

void Fiea::GameEngine::FireflyDown::OnDamage(IEvent* eventData)
{
    OnDamageDealtEvent* ev = eventData->As<OnDamageDealtEvent>();
    assert(ev != nullptr);

    assert(_context->GetAnimRenderAction() != nullptr);

    assert(_animRenderAction != nullptr);

    GameObject* a = _animRenderAction->getParentGameObject();
    GameObject* b = ev->damagingObject;


    if (_animRenderAction->getParentGameObject() == ev->damagingObject)
    {
        _context->TransitionTo(new FireflyDeath());
    }
}

void Fiea::GameEngine::FireflyDown::OnDie(IEvent* eventData)
{
	OnDeathEvent* ev = eventData->As<OnDeathEvent>();
	assert(ev != nullptr);

	if (_animRenderAction->getParentGameObject() == ev->rottingDeadThing)
	{
		_context->TransitionTo(new FireflyDeath());
	}
}

void Fiea::GameEngine::FireflyLeft::OnDamage(IEvent* eventData)
{
    OnDamageDealtEvent* ev = eventData->As<OnDamageDealtEvent>();
    assert(ev != nullptr);

    assert(_context->GetAnimRenderAction() != nullptr);

    assert(_animRenderAction != nullptr);

    GameObject* a = _animRenderAction->getParentGameObject();
    GameObject* b = ev->damagingObject;


    if (_animRenderAction->getParentGameObject() == ev->damagingObject)
    {
         _context->TransitionTo(new FireflyDeath());
    }
}

void Fiea::GameEngine::FireflyLeft::OnDie(IEvent* eventData)
{
	OnDeathEvent* ev = eventData->As<OnDeathEvent>();
	assert(ev != nullptr);

	if (_animRenderAction->getParentGameObject() == ev->rottingDeadThing)
	{
		_context->TransitionTo(new FireflyDeath());
	}
}

void Fiea::GameEngine::FireflyIdle::OnDamage(IEvent* eventData)
{
     OnDamageDealtEvent* ev = eventData->As<OnDamageDealtEvent>();
    assert(ev != nullptr);
    
    assert(_context->GetAnimRenderAction() != nullptr);

    assert(_animRenderAction != nullptr);

    GameObject* a = _animRenderAction->getParentGameObject();
    GameObject* b = ev->damagingObject;


    if (_animRenderAction->getParentGameObject() == ev->damagingObject)
    {
        _context->TransitionTo(new FireflyDeath());
    }
}

void Fiea::GameEngine::FireflyIdle::OnDie(IEvent* eventData)
{
	OnDeathEvent* ev = eventData->As<OnDeathEvent>();
	assert(ev != nullptr);

	if (_animRenderAction->getParentGameObject() == ev->rottingDeadThing)
	{
		_context->TransitionTo(new FireflyDeath());
	}
}

void Fiea::GameEngine::FireflyDeath::update(const GameTime& time, GameObject* gameObject)
{
    if (firstFrame) {
        int randomNumber = rand() % 4 + 0;

        switch (randomNumber)
        {
        case 0:
            AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogFireflyDeath/AudioClip/sfx_frogs_tall_firefly_death_01.wav", 0.75f);
            break;
        case 1:
            AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogFireflyDeath/AudioClip/sfx_frogs_tall_firefly_death_02.wav", 0.75f);
            break;
        case 2:
            AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogFireflyDeath/AudioClip/sfx_frogs_tall_firefly_death_03.wav", 0.75f);
            break;
        case 3:
            AudioManager::PlaySFX2D("../../Audio Assets/Frog Level/SFX/TallFrog/TallFrogFireflyDeath/AudioClip/sfx_frogs_tall_firefly_death_04.wav", 0.75f);
            break;
        }
        firstFrame = false;
    }
    if (!isDead)
    {
        isDead = true;
    }

    if (_animRenderAction->getCurrentFrame() >= 8.5f)
    {
        _context->getParentGameObject()->enabled = false;
        isDead = false;
    }
}
