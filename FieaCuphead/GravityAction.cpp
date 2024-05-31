#include "GravityAction.h"
#include "TypeManager.h"
#include "GameObject.h"

namespace Fiea::GameEngine
{
	RTTI_DEFINITIONS(GravityAction);

	GravityAction::GravityAction(IdType type) : Action(type)
	{
	}

	GravityAction::GravityAction() : Action(TypeIdClass())
	{
	}

	GravityAction::GravityAction(const GravityAction& other) : Action(other)
	{
	}

	GravityAction* GravityAction::clone() const
	{
		return new GravityAction(*this);
	}

	void GameEngine::GravityAction::onEnable()
	{
	}

	void GameEngine::GravityAction::onDisable()
	{
	}

	void GameEngine::GravityAction::update(const GameTime& time)
	{
		if (parent == nullptr)
		{
			parent = getParentGameObject();
		}

		if (parent->transform.position.y >= 125.0f)
		{
			ySpeed -= gravity * time.Frame() * 0.01f;
			
			parent->transform.position.y += ySpeed * time.Frame() * 0.01f;
		}
		else
		{
			if (parent->transform.position.y <= 125.0f)
			{
				ySpeed = 0.0f;
				parent->transform.position.y = 125.0f;
				return;
			}
		}
	}

	std::vector<Signature> GameEngine::GravityAction::prescribedAttributes()
	{
		return std::vector<Signature>
		{
			{"float_ySpeed", Datum::DataType::FLOAT, 1, offsetof(GravityAction, ySpeed)},
			{ "float_gravity", Datum::DataType::FLOAT, 1, offsetof(GravityAction, gravity) }
		};
	}

	bool GameEngine::GravityAction::registerSignatures()
	{
		static bool registered = []()
		{
			TypeManager::registerType(TypeIdClass(), prescribedAttributes());
			return true;
		}();
		return registered;
	}

	std::string GameEngine::GravityAction::ToString() const
	{
		return std::string("GravityAction");
	}

}
