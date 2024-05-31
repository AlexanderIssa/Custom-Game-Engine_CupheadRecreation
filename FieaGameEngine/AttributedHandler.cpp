#include "pch.h"
#include "AttributedHandler.h"
#include "AttributedWrapper.h"
#include "FactoryManager.h"
#include "Reaction.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for AttributedHandler
		*/
		IParseHandler* AttributedHandler::create() const
		{
			return new AttributedHandler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void AttributedHandler::initialize()
		{
			initialized = true;
		}

		/**
		 * @brief Override for IParseHandler::start
		 * @param key: The key of the value
		 * @param value: The value to parse
		 * @param wrapper: The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool AttributedHandler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			if (attributedWrapper == nullptr) return false;

			if (attributedWrapper->getDepth() >= attributedWrapper->maxDepth)
			{
				attributedWrapper->maxDepth = attributedWrapper->getDepth();
			}

			// If not, we append the scope to the current scope
			if (hasPrefix(key, "obj_"))
			{
				Scope& subTable = attributedWrapper->getCurrentSubTable()->appendScope(key);
				attributedWrapper->setCurrentSubTable(&subTable);

				++startCount;
				return true;
			}
			else
			{
				Scope* currentTable = attributedWrapper->getCurrentSubTable();
				Scope* subTable = FactoryManager<Scope>::create(getPrefix(key));
				attributedWrapper->setCurrentSubTable(subTable);

				// GameObject have a special case where we add the action to the GameObject
				GameObject* gameObject = currentTable->As<GameObject>();
				ActionList* actionList = currentTable->As<ActionList>();

				GameObject* subGameObject = subTable->As<GameObject>();
				Action* action = subTable->As<Action>();
				Reaction* reaction = subTable->As<Reaction>();
				// test for reaction first
				if (gameObject != nullptr && reaction != nullptr)
				{
					gameObject->addReaction(reaction, getPrefix(key));
				}
				else if (gameObject != nullptr && action != nullptr)
				{
					gameObject->addAction(action, getPrefix(key));
				}
				else if (actionList != nullptr && action != nullptr)
				{
					actionList->addAction(action, getPrefix(key));
				}
				else if (gameObject != nullptr && subGameObject != nullptr)
				{
					gameObject->addChild(subGameObject, key);
				}
				else 
				{
					currentTable->adopt(std::move(subTable), key);
				}

				++startCount;
				return true;
			}

			return false;
		}

		/**
		 * @brief Override for IParseHandler::end
		 * @param key: The key of the value
		 * @param wrapper: The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool AttributedHandler::end(const std::string& key, ParseCoordinator::Wrapper* wrapper)
		{
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			if (attributedWrapper == nullptr) return false;

			// Hardcoded: If reaction, hook it up to Event Dispatcher
			Reaction* reaction = attributedWrapper->getCurrentSubTable()->As<Reaction>();
			if (reaction != nullptr)
			{
				reaction->SubscribeToCorrespondingEvent();
			}

			if (hasPrefix(key, "obj_") || 
				(getPrefix(key) != "" && getPrefix(key).find_first_not_of(" \t\n\v\f\r") != std::string::npos))
			{
				attributedWrapper->popSubTable();

				++endCount;
				return true;
			}

			return false;
		}
	}
}