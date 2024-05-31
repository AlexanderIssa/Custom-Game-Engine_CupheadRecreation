#pragma once
#include "Foo.h"
#include "RTTI.h"

namespace Fiea 
{
	namespace GameEngine 
	{
		class FooFoo : public Foo 
		{
			RTTI_DECLARATIONS(FooFoo, Foo);
		public:
			explicit FooFoo(std::int32_t data = 0): Foo(data){}
			FooFoo(const FooFoo& rhs) : Foo(rhs) {};
			FooFoo(FooFoo&& rhs) noexcept : Foo(rhs) {};

			// override the virtual functions
			bool Equals(const RTTI* rhs) const override;
		};
	}
}