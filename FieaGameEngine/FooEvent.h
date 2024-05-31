#pragma once
#include "IEvent.h"

namespace Fiea
{
	namespace GameEngine
	{
		class FooEvent final : public IEvent
		{
			RTTI_DECLARATIONS(FooEvent, IEvent);

		public:
			FooEvent() = default;
			virtual ~FooEvent() = default;

			int integerValue{ 0 };
			float floatValue{ 0.0f };
		};
	}
}