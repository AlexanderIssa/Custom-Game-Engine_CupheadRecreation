#pragma once
#include "ActionList.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ActionListWhile final : public ActionList
		{
			RTTI_DECLARATIONS(ActionListWhile, ActionList);

		public:
			// Default constructor
			ActionListWhile();

			// Copy constructor
			ActionListWhile(const ActionListWhile& other) = default;

			// Move constructor
			ActionListWhile(ActionListWhile&& other) noexcept = default;

			// Copy assignment operator
			ActionListWhile& operator=(const ActionListWhile& rhs) = default;

			// Move assignment operator
			ActionListWhile& operator=(ActionListWhile&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] ActionListWhile* clone() const override;

			// Destructor
			~ActionListWhile() override = default;

			// Enable function to be called when the object is enabled
			void onEnable() override;

			// Disable function to be called when the object is disabled
			void onDisable() override;

			// Update function to be called every frame
			void update(const GameTime&) override;

			// Set the condition of the while loop
			void SetCondition(int num);

			// Set the preamble of the while loop
			void SetPreamble(Action* action);

			// Set the increment of the while loop
			void SetIncrement(Action* action);

			// Get the prescribed attributes of the ActionListWhile class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

		private:
			int condition = 0;
			Action* preamble = nullptr;
			Action* increment = nullptr;
			
			bool isConditionSet = false;
		};
	}
}
