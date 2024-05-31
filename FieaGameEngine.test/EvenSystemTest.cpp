#include "pch.h"
#include "CppUnitTest.h"
#include "GameClock.h"

#include "EventDispatcher.h"
#include "FooEvent.h"
#include "BarEvent.h"
#include "TestSubscriber.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef Fiea::GameEngine::EventCallback EventCallback;
typedef Fiea::GameEngine::GameClock GameClock;
typedef Fiea::GameEngine::GameTime GameTime;
typedef Fiea::GameEngine::EventDispatcher EventDispatcher;
typedef Fiea::GameEngine::IEvent IEvent;
typedef Fiea::GameEngine::FooEvent FooEvent;
typedef Fiea::GameEngine::BarEvent BarEvent;
typedef Fiea::GameEngine::TestSubscriber TestSubscriber;

namespace FieaGameEnginetest
{
	TEST_CLASS(FieaGameEngineEventTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			EventDispatcher::clear();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(EventSubscribeTest)
		{
			// Create a foo event and modify its metadata
			FooEvent* testEvent = new FooEvent();
			testEvent->integerValue = 10;
			testEvent->floatValue = 6.9f;

			// TestSubscriber subscribes to FooEvent on construction
			// and unsubscribes to it on destruction
			TestSubscriber* testSubscriber = new TestSubscriber();
			Assert::AreEqual(0, testSubscriber->mInteger);
			Assert::AreEqual(0.0f, testSubscriber->mFloat);

			EventDispatcher::dispatchEvent(testEvent);
			Assert::AreEqual(10, testSubscriber->mInteger);
			Assert::AreEqual(6.9f, testSubscriber->mFloat);

			EventDispatcher::dispatchEvent(testEvent);
			Assert::AreEqual(20, testSubscriber->mInteger);
			Assert::AreEqual(13.8f, testSubscriber->mFloat);

			// Test exception
			EventCallback* emptyCallback = new EventCallback();

			// Destructor should remove the listener
			delete testSubscriber;
			Assert::ExpectException<std::invalid_argument>([&] { EventDispatcher::addListener<FooEvent>(nullptr); });

			Assert::ExpectException<std::invalid_argument>([&] { EventDispatcher::removeListener<FooEvent>(nullptr); });
			Assert::ExpectException<std::runtime_error>([&] { EventDispatcher::removeListener<FooEvent>(emptyCallback); });

			Assert::ExpectException<std::invalid_argument>([&] { EventDispatcher::dispatchEvent<FooEvent>(nullptr); });
			Assert::ExpectException<std::runtime_error>([&] { EventDispatcher::dispatchEvent<FooEvent>(testEvent); });

			delete testEvent;
			delete emptyCallback;
		}

		TEST_METHOD(EventQueueTest)
		{          	
			// Create a foo event and modify its metadata
			FooEvent* fooEvent = new FooEvent();
			fooEvent->integerValue = 10;
			fooEvent->floatValue = 1.0f;

			// Create a bar event and modify its metadata
			BarEvent* barEvent = new BarEvent();
			barEvent->stringValue = "Hello";

			// TestSubscriber subscribes to FooEvent and BarEvent on construction
			// and unsubscribes to them on destruction
			TestSubscriber testSubscriber;
			Assert::AreEqual(0, testSubscriber.mInteger);
			Assert::AreEqual(0.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string(""), testSubscriber.mString);

			EventDispatcher::dispatchEvent(fooEvent);
			EventDispatcher::dispatchEvent(barEvent);
			Assert::AreEqual(10, testSubscriber.mInteger);
			Assert::AreEqual(1.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string("Hello"), testSubscriber.mString);

			// Test event queue
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();
			GameClock clock([&now]() { return now; });
			GameTime time = clock.Current();

			// Enqueue foo event to be dispatched in 1 second
			EventDispatcher::enqueue(fooEvent, time, 1);

			// Fast forward time by 100ms, nothing happens yet
			now += std::chrono::milliseconds(100);
			clock.Update(time);
			time = clock.Current();
			EventDispatcher::update(time);
			Assert::AreEqual(10, testSubscriber.mInteger);
			Assert::AreEqual(1.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string("Hello"), testSubscriber.mString);

			// Fast forward time by 900ms, the event is dispatched
			now += std::chrono::milliseconds(900);
			clock.Update(time);
			time = clock.Current();
			EventDispatcher::update(time);
			Assert::AreEqual(20, testSubscriber.mInteger);
			Assert::AreEqual(2.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string("Hello"), testSubscriber.mString);

			// Enqueue bar event and foo event to be dispatched in 1.69 second
			barEvent->stringValue = "Hola";
			EventDispatcher::enqueue(fooEvent, time, 1.69f);
			EventDispatcher::enqueue(barEvent, time, 1.69f);

			// Fast forward time by 100ms, nothing happens yet
			now += std::chrono::milliseconds(100);
			clock.Update(time);
			time = clock.Current();
			EventDispatcher::update(time);
			Assert::AreEqual(20, testSubscriber.mInteger);
			Assert::AreEqual(2.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string("Hello"), testSubscriber.mString);

			// Fast forward time by 1590ms, the event is dispatched
			now += std::chrono::milliseconds(1590);
			clock.Update(time);
			time = clock.Current();
			EventDispatcher::update(time);
			Assert::AreEqual(30, testSubscriber.mInteger);
			Assert::AreEqual(3.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string("Hola"), testSubscriber.mString);

			// Check if events are not repeated
			for (int i = 0; i < 10; i++)
			{
				now += std::chrono::milliseconds(10);
				clock.Update(time);
				time = clock.Current();
				Assert::AreEqual(30, testSubscriber.mInteger);
				Assert::AreEqual(3.0f, testSubscriber.mFloat);
				Assert::AreEqual(std::string("Hola"), testSubscriber.mString);
			}

			// Test dispatch all queued events
			// Also try to call from other method
			barEvent->stringValue = "Halo";
			auto EnqueueAll = [&fooEvent ,&barEvent , &time]() {
				EventDispatcher::enqueue(fooEvent, time, 3.10f);
				EventDispatcher::enqueue(fooEvent, time, 5.78f);
				EventDispatcher::enqueue(fooEvent, time, 7.69f);
				EventDispatcher::enqueue(barEvent, time, 5.43f);
			};
			EnqueueAll();

			// Also create second listener
			TestSubscriber testSubscriber2;
			Assert::AreEqual(0, testSubscriber2.mInteger);
			Assert::AreEqual(0.0f, testSubscriber2.mFloat);
			Assert::AreEqual(std::string(""), testSubscriber2.mString);
			
			EventDispatcher::dispatchAllQueuedEvents();
			Assert::AreEqual(60, testSubscriber.mInteger);
			Assert::AreEqual(6.0f, testSubscriber.mFloat);
			Assert::AreEqual(std::string("Halo"), testSubscriber.mString);

			Assert::AreEqual(30, testSubscriber2.mInteger);
			Assert::AreEqual(3.0f, testSubscriber2.mFloat);
			Assert::AreEqual(std::string("Halo"), testSubscriber2.mString);

			Assert::IsTrue(EventDispatcher::isQueueEmpty());
			Assert::AreEqual((size_t)0, EventDispatcher::getQueueSize());

			// Test exception
			Assert::ExpectException<std::invalid_argument>([&] { EventDispatcher::enqueue<FooEvent>(nullptr, time, 0); });
			Assert::ExpectException<std::runtime_error>([&] { EventDispatcher::dispatchAllQueuedEvents(); });
			Assert::ExpectException<std::runtime_error>([&] { EventDispatcher::dispatchEvent(fooEvent->As<IEvent>()); });

			delete fooEvent;
			delete barEvent;
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}