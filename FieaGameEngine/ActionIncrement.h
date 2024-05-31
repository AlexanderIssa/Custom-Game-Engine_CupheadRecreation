#pragma once
#include <vector>
#include "Action.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ActionIncrement final : public Action
		{
			RTTI_DECLARATIONS(ActionIncrement, Action);

		public:

			// Default constructor
			ActionIncrement();

			// Copy constructor
			ActionIncrement(const ActionIncrement& other) = default;

			// Move constructor
			ActionIncrement(ActionIncrement&& other) noexcept = default;

			// Copy assignment operator
			ActionIncrement& operator=(const ActionIncrement& rhs) = default;

			// Move assignment operator
			ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] ActionIncrement* clone() const override;

			// Destructor
			virtual ~ActionIncrement() override = default;

			// Enable function to be called when the object is enabled
			virtual void onEnable() override;

			// Disable function to be called when the object is disabled
			virtual void onDisable() override;

			// Update function to be called every frame
			virtual void update(const GameTime&) override;

			// Get the prescribed attributes of the ActionIncrement class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

			std::string key = "";
			float value;
		};
	}
}