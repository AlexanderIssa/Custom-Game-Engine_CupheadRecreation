#include "pch.h"
#include "AttributedFoo.h"
#include "TypeManager.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(AttributedFoo);

		/*
		* Default constructor
		*/
		AttributedFoo::AttributedFoo() : Attributed(TypeIdClass()){}

		/*
		* Specialized constructor
		*/
		AttributedFoo::AttributedFoo(IdType type) : Attributed(type), externalIntegerArray(), externalFloatArray(), externalStringArray() {}

		/*
		* Clone method for AttributedBar
		* @param other - The AttributedFoo object to copy from
		*/
		AttributedFoo* AttributedFoo::clone() const
		{
			return new AttributedFoo(*this);
		}

		/*
		* Append a Datum to this Scope with the given name, and return a reference to that Datum. 
		* If a Datum with the given name already exists, return the existing Datum.
		* @param key - The name of the Datum to append
		* @return - A reference to the Datum appended
		*/
		Datum& AttributedFoo::appendAuxiliaryAttribute(const std::string& key)
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
		bool AttributedFoo::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		/*
		* Prescribed attributes for every AttributedFoo object
		* @return - A vector of Signatures representing the prescribed attributes for AttributedFoo
		*/
		std::vector<Signature> AttributedFoo::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "externalInteger"s, Datum::DataType::INTEGER, 1, offsetof(AttributedFoo, externalInteger) },
				{ "externalFloat"s, Datum::DataType::FLOAT, 1, offsetof(AttributedFoo, externalFloat) },
				{ "externalString"s, Datum::DataType::STRING, 1, offsetof(AttributedFoo, externalString) },
				{ "externalIntegerArray"s, Datum::DataType::INTEGER, arraySize, offsetof(AttributedFoo, externalIntegerArray) },
				{ "externalFloatArray"s, Datum::DataType::FLOAT, arraySize, offsetof(AttributedFoo, externalFloatArray) },
				{ "externalStringArray"s, Datum::DataType::STRING, arraySize, offsetof(AttributedFoo, externalStringArray) }
			};
		}

		// ========================= RTTI Overrides =========================
		
		std::string AttributedFoo::ToString() const
		{
			return "AttributedFoo"s;
		}
	}
}