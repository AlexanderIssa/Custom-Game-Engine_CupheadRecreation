#include"pch.h"
#include "ActionIncrement.h"
#include "GameObject.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(ActionIncrement);

		/*
		* Default constructor for ActionIncrement
		*/
		ActionIncrement::ActionIncrement() : Action(ActionIncrement::TypeIdClass()) {}

		/*
		* Clone function to create a copy of the ActionIncrement object
		* @return - A pointer to the newly created ActionIncrement object
		*/
		ActionIncrement* ActionIncrement::clone() const
		{
			return new ActionIncrement(*this);
		}

		/*
		* OnEnable function to be called when the object is enabled
		*/
		void ActionIncrement::onEnable()
		{

		}

		/*
		* onDisable function to be called when the object is disabled
		*/
		void ActionIncrement::onDisable()
		{
			if (!getParentGameObject()->alive)
			{
				key = "";
				value = 0;
			}
		}

		void ActionIncrement::update(const GameTime&)
		{
			if (getParentGameObject() != nullptr)
			{
				Datum* target = search(key);
				//Datum* target = getParentGameObject()->find(key);
				if (target != nullptr)
				{
					if (target->getDataType() == Datum::DataType::INTEGER)
					{
						target->get<int>(0) += static_cast<int>(value);
					}
					else if (target->getDataType() == Datum::DataType::FLOAT)
					{
						target->get<float>(0) += value;
					}
				}
			}	
		}

		/*
		* Prescribed attributes for every ActionIncrement object
		* @return - A vector of Signatures representing the prescribed attributes for ActionIncrement
		*/
		std::vector<Signature> ActionIncrement::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_key"s, Datum::DataType::STRING, 1, offsetof(ActionIncrement, key) },
				{ "float_value"s, Datum::DataType::FLOAT, 1, offsetof(ActionIncrement, value) }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string ActionIncrement::ToString() const
		{
			return "ActionIncrement"s;
		}
	}
}