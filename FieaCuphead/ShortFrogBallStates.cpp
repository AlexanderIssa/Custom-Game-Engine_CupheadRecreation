#include "ShortFrogBallStates.h"

void Fiea::GameEngine::BallUp::update(const GameTime& time, GameObject* gameObject)
{
	gameObject->transform.position.x += 210.0f * time.Frame() * 0.001f; // Move horizontally by 5 pixels per second
	gameObject->transform.position.y += 480.0f * time.Frame() * 0.001f; // Move vertically by 25 pixels per second

	if (gameObject->transform.position.y > 555)
		_context->TransitionTo(new BallDown());
}

void Fiea::GameEngine::BallDown::update(const GameTime& time, GameObject* gameObject)
{
	gameObject->transform.position.x += 210.0f * time.Frame() * 0.001f; // Move horizontally by 5 pixels per second
	gameObject->transform.position.y -= 480.0f * time.Frame() * 0.001f; // Move vertically by 25 pixels per second

	if (gameObject->transform.position.y < 45)
		_context->TransitionTo(new BallUp());
}
