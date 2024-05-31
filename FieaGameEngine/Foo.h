#pragma once

#include <cstdint>
#include "RTTI.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Foo : public RTTI
		{
			RTTI_DECLARATIONS(Foo, RTTI);
		public:
			explicit Foo(std::int32_t data = 0);
			Foo(const Foo& rhs);
			Foo(Foo&& rhs) noexcept;
			Foo& operator=(const Foo& rhs);
			Foo& operator=(Foo&& rhs) noexcept;
			virtual ~Foo() override;

			bool operator==(const Foo& rhs) const;
			bool operator!=(const Foo& rhs) const;

			std::int32_t Data() const;
			void SetData(std::int32_t data);

			// RTTI Overrides
			std::string ToString() const override;
			bool Equals(const RTTI* rhs) const override;
		private:
			std::int32_t* _data;
		};
	}
}