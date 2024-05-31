#pragma once
#include "RTTI.h"

namespace Fiea
{
	namespace GameEngine
	{
		// IEvent class is more like a interface for event "message/payload"
		// that will be received by the subscribers of the EventDispatcher
		// the subscribers(callbacks) will read the data from the event and act accordingly
		class IEvent : public RTTI
		{
			RTTI_DECLARATIONS(IEvent, RTTI);

		public:
			IEvent() = default;
			virtual ~IEvent() = default;	
		};
	}
}