#pragma once

namespace Fiea
{
	namespace GameEngine
	{
		class TestSubscriber
		{
		public:
			TestSubscriber();
			virtual ~TestSubscriber();
			
			// For testing purposes these variables are public
			int mInteger{ 0 };
			float mFloat{ 0.0f };
			std::string mString { "" };

		private:
			EventCallback* myFooCallback;
			EventCallback* myBarCallback;

			void OnFooEvent(IEvent* eventData);
			void OnBarEvent(IEvent* eventData);
		};
	}
}
