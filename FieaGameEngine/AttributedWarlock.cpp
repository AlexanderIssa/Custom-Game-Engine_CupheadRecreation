#include "pch.h"
#include "AttributedWarlock.h"
#include "TypeManager.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(AttributedWarlock);

		/*
		* Default constructor
		*/
		AttributedWarlock::AttributedWarlock() : Attributed(TypeIdClass()) {}

		/*
		* Clone function to create a copy of the current object
		* @return - A pointer to the cloned object
		*/
		AttributedWarlock* AttributedWarlock::clone() const
		{
			return new AttributedWarlock(*this);
		}

		/*
		* Append a Datum to this Scope with the given name, and return a reference to that Datum.
		* If a Datum with the given name already exists, return the existing Datum.
		* @param key - The name of the Datum to append
		* @return - A reference to the Datum appended
		*/
		Datum& AttributedWarlock::appendAuxiliaryAttribute(const std::string& key)
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
		bool AttributedWarlock::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		/*
		* Prescribed attributes for every AttributedWarlock object
		* @return - A vector of Signatures representing the prescribed attributes for AttributedWarlock
		*/
		std::vector<Signature> AttributedWarlock::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "name"s, Datum::DataType::STRING, 1, offsetof(AttributedWarlock, name) },
				{ "age"s, Datum::DataType::INTEGER, 1, offsetof(AttributedWarlock, age) },
				{ "manaPoints"s, Datum::DataType::FLOAT, 1, offsetof(AttributedWarlock, manaPoints) },
				{ "spellBook"s, Datum::DataType::TABLE, 1, offsetof(AttributedWarlock, spellBook)},
				{ "startPosition"s, Datum::DataType::VEC4, 1, offsetof(AttributedWarlock, startPosition) },
				{ "transform4D"s, Datum::DataType::MAT4, 1, offsetof(AttributedWarlock, transform4D) }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string AttributedWarlock::ToString() const
		{
			return "AttributedWarlock"s;
		}
	}
}