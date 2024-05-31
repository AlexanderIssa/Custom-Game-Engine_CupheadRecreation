#pragma once
#include "IEvent.h"
#include "GameClock.h"
#include <vector>
#include <functional>
#include <unordered_map>

namespace Fiea
{
	namespace GameEngine
	{
		// Imitation of the Delegate class from the C# language
		template<typename ReturnType, typename... Args>
		using Delegate = std::function<ReturnType(Args...)>;

		// Delegate for event callbacks
		using EventCallback = Delegate<void, IEvent*>;

		class EventDispatcher
		{
		private:
			struct QueueEntry
			{
				RTTI::IdType eventID;
				IEvent* event;
				GameTime startTime;
				float delay;
				bool dispatched{ false };
			};

		public:
			// Default constructor
			EventDispatcher() = default;

			// Copy constructor deleted
			EventDispatcher(EventDispatcher& other) = delete;

			// Move constructor deleted
			EventDispatcher(EventDispatcher&& rhs) = delete;

			// Destructor
			~EventDispatcher () = default;

			// Add a listener to the event dispatcher, id is the event's IdType
			static void addListener(RTTI::IdType eventID, EventCallback* callback);

			// Remove a listened from the event dispatcher, id is the event's IdType
			static void removeListener(RTTI::IdType eventID, EventCallback* callback);

			// Add a listener to the event dispatcher, T is the event type
			template<typename T>
			static void addListener(EventCallback* callback);

			// Remove a listener from the event dispatcher, T is the event type
			template<typename T>
			static void removeListener(EventCallback* callback);

			// Dispatch an event to all subscribers
			template<typename T>
			static void dispatchEvent(T* event);

			// Dispatch an event by ID
			static void dispatchEvent(RTTI::IdType eventID, IEvent* event);

			// Dispatch all events in the queue at once
			static void dispatchAllQueuedEvents();

			// Enqueue an event with a optional delay
			template<typename T>
			static void enqueue(T* event, GameTime currentTime, float delay = 0.f);

			// enqueue an event by ID
			static void enqueue(RTTI::IdType eventID, IEvent* event, GameTime currentTime, float delay = 0.f);

			// Fire any expired event in the queue
			static void update(GameTime time);

			// Clear all subscribers
			static void clearSubscribers();

			// Clear all events in the queue
			static void clearQueue();

			// Clear all contents
			static void clear();

			// Check if the event queue is empty
			static bool isQueueEmpty();

			// Get the number of events in the queue
			static size_t getQueueSize();

		private:
			inline static std::unordered_map<RTTI::IdType, std::vector<EventCallback*>>* subscribers;
			inline static std::vector<QueueEntry>* eventQueue;

		};
	}
}
#include "EventDispatcher.inl"

#define BIND_FUNC(Sender, Function) \
	std::bind(&Function, Sender, std::placeholders::_1);