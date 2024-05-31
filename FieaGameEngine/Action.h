#pragma once
#include <vector>
#include "Attributed.h"

namespace Fiea
{
	namespace GameEngine
	{
		struct GameTime;
		class GameObject;

		class Action : public Attributed
		{
			RTTI_DECLARATIONS(Action, Attributed);

		protected:
			// Specialized constructor for child classes to set the type of the object
			Action(IdType type);

		public:

			// Default constructor
			Action();

			// Copy constructor
			Action(const Action& other);

			// Move constructor
			Action(Action&& other) noexcept;

			// Copy assignment operator
			Action& operator=(const Action& rhs);

			// Move assignment operator
			Action& operator=(Action&& rhs) noexcept;

			// Cloneable interface
			[[nodiscard]] virtual Action* clone() const override = 0;

			// Destructor
			virtual ~Action() = default;

			// Enable function to be called when the object is enabled
			virtual void onEnable() = 0;

			// Disable function to be called when the object is disabled
			virtual void onDisable() = 0;

			// Update function to be called every frame
			virtual void update(const GameTime&) = 0;

			// Append a Datum to the Scope of current object, if it already exists, return the existing Datum
			Datum& appendAuxiliaryAttribute(const std::string& key) override;

			// Get the prescribed attributes of the Action class
			static std::vector<Signature> prescribedAttributes();

			// Register prescribed attributes to TypeManager
			static bool registerSignatures();

			//RTTI Overrides
			virtual std::string ToString() const override;

			// Public data members
			std::string name = "Action";

			glm::vec4 getParentPosition();
			glm::vec4 getParentRotation();
			glm::vec4 getParentScale();

			GameObject* getParentGameObject();
			void setParentGameObject(GameObject* parent);

		private:
			// We do lazy initialization of the parentGameObject pointer
			// Call searchRootScope() method from Scope class to get the GameObject pointer if needed
			// ActionIncrement.cpp has an example of how to use this method
			GameObject* parentGameObject { nullptr };
		};
	}
}
