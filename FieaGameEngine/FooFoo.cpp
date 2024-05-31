#include "pch.h"
#include "FooFoo.h"

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(FooFoo);

		bool FooFoo::Equals(const RTTI* other) const
		{
			if (other == nullptr)
			{
				return false;
			}

			const FooFoo* foo = other->As<FooFoo>();
			return foo != nullptr ? *this == *foo : false;
		}
	}
}