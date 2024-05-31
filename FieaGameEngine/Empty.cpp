#include "pch.h"
#include "Empty.h"

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(Empty);

		bool Empty::operator==(const Empty& rhs) const
		{
			return this == &rhs;
		}

		std::string Empty::ToString() const
		{
			return "Empty";
		}

		bool Empty::Equals(const RTTI* other) const
		{
			if (other == nullptr)
			{
				return false;
			}

			const Empty* empty = other->As<Empty>();
			return empty != nullptr ? *this == *empty : false;
		}
	}
}