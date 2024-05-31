#pragma once
#include "IEvent.h"

namespace Fiea
{
	namespace GameEngine
	{
		class ThrowBoneEvent final : public IEvent
		{
			RTTI_DECLARATIONS(BarEvent, IEvent);

		public:
			ThrowBoneEvent() = default;
			virtual ~ThrowBoneEvent() = default;

			std::string color = "DefaultWhite";
			float mass = 10;
		};
	}
}