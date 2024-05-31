#include "pch.h"
#include "Action.h"
#include "GameObject.h"
#include "TypeManager.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(Action);

		/*
		* Default constructor
		*/
		Action::Action() : Attributed(TypeIdClass()) {}

		/*
		* Copy constructor
		* @param other - The Action object to copy from
		*/
		Action::Action(const Action& other) : Attributed(other)
		{
			parentGameObject = nullptr;
		}

		/*
		* Move constructor
		* @param other - The Action object to move from
		*/
		Action::Action(Action&& other) noexcept : Attributed(std::move(other)), parentGameObject(nullptr)
		{
			other.parentGameObject = nullptr;
		}

		/*
		* Copy assignment operator
		* @param other - The Action object to copy from
		*/
		Action& Action::operator=(const Action& rhs)
		{
			if (this != &rhs)
			{
				Attributed::operator=(rhs);
				parentGameObject = nullptr;
			}

			return *this;
		}

		/*
		* Move assignment operator
		* @param other - The Action object to move from
		*/
		Action& Action::operator=(Action&& rhs) noexcept
		{
			if (this != &rhs)
			{
				Attributed::operator=(std::move(rhs));
				parentGameObject = nullptr;
				rhs.parentGameObject = nullptr;
			}

			return *this;
		}

		/*
		* Specialized constructor for child classes to set the type of the object
		*/
		Action::Action(IdType type) : Attributed(type) {}

		/*
		* Append a Datum to this Scope with the given name, and return a reference to that Datum.
		* If a Datum with the given name already exists, return the existing Datum.
		* @param key - The name of the Datum to append
		* @return - A reference to the Datum appended
		*/
		Datum& Action::appendAuxiliaryAttribute(const std::string& key)
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
		bool Action::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		/*
		* Prescribed attributes for every Action object
		* @return - A vector of Signatures representing the prescribed attributes for Action
		*/
		std::vector<Signature> Action::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_name"s, Datum::DataType::STRING, 1, offsetof(Action, name) }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string Action::ToString() const
		{
			return "Action"s;
		}

		// ========================= Parent GameObject =========================

		glm::vec4 Action::getParentPosition()
		{
			return getParentGameObject()->transform.position;
		}

		glm::vec4 Action::getParentRotation()
		{
			return getParentGameObject()->transform.rotation;
		}

		glm::vec4 Action::getParentScale()
		{
			return getParentGameObject()->transform.scale;
		}

		GameObject* Action::getParentGameObject()
		{
			if (parentGameObject == nullptr)
			{
				parentGameObject = searchParentScope<GameObject>();
				return parentGameObject;
			}

			return parentGameObject;	
		}

		void Action::setParentGameObject(GameObject* parent)
		{
			parentGameObject = parent;
		}
	}
}