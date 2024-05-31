#pragma once
#include "IEvent.h"
#include <vector>
#include <unordered_map>

namespace Fiea
{
	namespace GameEngine
	{
		struct GameTime;

		class EventQueue
		{
			// Enqueue an event with a optional delay
			void Enqueue(IEvent* event, GameTime currentTime, float delay = 0.f);

			// Fire any expired event
			void Update(GameTime tick);

			// Clear all events
			void Clear();

			// Check if the event queue is empty
			bool IsEmpty() const;

			// Get the number of events in the queue
			size_t getSize() const;

		private:
			std::unordered_map<IEvent*, float> eventList;
			size_t mSize { 0 };
		};
	}
}