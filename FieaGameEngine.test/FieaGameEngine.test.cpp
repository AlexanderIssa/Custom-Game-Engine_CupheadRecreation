#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Empty.h"
#include "Foo.h"
#include "FooFoo.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Datum::DataType DataType;
typedef Fiea::GameEngine::Empty Empty;
typedef Fiea::GameEngine::Foo Foo;
typedef Fiea::GameEngine::FooFoo FooFoo;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template<>
	inline std::wstring ToString<DataType>(const DataType& t)
	{
		switch (t)
		{
			case DataType::UNKNOWN:
				return L"UNKNOWN";
			case DataType::INTEGER:
				return L"INTEGER";
			case DataType::FLOAT:
				return L"FLOAT";
			case DataType::STRING:
				return L"STRING";
			case DataType::VEC4:
				return L"VEC4";
			case DataType::MAT4:
				return L"MAT4";
			default:
				return L"Undefined";
		}
	}
}

namespace FieaGameEnginetest
{
	TEST_CLASS(FieaGameEnginetest)
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

		
		TEST_METHOD(TestTemp)
		{
			Fiea::GameEngine::Temp temp;
			Assert::AreEqual(temp.TempFunction(), true);
		}
		TEST_METHOD(TestIsRTTI)
		{
			Foo f;
			Assert::IsTrue(f.Is(Foo::TypeIdClass()));

			Empty e;
			Assert::IsTrue(e.Is(Empty::TypeIdClass()));
			Assert::IsFalse(e.Is(Foo::TypeIdClass()));
			Assert::IsFalse(f.Is(Empty::TypeIdClass()));

			FooFoo ff1(1), ff2(2);
			Assert::IsTrue(ff1.Is(Foo::TypeIdClass()));
			Assert::IsTrue(ff1.Is(FooFoo::TypeIdClass()));
			Assert::IsTrue(ff2.Is(Foo::TypeIdClass()));
			Assert::IsTrue(ff2.Is(FooFoo::TypeIdClass()));
			Assert::IsFalse(ff1.Is(Empty::TypeIdClass()));
			Assert::IsFalse(ff2.Is(Empty::TypeIdClass()));
			Assert::IsFalse(f.Is(FooFoo::TypeIdClass()));
			Assert::IsFalse(e.Is(FooFoo::TypeIdClass()));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
