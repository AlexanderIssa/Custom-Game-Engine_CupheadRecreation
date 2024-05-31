#pragma once
#include "Action.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ActionList : public Action
		{
			RTTI_DECLARATIONS(ActionList, Action);

		protected:
			// Specialized constructor for child classes to set the type of the object
			ActionList(IdType type);

		public:

			// Default constructor
			ActionList();

			// Copy constructor
			ActionList(const ActionList& other) = default;

			// Move constructor
			ActionList(ActionList&& other) noexcept = default;

			// Copy assignment operator
			ActionList& operator=(const ActionList& rhs) = default;

			// Move assignment operator
			ActionList& operator=(ActionList&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] virtual ActionList* clone() const override;

			// Destructor
			virtual ~ActionList() override = default;

			// Enable function to be called when the object is enabled
			virtual void onEnable() override;

			// Disable function to be called when the object is disabled
			virtual void onDisable() override;

			// Update function to be called every frame
			virtual void update(const GameTime&) override;

			// Create and add an Action class from class name
			Action* createAction(const std::string& className, const std::string& instanceName = "");

			// Add an action to the current object
			void addAction(Action* action, const std::string& key = "");

			// Remove an action from the current object by key
			void removeAction(const std::string& key);

			// Get action by key
			Action* getAction(const std::string& key, size_t index = 0);

			// Get the const action by key
			const Action* getAction(const std::string& key) const;

			// Get the prescribed attributes of the ActionList class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

		protected:
			Scope* actions;
		};
	}
}
