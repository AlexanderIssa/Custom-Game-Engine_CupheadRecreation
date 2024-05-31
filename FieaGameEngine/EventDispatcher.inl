#pragma once
#include "EventDispatcher.h"
#include  <stdexcept>

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* A static method to add a subscriber to the event dispatcher.
		* Note: You can subscribe to a event before it is initialized.
		* @parm eventID: The event to subscribe to.
		* @param callback: The callback to add to the event.
		*/
		template<typename T>
		void EventDispatcher::addListener(EventCallback* callback)
		{
			// Essentially C# where T : IEvent
			if constexpr (!std::is_base_of<IEvent, T>::value)
			{
				throw std::invalid_argument("T must be a subclass of IEvent.");
			}

			if (callback == nullptr)
			{
				throw std::invalid_argument("Callback cannot be null.");
			}

			// Lazy initialization of the subscribers map
			if (subscribers == nullptr)
			{
				subscribers = new std::unordered_map<RTTI::IdType, std::vector<EventCallback*>>();
			}

			RTTI::IdType eventID = T::TypeIdClass();

			(*subscribers)[eventID].push_back(callback);
		}

		// RICHI-NYA!

		/*
		* A static method to remove a subscriber from the event dispatcher.
		* @param eventID: The event to unsubscribe from.
		* @param callback: The callback to remove from the event.
		*/
		template<typename T>
		void EventDispatcher::removeListener(EventCallback* callback)
		{
			// Essentially C# where T : IEvent
			if constexpr (!std::is_base_of<IEvent, T>::value)
			{
				throw std::invalid_argument("T must be a subclass of IEvent.");
			}

			if (callback == nullptr)
			{
				throw std::invalid_argument("Callback cannot be null.");
			}

			if (subscribers == nullptr || subscribers->size() <= 0)
			{
				throw std::runtime_error("No active event found in the system.");
			}

			RTTI::IdType eventID = T::TypeIdClass();

			if ((*subscribers).find(eventID) != subscribers->end()
				&& (*subscribers)[eventID].size() > 0)
			{
				// Remove any matching callbacks from the event
				// std::remove shifts all the elements to be removed to the end of the vector
				// std::remove returns an iterator to the first element to be removed
				// so vector.erase() removes all the elements from that iterator to the end of the vector
				(*subscribers)[eventID].erase(
					std::remove(
						(*subscribers)[eventID].begin(),
						(*subscribers)[eventID].end(),
						callback
					),
					(*subscribers)[eventID].end());

				// If there are no more subscribers for this event, remove the event from the map
				if (subscribers->size() == 0)
				{
					subscribers->erase(eventID);
				}
			}
			else
			{
				throw std::runtime_error("No subscribers found for this event.");
			}			
		}
		/*
		* Enqueues an event to be dispatched at a later time.
		* @param event: The event to enqueue.
		* @param currentTime: The current time of the game.
		* @param delay: The delay in second before the event is fired.
		*/
		template<typename T>
		void EventDispatcher::enqueue(T* event, GameTime currentTime, float delay)
		{
			// Essentially C# where T : IEvent
			if constexpr (!std::is_base_of<IEvent, T>::value)
			{
				throw std::invalid_argument("T must be a subclass of IEvent.");
			}

			if (event == nullptr)
			{
				throw std::invalid_argument("Event cannot be null.");
			}

			// Lazy initialization of the event queue
			if (eventQueue == nullptr)
			{
				eventQueue = new std::vector<QueueEntry>();
			}

			eventQueue->push_back({ T::TypeIdClass(), event, currentTime, delay });
		}

		/*
		* A static method to dispatch an event to all subscribers.
		* @param event: The event to dispatch.
		*/
		template<typename T>
		void EventDispatcher::dispatchEvent(T* event)
		{
			// Essentially C# where T : IEvent
			if constexpr (!std::is_base_of<IEvent, T>::value)
			{
				throw std::invalid_argument("T must be a subclass of IEvent.");
			}

			if (event == nullptr)
			{
				throw std::invalid_argument("Event cannot be null.");
			}

			if (subscribers == nullptr || subscribers->size() <= 0)
			{
				throw std::runtime_error("No active event found in the system.");
			}

   			RTTI::IdType eventID = event->TypeIdClass();

			// If this event has subscribers, invoke all the callbacks
			if ((*subscribers).find(eventID) != subscribers->end()
				&& (*subscribers)[eventID].size() > 0)
			{
				for (auto& callback : (*subscribers)[event->TypeIdClass()])
				{
					(*callback)(event);
				}
			}
			else
			{
				throw std::runtime_error("No subscribers found for this event.");
			}
		}

	}
}