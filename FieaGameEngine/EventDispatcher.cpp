#include "pch.h"
#include "EventDispatcher.h"
#include "GameClock.h"
#include  <stdexcept>
#include <iostream>

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
		void EventDispatcher::addListener(RTTI::IdType eventID, EventCallback * callback)
		{
			if (callback == nullptr)
			{
				throw std::invalid_argument("Callback cannot be null.");
			}
			// Lazy initialization of the subscribers map
			if (subscribers == nullptr)
			{
				subscribers = new std::unordered_map<RTTI::IdType, std::vector<EventCallback*>>();
			}
			(*subscribers)[eventID].push_back(callback);
		}

		/*
		* A static method to remove a subscriber from the event dispatcher.
		* @param eventID: The event to unsubscribe from.
		* @param callback: The callback to remove from the event.
		*/
		void EventDispatcher::removeListener(RTTI::IdType eventID, EventCallback* callback)
		{
			if (callback == nullptr)
			{
				throw std::invalid_argument("Callback cannot be null.");
			}

			if (subscribers == nullptr || subscribers->size() <= 0)
			{
				throw std::runtime_error("No active event found in the system.");
			}

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
		* Dispatch an event to all subscribers.
		* @param eventID: The id of the event to dispatch.
		*/
		void EventDispatcher::dispatchEvent(RTTI::IdType eventID, IEvent* event)
		{
			if (subscribers == nullptr || subscribers->size() <= 0)
			{
				throw std::runtime_error("No active event found in the system.");
			}

			// If this event has subscribers, invoke all the callbacks
			if ((*subscribers).find(eventID) != subscribers->end()
				&& (*subscribers)[eventID].size() > 0)
			{
				for (auto& callback : (*subscribers)[eventID])
				{
					(*callback)(event);
				}
			}
			else
			{
				//throw std::runtime_error("No subscribers found for this event.");
				std::cout << "[DispatchEvent] No subscribers found for this event." << std::endl;
			}
		}

		/*
		* Dispatch all events in the queue.
		*/
		void EventDispatcher::dispatchAllQueuedEvents()
		{
			if (eventQueue == nullptr || eventQueue->size() <= 0)
			{
				throw std::runtime_error("Event queue is empty.");
			}

			for (auto& eventData : *eventQueue)
			{
				EventDispatcher::dispatchEvent(eventData.eventID, eventData.event);
				eventData.dispatched = true;
			}

			eventQueue->clear();
		}

		/*
		* Enqueues an event to be dispatched at a later time.
		* @param eventID: the IdType of the event
		* @param event: The event to enqueue.
		* @param currentTime: The current time of the game.
		* @param delay: The delay in second before the event is fired.
		*/
		void EventDispatcher::enqueue(RTTI::IdType eventID, IEvent* event, GameTime currentTime, float delay)
		{
			if (event == nullptr)
			{
				throw std::invalid_argument("Event cannot be null.");
			}

			// Lazy initialization of the event queue
			if (eventQueue == nullptr)
			{
				eventQueue = new std::vector<QueueEntry>();
			}

			eventQueue->push_back({ eventID, event, currentTime, delay });
		}

		void EventDispatcher::update(GameTime time)
		{
			if (eventQueue == nullptr || eventQueue->size() <= 0)
				return;

			// Check if the delay for any event has expired
			for (auto& eventData : *eventQueue)
			{
				float timePassed = static_cast<float>(time.Game() - eventData.startTime.Game());
				float delayMilliseconds = eventData.delay * 1000.f;
				if (timePassed >= delayMilliseconds)
				{
					EventDispatcher::dispatchEvent(eventData.eventID, eventData.event);
					eventData.dispatched = true;
				}
			}

			// Remove all dispatched events from the queue
			eventQueue->erase(
				std::remove_if(
					eventQueue->begin(),
					eventQueue->end(),
					[](const QueueEntry& eventData) { return eventData.dispatched; }),
				eventQueue->end()
			);
		}

		/*
		* Clear all subscribers
		*/
		void EventDispatcher::clearSubscribers()
		{
			if (subscribers != nullptr)
			{
				subscribers->clear();
			}
		}

		/*
		* Clear all events in the queue.
		*/
		void EventDispatcher::clearQueue()
		{
			if (eventQueue != nullptr)
			{
				eventQueue->clear();
			}
		}

		/*
		* A static method to clear the class
		*/
		void EventDispatcher::clear()
		{
			clearQueue();
			clearSubscribers();

			if (eventQueue != nullptr)
			{
				delete eventQueue;
				eventQueue = nullptr;
			}

			if (subscribers != nullptr)
			{
				delete subscribers;
				subscribers = nullptr;
			}
		}

		/*
		* Get the number of events in the queue.
		* @return is the queue empty?
		*/
		bool EventDispatcher::isQueueEmpty()
		{
			return eventQueue->empty();
		}

		/*
		* Get the number of events in the queue.
		* @return The number of events in the queue.
		*/
		size_t EventDispatcher::getQueueSize()
		{
			return eventQueue->size();
		}
	}
}