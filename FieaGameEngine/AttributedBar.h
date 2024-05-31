#pragma once
#include <vector>
#include "AttributedFoo.h"

namespace Fiea
{
	namespace GameEngine
	{
		class AttributedBar final : public AttributedFoo
		{
			RTTI_DECLARATIONS(AttributedBar, AttributedFoo);

		public:

			// Default constructor
			AttributedBar();

			// Copy constructor
			AttributedBar(const AttributedBar& other) = default;

			// Move constructor
			AttributedBar(AttributedBar&& other) noexcept = default;

			// Copy assignment operator
			AttributedBar& operator=(const AttributedBar& rhs) = default;

			// Move assignment operator
			AttributedBar& operator=(AttributedBar&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] AttributedBar* clone() const override;

			// Destructor
			~AttributedBar() override = default;

			// Get the prescribed attributes of the AttributedBar class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

			std::string externalStringChild = "";
		};
	}
}