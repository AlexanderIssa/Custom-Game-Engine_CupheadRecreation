#include "pch.h"
#include "framework.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "GameClock.h"
#include  <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::chrono;

namespace Fiea::GameEngine::Test
{

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

	TEST_CLASS(BenchmarkTest)
	{
	public:

		TEST_CLASS_INITIALIZE(InitializeClass)
		{

		}

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
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

		TEST_METHOD(Benchmark)
		{
			constexpr int count = 500;

			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();
			GameClock clock([&now]() { return now; });
			GameTime time = clock.Current();
			now += std::chrono::milliseconds(ms);

			GameObject root{};

			// Gameobject creation
			{
				high_resolution_clock clock{};
				auto start = clock.now();
				for (size_t i = 0; i < count; ++i)
				{
					root.addChild(new GameObject(), "GameObject");
				}
				auto duration = clock.now() - start;

				std::stringstream msg;
				msg << "create objects: " << duration.count() / 1000000 << std::endl;
				Logger::WriteMessage(msg.str().c_str());
			}

			// Gameobject update
			{
				high_resolution_clock clock{};
				auto start = clock.now();
				for (size_t i = 0; i < count; ++i)
				{
					root.update(time);
				}
				auto duration = clock.now() - start;

				std::stringstream msg;
				msg << "update objects: " << duration.count() << std::endl;
				Logger::WriteMessage(msg.str().c_str());
			}

		}

	private:
		inline static _CrtMemState _startMemState;
	};
}