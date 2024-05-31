#include"pch.h"
#include "ActionListWhile.h"
#include "GameObject.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(ActionListWhile);

		/*
		* Default constructor for ActionListWhile
		*/
		ActionListWhile::ActionListWhile() : ActionList(ActionListWhile::TypeIdClass()) {}

		/*
		* Clone function to create a copy of the current ActionListWhile
		* @return - A pointer to the cloned ActionListWhile
		*/
		ActionListWhile* ActionListWhile::clone() const
		{
			return new ActionListWhile(*this);
		}
		
		/*
		* Specialized constructor for ActionIncrement
		*/
		void ActionListWhile::onEnable()
		{

		}

		/*
		* onDisable function to be called when the object is disabled
		*/
		void ActionListWhile::onDisable()
		{
			if (!getParentGameObject()->alive)
			{
				ActionList::onDisable();
				int condition = 0;
				preamble = nullptr;
				increment = nullptr;
				isConditionSet = false;
			}
		}

		// 1. Call preamble before the first iteration
		// 2. Loop unitl condition is 0
		// 3. Call increment after each iteration
		void ActionListWhile::update(const GameTime& tick)
		{
			if (!isConditionSet)
				return;

			if (preamble != nullptr)
				preamble->update(tick);

			while (condition > 0)
			{
				for (size_t i = 0; i < actions->getSize(); i++)
				{
					(*actions)[i].get<Scope*>(0)->As<Action>()->update(tick);
				}

				if (increment != nullptr)
					increment->update(tick);

				condition--;
			}
		}

		/*
		* Set the condition of the while loop
		* @param num - The condition to set
		*/
		void ActionListWhile::SetCondition(int num)
		{
			condition = num;
			isConditionSet = true;
		}

		/*
		* Set the preamble of the while loop
		* @param action - The preamble action to set
		*/
		void ActionListWhile::SetPreamble(Action* action)
		{
			if (action == nullptr)
				throw std::invalid_argument("Cannot set a nullptr as the preamble action");

			preamble = action;
			adopt(std::move(preamble), "obj_preamble"s);
		}

		/*
		* Set the increment of the while loop
		* @param action - The increment action to set
		*/
		void ActionListWhile::SetIncrement(Action* action)
		{
			if(action == nullptr)
				throw std::invalid_argument("Cannot set a nullptr as the increment action");

			increment = action;
			adopt(std::move(increment), "obj_increment"s);
		}

		/*
		* Prescribed attributes for every ActionListWhile object
		* @return - A vector of Signatures representing the prescribed attributes for ActionListWhile
		*/
		std::vector<Signature> ActionListWhile::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "int_condition"s, Datum::DataType::INTEGER, 1, offsetof(ActionListWhile, condition)},
				{ "obj_preamble"s, Datum::DataType::TABLE, 0, 0 },
				{ "obj_increment"s, Datum::DataType::TABLE, 0, 0 }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string ActionListWhile::ToString() const
		{
			return "ActionListWhile"s;
		}
	}
}