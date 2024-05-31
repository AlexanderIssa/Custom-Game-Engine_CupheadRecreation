#pragma once
#include <vector>
#include "Scope.h"
#include "Signature.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Attributed : public Scope
		{
			RTTI_DECLARATIONS(Attributed, Scope);

		public:
			// Default constructor
			Attributed(IdType type);

			// Copy constructor
			Attributed(const Attributed& other);

			// Move constructor
			Attributed(Attributed&& other) noexcept;

			// Copy assignment operator
			Attributed& operator=(const Attributed& other);

			// Move assignment operator
			Attributed& operator=(Attributed&& other) noexcept;

			// Cloneable interface
			[[nodiscard]] virtual Attributed* clone() const override = 0;

			// Equality operator
			bool operator==(const Attributed& other) const;

			// Inequality operator
			bool operator!=(const Attributed& other) const;

			// Return true if the key refers to either a prescribed attribute or an auxiliary attribute
			bool isAttribute(const std::string& key) const;

			// Return true if the key refers to a prescribed attribute
			bool isPrescribedAttribute(const std::string& key) const;

			// Return true if the key refers to an auxiliary attribute
			bool isAuxiliaryAttribute(const std::string& key) const;

			// Append a Datum to the Scope of current object, if it already exists, return the existing Datum
			virtual Datum& appendAuxiliaryAttribute(const std::string& key) = 0;

			// Destructor
			virtual ~Attributed() = default;

		protected:
			// I could just use a value pair vector, but I want to use the Signature class
			// so later when we implement those accessors, we can all return a vector of Signatures
			std::vector<Signature> auxiliaryAttributes;

		private:
			// Initialize the prescribed attributes
			void initializePrescribedAttributes(size_t type);

			RTTI* selfPtr;
		};
	}
}