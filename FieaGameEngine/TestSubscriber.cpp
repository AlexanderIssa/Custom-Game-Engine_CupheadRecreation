#include "pch.h"
#include "EventDispatcher.h"
#include "TestSubscriber.h"
#include "FooEvent.h"
#include "BarEvent.h"
#include <stdexcept>

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* Constructor for the test subscriber.
		*/
		TestSubscriber::TestSubscriber()
		{
			auto bindedFooFunction = BIND_FUNC(this, TestSubscriber::OnFooEvent);
			myFooCallback = new EventCallback(bindedFooFunction);
			EventDispatcher::addListener<FooEvent>(myFooCallback);

			auto bindedBarFunction = BIND_FUNC(this, TestSubscriber::OnBarEvent);
			myBarCallback = new EventCallback(bindedBarFunction);
			EventDispatcher::addListener<BarEvent>(myBarCallback);
		}

		/*
		* Destructor for the test subscriber.
		*/
		TestSubscriber::~TestSubscriber()
		{
			EventDispatcher::removeListener<FooEvent>(myFooCallback);
			EventDispatcher::removeListener<BarEvent>(myBarCallback);
			delete myFooCallback;
			delete myBarCallback;
		}

		/*
		* The callbalck function that reacts to the Foo event.
		* @param eventData: The event data to handle.
		*/
		void TestSubscriber::OnFooEvent(IEvent* eventData)
		{
			if (eventData == nullptr)
			{
				throw std::runtime_error("eventData cannot be null.");
			}

			FooEvent* data = eventData->As<FooEvent>();

			// Check if the data is of the correct type, this should always be true though
			if (data == nullptr)
			{
				throw std::runtime_error("Invalid event data type.");
			}

			mInteger += data->integerValue;
			mFloat += data->floatValue;
		}

		/*
		* The callbalck function that reacts to the test event.
		* @param eventData: The event data to handle.
		*/
		void TestSubscriber::OnBarEvent(IEvent* eventData)
		{
			if (eventData == nullptr)
			{
				throw std::runtime_error("eventData cannot be null.");
			}

			BarEvent* data = eventData->As<BarEvent>();

			// Check if the data is of the correct type, this should always be true though
			if (data == nullptr)
			{
				throw std::runtime_error("Invalid event data type.");
			}

			mString = data->stringValue;
		}
	}
}