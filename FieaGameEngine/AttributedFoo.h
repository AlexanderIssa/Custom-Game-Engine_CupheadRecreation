#pragma once
#include <vector>
#include "Attributed.h"

namespace Fiea
{
	namespace GameEngine
	{
		class AttributedFoo : public Attributed
		{
			RTTI_DECLARATIONS(AttributedFoo, Attributed);

		protected:
			// Specialized constructor for child classes to set the type of the object
			AttributedFoo(IdType type);
			
		public:
			// Default constructor
			AttributedFoo();

			// Copy constructor
			AttributedFoo(const AttributedFoo& other) = default;

			// Move constructor
			AttributedFoo(AttributedFoo&& other) noexcept = default;

			// Copy assignment operator
			AttributedFoo& operator=(const AttributedFoo& rhs) = default;

			// Move assignment operator
			AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;

			// Cloneable interface
			[[nodiscard]] AttributedFoo* clone() const override;

			// Destructor
			virtual ~AttributedFoo() override = default;

			// Append a Datum to the Scope of current object, if it already exists, return the existing Datum
			Datum& appendAuxiliaryAttribute(const std::string& key) override;

			// Get the prescribed attributes of the AttributedFoo class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

			// Register prescribed attributes to TypeManager
			static bool registerSignatures();

		private:
			static const size_t arraySize = 10;

		public:
			int externalInteger = 0;
			float externalFloat = 0.0f;
			std::string externalString = "";

			int externalIntegerArray[arraySize];
			float externalFloatArray[arraySize];
			std::string externalStringArray[arraySize];
		};
	}
}