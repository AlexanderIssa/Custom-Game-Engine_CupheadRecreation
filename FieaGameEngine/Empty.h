#pragma once
#include"RTTI.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Empty : public RTTI
		{
			RTTI_DECLARATIONS(Empty, RTTI);
		public:
			virtual ~Empty() = default;

			// Equality operator for testing purposes
			bool operator==(const Empty& rhs) const;

			// RTTI Overrides
			std::string ToString() const override;
			bool Equals(const RTTI* rhs) const override;
		};
	}
}