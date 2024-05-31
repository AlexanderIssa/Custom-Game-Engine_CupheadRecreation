#include"pch.h"
#include "ActionList.h"
#include "FactoryManager.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(ActionList);

		/*
		* Default constructor for ActionList
		*/
		ActionList::ActionList() : Action(ActionList::TypeIdClass()) 
		{
			actions = new Scope();
			adopt(std::move(actions), "obj_actions");
		}

		/*
		* Specialized constructor for child classes to set the type of the object
		*/
		ActionList::ActionList(IdType type) : Action(type)
		{
			actions = new Scope();
			adopt(std::move(actions), "obj_actions");
		}

		/*
		* Clone function to be called when a copy of the object is needed
		* @return - A new ActionList object that is a copy of this object
		*/
		ActionList* ActionList::clone() const
		{
			return new ActionList(*this);
		}

		/*
		* onEnable function to be called when the object is enabled
		*/
		void ActionList::onEnable()
		{
		}

		/*
		* onDisable function to be called when the object is disabled
		*/
		void ActionList::onDisable()
		{
			if (getParentGameObject()->alive)
			{
				actions->clear();
			}
		}

		void ActionList::update(const GameTime& tick)
		{
			for (size_t i = 0; i < actions->getSize(); i++)
			{
				for (size_t j = 0; j < (*actions)[i].getSize(); j++)
				{
					Action* curAction = (*actions)[i].get<Scope*>(j)->As<Action>();
					if (curAction != nullptr)
					{
						curAction->update(tick);
					}	
				}
			}
		}

		/*
		* Create and add an Action class from class name
		* @param className - The name of the Action class to create
		* @param instanceName - The name of the instance of the Action class to create
		*/
		Action* ActionList::createAction(const std::string& className, const std::string& instanceName)
		{
			Action* action = FactoryManager<Scope>::create(className)->As<Action>();
			addAction(action, instanceName);
			return action;
		}

		/*
		* Add an Action to this ActionList
		* @param action - The Action to add
		* @param key - The key to add the Action with
		*/
		void ActionList::addAction(Action* action, const std::string& key)
		{
			if (action == nullptr)
			{
				throw std::invalid_argument("Cannot add a nullptr as an Action");
			}

			// Name the Action
			if (key == "" || key.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			{
				action->name = action->ToString();
			}
			else
			{
				action->name = key;
			}

			/*size_t index = 0;
			if (actions->findContainedScope(action, index) != nullptr || actions->find(action->name) != nullptr)
			{
				throw std::invalid_argument("Action already exists in this ActionList");
			}*/

			// Transfer ownership of the Action to this ActionList
			actions->adopt(std::move(action), action->name);
		}

		/*
		* Remove an Action from this ActionList
		* @param key - The key of the Action to remove
		*/
		void ActionList::removeAction(const std::string& key)
		{
			Datum* datum = actions->find(key);
			if (datum == nullptr || datum->getSize() == 0 || (*datum)[0].As<Action>() == nullptr)
			{
				throw std::invalid_argument("Action does not exist in this ActionList");
			}

			datum->remove(0);
		}

		/*
		* Get the Action with the given key
		* @param key - The key of the Action to get
		*/
		Action* ActionList::getAction(const std::string& key, size_t index)
		{
			Datum* datum = actions->find(key);
			if (datum == nullptr || datum->getSize() == 0)
			{
				throw std::invalid_argument("Action does not exist in this ActionList");
			}

			return datum->get<Scope*>(index)->As<Action>();
		}

		/*
		* Get the Action with the given key
		* @param key - The key of the Action to get
		*/
		const Action* ActionList::getAction(const std::string& key) const
		{
			const Datum* datum = actions->find(key);
			if (datum == nullptr || datum->getSize() == 0)
			{
				throw std::invalid_argument("Action does not exist in this ActionList");
			}

			return datum->get<Scope*>(0)->As<Action>();
		}

		/*
		* Prescribed attributes for every ActionList object
		* @return - A vector of Signatures representing the prescribed attributes for ActionList
		*/
		std::vector<Signature> ActionList::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "obj_actions"s, Datum::DataType::TABLE, 0, 0 }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string ActionList::ToString() const
		{
			return "ActionList"s;
		}
	}
}