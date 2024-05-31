#include"pch.h"
#include "Monster.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(Monster);

		/*
		* Default constructor for Monster
		*/
		Monster::Monster() : GameObject(Monster::TypeIdClass()) {}

		/*
		* Prescribed attributes for every Monster object
		* @return - A vector of Signatures representing the prescribed attributes for Monster
		*/
		std::vector<Signature> Monster::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_tag"s, Datum::DataType::STRING, 1, offsetof(Monster, tag) }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string Monster::ToString() const
		{
			return "Monster"s;
		}
	}
}