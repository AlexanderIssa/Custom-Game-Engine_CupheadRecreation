#include "pch.h"
#include "GameObject.h"
#include "TypeManager.h"
#include "FactoryManager.h"
#include "GameClock.h"
#include <cstddef>
#include <iostream>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(GameObject);

		/*
		* Default constructor
		*/
		GameObject::GameObject() : Attributed(TypeIdClass()) 
		{
			// Set the parent of the children and actions to this GameObject
			children = new Scope();
			actions = new Scope();
			reactions = new Scope();
			parent = nullptr;
			adopt(std::move(children), "obj_children");
			adopt(std::move(actions), "obj_actions");
			adopt(std::move(reactions), "obj_reactions");
		}

		/*
		* Copy constructor
		* @param other - The GameObject to copy
		*/
		GameObject::GameObject(const GameObject& other) : Attributed(other), transform(other.transform), parent(nullptr)
		{
			// Set the parent of the children and actions to this GameObject
			children = new Scope();
			actions = new Scope();
			reactions = new Scope();
			
			adopt(std::move(children), "obj_children");
			adopt(std::move(actions), "obj_actions");
			adopt(std::move(reactions), "obj_reactions");

			deepCopy(other);
			deepReparent();
		}

		/*
		* Move constructor
		* @param other - The GameObject to move
		*/
		GameObject::GameObject(GameObject && other) noexcept : 
			Attributed(std::move(other)), transform(other.transform),
			children(new Scope(std::move(*other.children))), 
			actions(new Scope(std::move(*other.actions))),
			reactions(new Scope(std::move(*other.reactions))), 
			parent(nullptr)
		{
			adopt(std::move(children), "obj_children");
			adopt(std::move(actions), "obj_actions");
			adopt(std::move(reactions), "obj_reactions");

			deepReparent();

			other.children = nullptr;
			other.actions = nullptr;
			other.reactions = nullptr;
			other.parent = nullptr;
		}

		/*
		* Copy assignment operator
		* @param rhs - The GameObject to copy
		* @return - A reference to the GameObject after copying
		*/
		GameObject& GameObject::operator=(const GameObject& rhs)
		{
			if (this != &rhs)
			{
				Attributed::operator=(rhs);
				children = new Scope(*rhs.children);
				actions = new Scope(*rhs.actions);
				reactions = new Scope(*rhs.reactions);
				transform = rhs.transform;
				parent = nullptr;

				adopt(std::move(children), "obj_children");
				adopt(std::move(actions), "obj_actions");
				adopt(std::move(reactions), "obj_reactions");

				deepCopy(rhs);
				deepReparent();
			}
			return *this;
		}

		/*
		* Move assignment operator
		* @param rhs - The GameObject to move
		* @return - A reference to the GameObject after moving
		*/
		GameObject& GameObject::operator=(GameObject&& rhs) noexcept
		{
			if (this != &rhs)
			{
				Attributed::operator=(std::move(rhs));
				children = new Scope(std::move(*rhs.children));
				actions = new Scope(std::move(*rhs.actions));
				reactions = new Scope(std::move(*rhs.reactions));
				transform = rhs.transform;
				parent = nullptr;

				adopt(std::move(children), "obj_children");
				adopt(std::move(actions), "obj_actions");
				adopt(std::move(reactions), "obj_reactions");

				rhs.children = nullptr;
				rhs.actions = nullptr;
				rhs.reactions = nullptr;
				rhs.parent = nullptr;

				deepReparent();
			}
			return *this;
		}

		/*
		* Clone function to create a copy of this GameObject
		* @return - A pointer to the new GameObject
		*/
		GameObject* GameObject::clone() const
		{
			return new GameObject(*this);
		}

		/*
		* Specialized constructor for child classes to set the type of the object
		*/
		GameObject::GameObject(IdType type) : Attributed(type)
		{
			// Set the parent of the children and actions to this GameObject
			children = new Scope();
			actions = new Scope();
			reactions = new Scope();
			parent = nullptr;
			adopt(std::move(children), "obj_children");
			adopt(std::move(actions), "obj_actions");
			adopt(std::move(reactions), "obj_reactions");
		}

		/*
		* onEnable function to be called when the object is enabled
		*/
		void GameObject::onEnable()
		{
			// Call onEnable on all GameObject children
			for (int i = 0; i < children->getSize(); i++)
			{
				Datum& datum = (*children)[i];

				if (datum.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < datum.getSize(); j++)
					{
						datum[j].As<GameObject>()->onEnable();
					}
				}
			}

			// Call onEnable on all Actions
			for (int i = 0; i < actions->getSize(); i++)
			{
				Datum& datum = (*actions)[i];

				if (datum.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < datum.getSize(); j++)
					{
						datum[j].As<Action>()->onEnable();
					}
				}
			}
		}

		/*
		* onDisable function to be called when the object is disabled
		*/
		void GameObject::onDisable()
		{
			// Call onDisable on all GameObject children
			for (int i = 0; i < children->getSize(); i++)
			{
				Datum& datum = (*children)[i];

				if (datum.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < datum.getSize(); j++)
					{
						datum[j].As<GameObject>()->onDisable();
					}
				}
			}

			// Call onDisable on all Actions
			for (int i = 0; i < actions->getSize(); i++)
			{
				Datum& datum = (*actions)[i];

				if (datum.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < datum.getSize(); j++)
					{
						datum[j].As<Action>()->onDisable();
					}
				}
			}
		}

		/*
		* Update the GameObject and all its children
		* @param tick - The GameTime object representing the current time in the game
		*/
		void GameObject::update(const GameTime& time)
		{
			if (!enabled || !alive)
				return;

			// Call update on all GameObject children
			for (int i = 0; i < children->getSize(); i++)
			{
				Datum& datum = (*children)[i];

				if (datum.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < datum.getSize(); j++)
					{
						datum[j].As<GameObject>()->update(time);
					}
				}
			}

			// Call update on all Actions
			for (int i = 0; i < actions->getSize(); i++)
			{
				Datum& datum = (*actions)[i];

				if (datum.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < datum.getSize(); j++)
					{
						datum[j].As<Action>()->update(time);
					}
				}
			}
		}

		/*
		* Add a child GameObject to this GameObject
		* @param child - The GameObject to add as a child
		*/
		void GameObject::addChild(GameObject* child, const std::string& key)
		{
			if (child == nullptr)
			{
				throw std::invalid_argument("Cannot add a nullptr as a child GameObject");
			}

			size_t index = 0;
			Datum* childDatum = children->findContainedScope(child, index);
			if(childDatum != nullptr && childDatum->getSize()>0)
			{
				throw std::invalid_argument("Child already exists in this GameObject");
			}

			// Name the child GameObject
			if (key == "" || key.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			{
				child->name = child->ToString();
			}
			else
			{
				child->name = key;
			}

			GameObject* parent = child->parent;
			if (parent != nullptr)
			{
				parent->orphanChild(child);
			}

			// Transfer ownership of the child to this GameObject
			children->adopt(std::move(child), child->name);
			child->parent = this;
		}

		/*
		* Remove a child GameObject from this GameObject
		* @param key - The key of the child GameObject to remove
		*/
		void GameObject::removeChild(const std::string& key, const std::string& name)
		{
			Datum* datum = children->find(key);
			if (datum == nullptr || datum->getSize() == 0 || (*datum)[0].As<GameObject>() == nullptr)
			{
				throw std::invalid_argument("Child does not exist in this GameObject");
			}

			size_t index = 0;

			if (name != "" || name.find_first_of(" \t\n\v\f\r") == std::string::npos)
			{
				for (int i = 0; i < datum->getSize(); i++)
				{
					if ((*datum)[i].As<GameObject>()->name == name)
					{
						index = i;
					}
				}
			}

			datum->remove(index);
		}

		/*
		* Get the child GameObject with the given key
		* @param key - The key of the child GameObject to get
		* @return - A pointer to the child GameObject with the given key
		*/
		GameObject* GameObject::getChild(const std::string& key, const std::string& name)
		{
			Datum* datum = children->find(key);
			if (datum == nullptr || datum->getSize() == 0)
			{
				//throw std::invalid_argument("Child does not exist in this GameObject");
				std::cout << "Child does not exist in this GameObject" << std::endl;
				return nullptr;
			}

			if (name != "" || name.find_first_of(" \t\n\v\f\r") == std::string::npos)
			{
				for (int i = 0; i < datum->getSize(); i++)
				{
					if ((*datum)[i].As<GameObject>()->name == name)
					{
						return (*datum)[i].As<GameObject>();
					}
				}
			}

			return datum->get<Scope*>(0)->As<GameObject>();
		}

		/*
		* Get the child GameObject with the given key
		* @param key - The key of the child GameObject to get
		* @return - A const pointer to the child GameObject with the given key
		*/
		const GameObject* GameObject::getChild(const std::string& key, const std::string& name) const
		{
			Datum* datum = children->find(key);
			if (datum == nullptr || datum->getSize() == 0)
			{
				//throw std::invalid_argument("Child does not exist in this GameObject");
				std::cout << "Child does not exist in this GameObject" << std::endl;
				return nullptr;
			}

			if (name != "" || name.find_first_of(" \t\n\v\f\r") == std::string::npos)
			{
				for (int i = 0; i < datum->getSize(); i++)
				{
					if ((*datum)[i].As<GameObject>()->name == name)
					{
						return (*datum)[i].As<GameObject>();
					}
				}
			}

			return datum->get<Scope*>(0)->As<GameObject>();
		}

		/*
		* Get the parent of this GameObject
		* @return - A pointer to the parent GameObject
		*/
		GameObject* GameObject::getParentGameObject()
		{
			return parent;
		}

		/*
		* Const version of getParentGameObject
		* @return - A const pointer to the parent GameObject
		*/
		const GameObject* GameObject::getParentGameObject() const
		{
			return parent;
		}

		/*
		* Create and add an Action class from class name
		* @param className - The name of the Action class to create
		* @param instanceName - The name of the instance of the Action class to create
		*/
		Action* GameObject::createAction(const std::string& className, const std::string& instanceName)
		{
			Action* action = FactoryManager<Scope>::create(className)->As<Action>();
			addAction(action, instanceName);
			return action;
		}

		/* 
		* Add an Action to this GameObject
		* @param action - The Action to add
		* @param key - The key to add the Action with
		*/
		void GameObject::addAction(Action* action, const std::string& key)
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

			size_t index = 0;
			if (actions->findContainedScope(action, index) != nullptr 
				|| actions->find(action->name) != nullptr)
			{
				throw std::invalid_argument("Action already exists in this GameObject");
			}

			// Orphan the Action from its parent GameObject
			GameObject* parent = action->getParentGameObject();
			if(parent != nullptr)
			{
				parent->orphanAction(action);
			}

			// Transfer ownership of the Action to this GameObject
			actions->adopt(std::move(action), action->name);
			action->setParentGameObject(this);
		}

		/*
		* Remove an Action from this GameObject
		* @param key - The key of the Action to remove
		*/
		void GameObject::removeAction(const std::string& key, const std::string& name)
		{
			Datum* datum = actions->find(key);
			if (datum == nullptr || datum->getSize() == 0 || (*datum)[0].As<Action>() == nullptr)
			{
				throw std::invalid_argument("Action does not exist in this GameObject");
			}

			size_t index = 0;

			if (name != "" || name.find_first_of(" \t\n\v\f\r") == std::string::npos)
			{
				for (int i = 0; i < datum->getSize(); i++)
				{
					if ((*datum)[i].As<GameObject>()->name == name)
					{
						index = i;
					}
				}
			}

			datum->remove(index);
		}

		/*
		* Get the Action with the given key
		* @param key - The key of the Action to get
		*/
		Action* GameObject::getAction(const std::string& key, const std::string& name)
		{
			Datum* datum = actions->find(key);
			if (datum == nullptr || datum->getSize() == 0)
			{
				//throw std::invalid_argument("Action does not exist in this GameObject");
				std::cout << "Action does not exist in this GameObject" << std::endl;
				return nullptr;
			}

			if (name != "" || name.find_first_of(" \t\n\v\f\r") == std::string::npos)
			{
				for (int i = 0; i < datum->getSize(); i++)
				{
					if ((*datum)[i].As<Action>()->name == name)
					{
						return (*datum)[i].As<Action>();
					}
				}
			}

			return datum->get<Scope*>(0)->As<Action>();
		}

		/*
		* Get the Action with the given key
		* @param key - The key of the Action to get
		*/
		const Action* GameObject::getAction(const std::string& key, const std::string& name) const
		{
			Datum* datum = actions->find(key);
			if (datum == nullptr || datum->getSize() == 0)
			{
				//throw std::invalid_argument("Action does not exist in this GameObject");
				std::cout << "Action does not exist in this GameObject" << std::endl;
				return nullptr;
			}

			if (name != "" || name.find_first_of(" \t\n\v\f\r") == std::string::npos)
			{
				for (int i = 0; i < datum->getSize(); i++)
				{
					if ((*datum)[i].As<Action>()->name == name)
					{
						return (*datum)[i].As<Action>();
					}
				}
			}

			return datum->get<Scope*>(0)->As<Action>();
		}

		void Fiea::GameEngine::GameObject::addReaction(Reaction * reaction, const std::string & key)
		{
			if (reaction == nullptr)
			{
				throw std::invalid_argument("Cannot add a nullptr as an reaction");
			}

			// Name the Action
			if (key == "" || key.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			{
				reaction->name = reaction->ToString();
			}
			else
			{
				reaction->name = key;
			}

			// Note to Peter: I commented this out. It is ok to have multiple reactions on one gameobject
			/*
			size_t index = 0;
			if (reactions->findContainedScope(reaction, index) != nullptr 
				|| reactions->find(reaction->name) != nullptr)
			{
				throw std::invalid_argument("reaction already exists in this GameObject");
			}
			*/

			// Transfer ownership of the Reaction to this GameObject
			reactions->adopt(std::move(reaction), reaction->name);
			reaction->setParentGameObject(this);
		}

		/*
		* Append a Datum to this Scope with the given name, and return a reference to that Datum.
		* If a Datum with the given name already exists, return the existing Datum.
		* @param key - The name of the Datum to append
		* @return - A reference to the Datum appended
		*/
		Datum& GameObject::appendAuxiliaryAttribute(const std::string& key)
		{
			bool foundAuxiliary = false;

			for (auto& sig : prescribedAttributes())
			{
				if (sig.Name == key)
				{
					throw std::invalid_argument("Cannot append a prescribed attribute as an auxiliary attribute");
				}
			}

			for (auto& sig : auxiliaryAttributes)
			{
				if (sig.Name == key)
				{
					foundAuxiliary = true;
					break;
				}
			}

			if (!foundAuxiliary)
			{
				Signature signature;
				signature.Name = key;
				signature.Type = Datum::DataType::UNKNOWN;
				signature.Size = 0;
				signature.Offset = 0;

				auxiliaryAttributes.push_back(signature);
			}

			return append(key);
		}

		/*
		* Register the prescribed attributes to TypeManager
		*/
		bool GameObject::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		/*
		* Prescribed attributes for every GameObject object
		* @return - A vector of Signatures representing the prescribed attributes for GameObject
		*/
		std::vector<Signature> GameObject::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_name"s, Datum::DataType::STRING, 1, offsetof(GameObject, name) },
				{ "vec4_position"s, Datum::DataType::VEC4, 1, offsetof(GameObject, transform.position) },
				{ "vec4_rotation"s, Datum::DataType::VEC4, 1, offsetof(GameObject, transform.rotation) },
				{ "vec4_scale"s, Datum::DataType::VEC4, 1, offsetof(GameObject, transform.scale) },
				{ "obj_children"s, Datum::DataType::TABLE, 0, 0 },
				{ "obj_actions"s, Datum::DataType::TABLE, 0, 0 },
				{ "obj_reactions"s, Datum::DataType::TABLE, 0, 0 }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string GameObject::ToString() const
		{
			return "GameObject"s;
		}

		// ========================= Private Helper Functions =========================

		/*
		* Orphan a child GameObject from this GameObject
		* @param child - The child GameObject to orphan
		* @return - A pointer to the orphaned child GameObject
		*/
		GameObject* GameObject::orphanChild(GameObject* child)
		{
			if (child == nullptr)
			{
				throw std::invalid_argument("Cannot orphan a nullptr as a child GameObject");
			}

			GameObject* orphanedChild = child;

			size_t index = 0;
			size_t datumIndex = 0;
			Datum* datum = children->findContainedScope(child, index, datumIndex);

			if (datum == nullptr)
			{
				throw std::runtime_error("Child does not exist in this GameObject");
			}

			datum->remove(datumIndex);

			return orphanedChild;
		}

		/*
		* Orphan an Action from this GameObject
		* @param action - The Action to orphan
		* @return - A pointer to the orphaned Action
		*/
		Action* GameObject::orphanAction(Action* action)
		{
			if (action == nullptr)
			{
				throw std::invalid_argument("Cannot orphan a nullptr as a child GameObject");
			}

			Action* orphanedAction = action;

			size_t index = 0;
			size_t datumIndex = 0;
			Datum* datum = actions->findContainedScope(action, index, datumIndex);

			if (datum == nullptr)
			{
				throw std::runtime_error("Action does not exist in this GameObject");
			}

			datum->remove(datumIndex);

			return orphanedAction;
		}

		/*
		* Deep copy helper function
		* @param other - The GameObject to deep copy
		*/
		void GameObject::deepCopy(const GameObject& other)
		{
			// Deep copy the children
			for (int i = 0; i < other.children->getSize(); i++)
			{
				Datum& childDatum = (*other.children)[i];
				for (int j = 0; j < childDatum.getSize(); j++)
				{
					GameObject* child = childDatum[j].As<GameObject>();
					if (child != nullptr)
					{
						addChild(child->clone(), child->name);
					}
				}
			}

			// Deep copy the actions
			for (int i = 0; i < other.actions->getSize(); i++)
			{
				Datum& actionDatum = (*other.actions)[i];
				for (int j = 0; j < actionDatum.getSize(); j++)
				{
					Action* action = actionDatum[j].As<Action>();
					if (action != nullptr)
					{
						addAction(action->clone(), action->name);
					}
				}
			}
		}

		/*
		* Deep reparent the children and actions of this GameObject
		*/
		void GameObject::deepReparent()
		{
			// Deep reparent the children
			for (int i = 0; i < children->getSize(); i++)
			{
				Datum& childDatum = (*children)[i];
				for (int j = 0; j < childDatum.getSize(); j++)
				{
					GameObject* child = childDatum[j].As<GameObject>();
					if (child != nullptr)
					{
						child->parent = this;
					}
				}
			}

			// Deep reparent the actions
			for (int i = 0; i < actions->getSize(); i++)
			{
				Datum& actionDatum = (*actions)[i];
				for (int j = 0; j < actionDatum.getSize(); j++)
				{
					Action* action = actionDatum[j].As<Action>();
					if (action != nullptr)
					{
						action->setParentGameObject(this);
					}
				}
			}
		}
	}
}