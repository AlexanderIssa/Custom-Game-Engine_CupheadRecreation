#include "pch.h"
#include "AttributedBar.h"
#include <cstddef>

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(AttributedBar);

		/*
		* Default constructor for AttributedBar
		*/
		AttributedBar::AttributedBar() : AttributedFoo(AttributedBar::TypeIdClass()){}

		/*
		* Clone method for AttributedBar
		* @return - A pointer to the cloned AttributedBar object
		*/
		AttributedBar* AttributedBar::clone() const
		{
			return new AttributedBar(*this);
		}

		/*
		* Prescribed attributes for every AttributedBar object
		* @return - A vector of Signatures representing the prescribed attributes for AttributedBar
		*/
		std::vector<Signature> AttributedBar::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "externalStringChild"s, Datum::DataType::STRING, 1, offsetof(AttributedBar, externalStringChild) }
			};
		}

		// ========================= RTTI Overrides =========================

		std::string AttributedBar::ToString() const
		{
			return "AttributedBar"s;
		}
	}
}