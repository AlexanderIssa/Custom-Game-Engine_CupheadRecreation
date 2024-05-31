#pragma once
#include "IEvent.h"

namespace Fiea
{
	namespace GameEngine
	{
		class BarEvent final : public IEvent
		{
			RTTI_DECLARATIONS(BarEvent, IEvent);

		public:
			BarEvent() = default;
			virtual ~BarEvent() = default;

			std::string stringValue{ "" };
		};
	}
}