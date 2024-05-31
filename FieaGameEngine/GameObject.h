#pragma once
#include <vector>
#include "Attributed.h"
#include "Action.h"
#include "Transform.h"
#include "Reaction.h"

namespace Fiea
{
	namespace GameEngine
	{
		struct GameTime;

		class GameObject : public Attributed
		{
			RTTI_DECLARATIONS(GameObject, Attributed);

		protected:
			// Specialized constructor for child classes to set the type of the object
			GameObject(IdType type);

		public:

			// Default constructor
			GameObject();

			// Copy constructor
			GameObject(const GameObject& other);

			// Move constructor
			GameObject(GameObject&& other) noexcept;

			// Copy assignment operator
			GameObject& operator=(const GameObject& rhs);

			// Move assignment operator
			GameObject& operator=(GameObject&& rhs) noexcept;

			// Cloneable interface
			[[nodiscard]] virtual GameObject* clone() const override;

			// Destructor
			virtual ~GameObject() override = default;

			// Enable function to be called when the object is enabled
			virtual void onEnable();

			// Disable function to be called when the object is disabled
			virtual void onDisable();

			// Update function to be called every frame
			virtual void update(const GameTime& tick);

			// Add a child to the current object
			void addChild(GameObject* child, const std::string& key = "");

			// Remove a child from the current object by key
			void removeChild(const std::string& key, const std::string& name = "");

			// Get child by key
			GameObject* getChild(const std::string& key, const std::string& name = "");

			// Get the const child by key
			const GameObject* getChild(const std::string& key, const std::string& name = "") const;

			// Get parent of the current object
			GameObject* getParentGameObject();

			const GameObject* getParentGameObject() const;

			// Create and add an Action class from class name
			Action* createAction(const std::string& className, const std::string& instanceName = "");

			// Add an action to the current object
			void addAction(Action* action, const std::string& key = "");

			// Remove an action from the current object by key
			void removeAction(const std::string& key, const std::string& name = "");

			// Get action by key
			Action* getAction(const std::string& key, const std::string& name = "");

			// Get the const action by key
			const Action* getAction(const std::string& key, const std::string& name = "") const;

			// Add a reaction to the curent object
			void addReaction(Reaction* reaction, const std::string& key = "");

			// Append a Datum to the Scope of current object, if it already exists, return the existing Datum
			Datum& appendAuxiliaryAttribute(const std::string& key) override;

			// Get the prescribed attributes of the GameObject class
			static std::vector<Signature> prescribedAttributes();

			// Register prescribed attributes to TypeManager
			static bool registerSignatures();

			//RTTI Overrides
			virtual std::string ToString() const override;

			// Public data members
			std::string name = "GameObject";
			Transform transform;

			bool enabled = false;
			bool alive = true;
			
		protected:
			// GameObject children container Scope
			Scope* children;

			// Actions container Scope
			Scope* actions;

			Scope* reactions;

			// Parent game object
			GameObject* parent = nullptr;

		private:
			int nextChildIndex = 1;
			int nextActionIndex = 1;

			// Orphan the child
			GameObject* orphanChild(GameObject* child);

			// Orphan the action
			Action* orphanAction(Action* action);

			// deep copy helper function
			void deepCopy(const GameObject& other);

			// deep reparent helper function
			void deepReparent();

		};
	}
}

