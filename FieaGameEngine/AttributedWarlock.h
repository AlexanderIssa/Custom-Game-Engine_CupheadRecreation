#pragma once
#include <vector>
#include "Attributed.h"

namespace Fiea
{
	namespace GameEngine
	{
		class AttributedWarlock final : public Attributed
		{
			RTTI_DECLARATIONS(AttributedWarlock, Attributed);

		public:

			// Default constructor
			AttributedWarlock();

			// Copy constructor
			AttributedWarlock(const AttributedWarlock& other) = default;

			// Move constructor
			AttributedWarlock(AttributedWarlock&& other) noexcept = default;

			// Copy assignment operator
			AttributedWarlock& operator=(const AttributedWarlock& rhs) = default;

			// Move assignment operator
			AttributedWarlock& operator=(AttributedWarlock&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] AttributedWarlock* clone() const override;

			// Destructor
			virtual ~AttributedWarlock() = default;

			// Append a Datum to the Scope of current object, if it already exists, return the existing Datum
			Datum& appendAuxiliaryAttribute(const std::string& key) override;

			// Get the prescribed attributes of the AttributedWarlock class
			static std::vector<Signature> prescribedAttributes();

			// Register prescribed attributes to TypeManager
			static bool registerSignatures();

			//RTTI Overrides
			virtual std::string ToString() const override;

			// Public data members
			std::string name = "";
			int age = 0;
			float manaPoints = 0.0f;
			Scope spellBook;
			glm::vec4 startPosition = glm::vec4(0.0f);
			glm::mat4 transform4D = glm::mat4(0.0f);
		};
	}
}