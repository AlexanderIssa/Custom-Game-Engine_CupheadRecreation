#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"

#include "Empty.h"
#include "Foo.h"
#include "FooFoo.h"
#include "Scope.h"

#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Datum::DataType DataType;
typedef Fiea::GameEngine::Empty Empty;
typedef Fiea::GameEngine::Foo Foo;
typedef Fiea::GameEngine::FooFoo FooFoo;
typedef Fiea::GameEngine::Scope Scope;

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
	inline std::wstring ToString<Empty>(Empty* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Foo>(Foo* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<FooFoo>(FooFoo* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Scope>(const Scope* t)
	{
		RETURN_WIDE_STRING(L"Scope ptr");
	}

	template<>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(L"Scope ptr");
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
		case DataType::POINTER:
			return L"POINTER";
		case DataType::TABLE:
			return L"TABLE";
		default:
			return L"Undefined";
		}
	}
}

namespace FieaGameEnginetest
{
	TEST_CLASS(FieaGameEngineDatumTest)
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

		TEST_METHOD(TestDatumConstruct)
		{
			Datum unknownDatum;

			Datum intDatum;
			intDatum.push_back(10);

			Datum floatDatum;
			floatDatum.push_back(0.69f);

			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);

			Datum vet4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vet4Datum.push_back(v);

			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);

			Datum pointerDatum;
			Empty* e = new Empty();
			Foo* f = new Foo(10);
			FooFoo* ff = new FooFoo(20);
			pointerDatum.push_back(e);
			pointerDatum.push_back(f);
			pointerDatum.push_back(ff);

			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			Scope* scope2 = (Scope*)0x2;
			scopeDatum.push_back(scope1);
			scopeDatum.push_back(scope2);

			// Test default constructor
			Datum datum;
			Assert::AreEqual(datum.getSize(), (size_t)0);

			// Test copy constructor
			Datum unknownDatumCopy(unknownDatum);
			Assert::AreEqual(unknownDatumCopy.getSize(), (size_t)0);

			Datum intDatumCopy(intDatum);
			Assert::AreEqual(intDatumCopy.getSize(), (size_t)1);
			Assert::AreEqual(intDatumCopy.get<int>(0), 10);

			Datum floatDatumCopy(floatDatum);
			Assert::AreEqual(floatDatumCopy.getSize(), (size_t)1);
			Assert::AreEqual(floatDatumCopy.get<float>(0), 0.69f);

			Datum stringDatumCopy(stringDatum);
			Assert::AreEqual(stringDatumCopy.getSize(), (size_t)1);
			Assert::AreEqual(stringDatumCopy.get<std::string>(0), s);

			Datum vet4DatumCopy(vet4Datum);
			Assert::AreEqual(vet4DatumCopy.getSize(), (size_t)1);
			Assert::AreEqual(vet4DatumCopy.get<glm::vec4>(0), v);

			Datum mat4DatumCopy(mat4Datum);
			Assert::AreEqual(mat4DatumCopy.getSize(), (size_t)1);
			Assert::AreEqual(mat4DatumCopy.get<glm::mat4>(0), m);

			Datum pointerDatumCopy(pointerDatum);
			Assert::AreEqual(pointerDatumCopy.getSize(), (size_t)3);
			Assert::IsTrue(pointerDatumCopy.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatumCopy.get<Foo*>(1)->Equals(f));
			Assert::IsTrue(pointerDatumCopy.get<FooFoo*>(2)->Equals(ff));

			Datum scopeDatumCopy(scopeDatum);
			Assert::AreEqual(scopeDatumCopy.getSize(), (size_t)2);
			Assert::AreEqual(scopeDatumCopy.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeDatumCopy.get<Scope*>(1), scope2);

			// Test move constructor
			Datum unknownDatumMove(std::move(unknownDatumCopy));
			Assert::AreEqual(unknownDatumMove.getSize(), (size_t)0);

			Datum intDatumMove(std::move(intDatumCopy));
			Assert::AreEqual(intDatumMove.getSize(), (size_t)1);
			Assert::AreEqual(intDatumMove.get<int>(0), 10);

			Datum floatDatumMove(std::move(floatDatumCopy));
			Assert::AreEqual(floatDatumMove.getSize(), (size_t)1);
			Assert::AreEqual(floatDatumMove.get<float>(0), 0.69f);

			Datum stringDatumMove(std::move(stringDatumCopy));
			Assert::AreEqual(stringDatumMove.getSize(), (size_t)1);
			Assert::AreEqual(stringDatumMove.get<std::string>(0), s);

			Datum vet4DatumMove(std::move(vet4DatumCopy));
			Assert::AreEqual(vet4DatumMove.getSize(), (size_t)1);
			Assert::AreEqual(vet4DatumMove.get<glm::vec4>(0), v);

			Datum mat4DatumMove(std::move(mat4DatumCopy));
			Assert::AreEqual(mat4DatumMove.getSize(), (size_t)1);
			Assert::AreEqual(mat4DatumMove.get<glm::mat4>(0), m);

			Datum pointerDatumMove(std::move(pointerDatumCopy));
			Assert::AreEqual(pointerDatumMove.getSize(), (size_t)3);
			Assert::AreEqual(pointerDatumMove.get<Empty*>(0), e);
			Assert::AreEqual(pointerDatumMove.get<Foo*>(1), f);
			Assert::AreEqual(pointerDatumMove.get<FooFoo*>(2), ff);

			Datum scopeDatumMove(std::move(scopeDatumCopy));
			Assert::AreEqual(scopeDatumMove.getSize(), (size_t)2);
			Assert::AreEqual(scopeDatumMove.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeDatumMove.get<Scope*>(1), scope2);

			// delete the pointers
			delete e;
			delete f;
			delete ff;
		}

		TEST_METHOD(TestDatumGetDataType)
		{
			// Test get data type for unknown
			Datum unknownDatum;
			Assert::AreEqual(unknownDatum.getDataType(), DataType::UNKNOWN);

			// Test get data type for int
			Datum intDatum;
			intDatum.push_back(10);
			Assert::AreEqual(intDatum.getDataType(), DataType::INTEGER);

			// Test get data type for float
			Datum floatDatum;
			floatDatum.push_back(0.69f);
			Assert::AreEqual(floatDatum.getDataType(), DataType::FLOAT);

			// Test get data type for string
			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);
			Assert::AreEqual(stringDatum.getDataType(), DataType::STRING);

			// Test get data type for glm::vec4
			Datum vet4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vet4Datum.push_back(v);
			Assert::AreEqual(vet4Datum.getDataType(), DataType::VEC4);

			// Test get data type for glm::mat4
			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);
			Assert::AreEqual(mat4Datum.getDataType(), DataType::MAT4);

			// Test get data type for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Assert::AreEqual(pointerDatum.getDataType(), DataType::POINTER);

			// Test get data type for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Assert::AreEqual(scopeDatum.getDataType(), DataType::TABLE);

			delete e;
		}

		TEST_METHOD(TestDatumPushBack)
		{
			// Test push back for int
			Datum datum;
			datum.push_back(10);
			Assert::AreEqual(datum.getSize(), (size_t)1);
			Assert::AreEqual(datum.get<int>(0), 10);

			datum.push_back(20);
			Assert::AreEqual(datum.getSize(), (size_t)2);
			Assert::AreEqual(datum.get<int>(0), 10);
			Assert::AreEqual(datum.get<int>(1), 20);

			// Test push back for float
			Datum datum2;
			datum2.push_back(0.69f);
			Assert::AreEqual(datum2.getSize(), (size_t)1);
			Assert::AreEqual(datum2.get<float>(0), 0.69f);

			datum2.push_back(0.96f);
			datum2.push_back(0.25f);
			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.69f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.25f);

			// Test push back for string
			Datum datum3;
			std::string s = "string";
			datum3.push_back(s);
			Assert::AreEqual(datum3.getSize(), (size_t)1);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string"));

			// Test push back for glm::vec4
			Datum datum4;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			datum4.push_back(v);
			Assert::AreEqual(datum4.getSize(), (size_t)1);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v);

			// Test push back for glm::mat4
			Datum datum5;
			glm::mat4 m = glm::mat4(1.0f);
			datum5.push_back(m);
			Assert::AreEqual(datum5.getSize(), (size_t)1);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m);

			// Test push back for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)1);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));

			// Test push back for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);

			// Test push back exception
			Assert::ExpectException<std::logic_error>([&] { datum.push_back(0.5f); });
			Assert::ExpectException<std::logic_error>([&] { datum.push_back(s); });
			Assert::ExpectException<std::logic_error>([&] { datum3.push_back(1); });
			Assert::ExpectException<std::logic_error>([&] { datum3.push_back("char_arr"); });
			Assert::ExpectException<std::logic_error>([&] { datum3.push_back(v); });
			Assert::ExpectException<std::logic_error>([&] { datum3.push_back(m); });
			Assert::ExpectException<std::logic_error>([&] { pointerDatum.push_back(scope1); });
			Assert::ExpectException<std::logic_error>([&] { scopeDatum.push_back(e); });

			Datum unknownDatum;
			Assert::ExpectException<std::logic_error>([&] { unknownDatum.push_back("char_arr"); });

			delete e;
		}

		TEST_METHOD(TestDatumPop)
		{
			// Test pop for int
			Datum intDatum;
			intDatum.push_back(10);
			Assert::AreEqual(intDatum.getSize(), (size_t)1);
			Assert::AreEqual(intDatum.get<int>(0), 10);

			intDatum.pop();
			Assert::AreEqual(intDatum.getSize(), (size_t)0);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)1);

			// Test pop for float
			Datum floatDatum;
			floatDatum.push_back(0.69f);
			Assert::AreEqual(floatDatum.getSize(), (size_t)1);
			Assert::AreEqual(floatDatum.get<float>(0), 0.69f);

			floatDatum.pop();
			Assert::AreEqual(floatDatum.getSize(), (size_t)0);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)1);

			// Test pop for string
			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);
			Assert::AreEqual(stringDatum.getSize(), (size_t)1);
			Assert::AreEqual(stringDatum.get<std::string>(0), s);

			stringDatum.pop();
			Assert::AreEqual(stringDatum.getSize(), (size_t)0);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)1);

			// Test pop for glm::vec4
			Datum vet4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vet4Datum.push_back(v);
			Assert::AreEqual(vet4Datum.getSize(), (size_t)1);
			Assert::AreEqual(vet4Datum.get<glm::vec4>(0), v);

			vet4Datum.pop();
			Assert::AreEqual(vet4Datum.getSize(), (size_t)0);
			Assert::AreEqual(vet4Datum.getCapacity(), (size_t)1);

			// Test pop for glm::mat4
			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)1);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), m);

			mat4Datum.pop();
			Assert::AreEqual(mat4Datum.getSize(), (size_t)0);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)1);

			// Test pop for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)1);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));

			pointerDatum.pop();
			Assert::AreEqual(pointerDatum.getSize(), (size_t)0);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)1);

			// Test pop for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);

			scopeDatum.pop();
			Assert::AreEqual(scopeDatum.getSize(), (size_t)0);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)1);

			// Test pop exception
			Assert::ExpectException<std::out_of_range>([&] { intDatum.pop(); });
			Assert::ExpectException<std::out_of_range>([&] { floatDatum.pop(); });
			Assert::ExpectException<std::out_of_range>([&] { stringDatum.pop(); });
			Assert::ExpectException<std::out_of_range>([&] { vet4Datum.pop(); });
			Assert::ExpectException<std::out_of_range>([&] { mat4Datum.pop(); });
			Assert::ExpectException<std::out_of_range>([&] { pointerDatum.pop(); });
			Assert::ExpectException<std::out_of_range>([&] { scopeDatum.pop(); });

			delete e;
		}

		TEST_METHOD(TestDatumAssignment)
		{
			// Test copy assignment
			Datum intDatum;
			intDatum.push_back(99);

			Datum unknownDatum;
			unknownDatum = intDatum;
			Assert::AreEqual(unknownDatum.getSize(), (size_t)1);
			Assert::AreEqual(unknownDatum.get<int>(0), 99);

			// Test move assignment
			Datum unknownDatum2;
			unknownDatum2 = std::move(unknownDatum);
			Assert::AreEqual(unknownDatum2.getSize(), (size_t)1);
			Assert::AreEqual(unknownDatum2.get<int>(0), 99);

			// Test assignment for Unknown
			Datum unknownDatum3;
			unknownDatum3 = 10;

			// Test assignment for int
			Fiea::GameEngine::Datum datum;
			datum = 10;
			Assert::AreEqual(datum.getSize(), (size_t)1);
			Assert::AreEqual(datum.get<int>(0), 10);

			datum = 20;
			Assert::AreEqual(datum.getSize(), (size_t)1);
			Assert::AreEqual(datum.get<int>(0), 20);

			// Test assignment for float
			Datum datum2;
			datum2 = 0.69f;
			Assert::AreEqual(datum2.getSize(), (size_t)1);
			Assert::AreEqual(datum2.get<float>(0), 0.69f);

			datum2 = 0.25f;
			Assert::AreEqual(datum2.getSize(), (size_t)1);
			Assert::AreEqual(datum2.get<float>(0), 0.25f);

			// Test assignment for string
			Datum datum3;
			std::string s = "string";
			datum3 = s;
			Assert::AreEqual(datum3.getSize(), (size_t)1);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string"));

			std::string s2 = "string2";
			datum3 = s2;
			Assert::AreEqual(datum3.getSize(), (size_t)1);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string2"));

			// Test assignment for glm::vec4
			Datum datum4;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			datum4 = v;
			Assert::AreEqual(datum4.getSize(), (size_t)1);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v);

			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			datum4 = v2;
			Assert::AreEqual(datum4.getSize(), (size_t)1);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v2);

			//Test assignment for glm::mat4
			Datum datum5;
			glm::mat4 m = glm::mat4(1.0f);
			datum5 = m;
			Assert::AreEqual(datum5.getSize(), (size_t)1);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m);

			glm::mat4 m2 = glm::mat4(5.0f);
			datum5 = m2;
			Assert::AreEqual(datum5.getSize(), (size_t)1);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m2);

			// Test assignment for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum = e;
			Assert::AreEqual(pointerDatum.getSize(), (size_t)1);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));

			Foo* f = new Foo(10);
			pointerDatum = f;
			Assert::AreEqual(pointerDatum.getSize(), (size_t)1);
			Assert::IsTrue(pointerDatum.get<Foo*>(0)->Equals(f));

			// Test assignment for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum = scope1;
			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);

			Scope* scope2 = (Scope*)0x2;
			scopeDatum = scope2;
			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope2);

			// Test assignment exception
			Assert::ExpectException<std::logic_error>([&] { datum = s; });
			Assert::ExpectException<std::logic_error>([&] { datum = 0.5f; });
			Assert::ExpectException<std::logic_error>([&] { datum3 = 1; });
			Assert::ExpectException<std::logic_error>([&] { datum3 = "char_arr"; });
			Assert::ExpectException<std::logic_error>([&] { datum3 = v; });
			Assert::ExpectException<std::logic_error>([&] { datum3 = m; });
			Assert::ExpectException<std::logic_error>([&] { pointerDatum = scope2; });
			Assert::ExpectException<std::logic_error>([&] { scopeDatum = e; });

			Datum unknownDatum4;
			Assert::ExpectException<std::logic_error>([&] { unknownDatum4 = "char_arr"; });

			int* arr = new int[5] {1, 2, 3, 4, 5};
			unknownDatum4.setStorage(arr, 5);
			Assert::ExpectException<std::runtime_error>([&] { unknownDatum4 = datum; });

			delete e;
			delete f;
			delete[] arr;
		}

		TEST_METHOD(TestDatumEquality)
		{
			Datum unknownDatum1;
			Datum unknownDatum2;
			Datum unknownDatum3;

			Datum intDatum1;
			Datum intDatum2;
			Datum intDatum3;
			intDatum1.push_back(10);
			intDatum2.push_back(10);
			intDatum3.push_back(20);

			Datum floatDatum1;
			Datum floatDatum2;
			Datum floatDatum3;
			floatDatum1.push_back(0.69f);
			floatDatum2.push_back(0.69f);
			floatDatum3.push_back(0.25f);

			Datum stringDatum1;
			Datum stringDatum2;
			Datum stringDatum3;
			std::string s = "string";
			stringDatum1.push_back(s);
			std::string s2 = "string";
			stringDatum2.push_back(s2);
			std::string s3 = "string3";
			stringDatum3.push_back(s3);

			Datum vet4Datum1;
			Datum vet4Datum2;
			Datum vet4Datum3;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vet4Datum1.push_back(v);
			glm::vec4 v2 = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vet4Datum2.push_back(v2);
			glm::vec4 v3 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			vet4Datum3.push_back(v3);

			Datum mat4Datum1;
			Datum mat4Datum2;
			Datum mat4Datum3;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum1.push_back(m);
			glm::mat4 m2 = glm::mat4(1.0f);
			mat4Datum2.push_back(m2);
			glm::mat4 m3 = glm::mat4(5.0f);
			mat4Datum3.push_back(m3);

			Datum pointerDatum1;
			Datum pointerDatum2;
			Datum pointerDatum3;
			Empty* e = new Empty();
			pointerDatum1.push_back(e);
			pointerDatum2.push_back(e);
			FooFoo* ff = new FooFoo(20);
			pointerDatum3.push_back(ff);

			Datum scopeDatum1;
			Datum scopeDatum2;
			Datum scopeDatum3;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum1.push_back(scope1);
			Scope* scope2 = (Scope*)0x1;
			scopeDatum2.push_back(scope2);
			Scope* scope3 = (Scope*)0x3;
			scopeDatum3.push_back(scope3);


			// Test equality for unknown
			Assert::IsTrue(unknownDatum1 == unknownDatum1);
			Assert::IsTrue(unknownDatum1 == unknownDatum2);
			Assert::IsTrue(unknownDatum1 == unknownDatum3);
			Assert::IsTrue(unknownDatum2 == unknownDatum3);

			// Test equality for int
			Assert::IsTrue(intDatum1 == intDatum1);
			Assert::IsTrue(intDatum1 == intDatum2);
			Assert::IsFalse(intDatum1 == intDatum3);
			Assert::IsTrue(intDatum1 != intDatum3);
			Assert::IsFalse(intDatum1 != intDatum2);
			Assert::IsFalse(intDatum1 == floatDatum1);

			intDatum1.push_back(5);
			intDatum2.push_back(5);
			Assert::IsTrue(intDatum1 == intDatum2);
			intDatum2.push_back(6);
			Assert::IsFalse(intDatum1 == intDatum2);

			// Test equality for float
			Assert::IsTrue(floatDatum1 == floatDatum1);
			Assert::IsTrue(floatDatum1 == floatDatum2);
			Assert::IsFalse(floatDatum1 == floatDatum3);
			Assert::IsTrue(floatDatum1 != floatDatum3);
			Assert::IsFalse(floatDatum1 != floatDatum2);
			Assert::IsFalse(floatDatum1 == stringDatum1);

			floatDatum1.push_back(0.5f);
			floatDatum2.push_back(0.5f);
			Assert::IsTrue(floatDatum1 == floatDatum2);
			floatDatum2.push_back(0.6f);
			Assert::IsFalse(floatDatum1 == floatDatum2);

			// Test equality for string
			Assert::IsTrue(stringDatum1 == stringDatum1);
			Assert::IsTrue(stringDatum1 == stringDatum2);
			Assert::IsFalse(stringDatum1 == stringDatum3);
			Assert::IsTrue(stringDatum1 != stringDatum3);
			Assert::IsFalse(stringDatum1 != stringDatum2);
			Assert::IsFalse(stringDatum1 == vet4Datum1);

			std::string s4 = "string4";
			stringDatum1.push_back(s4);
			stringDatum2.push_back(s4);
			Assert::IsTrue(stringDatum1 == stringDatum2);
			std::string s5 = "string4";
			stringDatum2.push_back(s5);
			Assert::IsFalse(stringDatum1 == stringDatum2);

			// Test equality for glm::vec4
			Assert::IsTrue(vet4Datum1 == vet4Datum1);
			Assert::IsTrue(vet4Datum1 == vet4Datum2);
			Assert::IsFalse(vet4Datum1 == vet4Datum3);
			Assert::IsTrue(vet4Datum1 != vet4Datum3);
			Assert::IsFalse(vet4Datum1 != vet4Datum2);
			Assert::IsFalse(vet4Datum1 == mat4Datum1);

			glm::vec4 v4 = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vet4Datum1.push_back(v4);
			vet4Datum2.push_back(v4);
			Assert::IsTrue(vet4Datum1 == vet4Datum2);
			vet4Datum2.push_back(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f));
			Assert::IsFalse(vet4Datum1 == vet4Datum2);

			// Test equality for glm::mat4
			Assert::IsTrue(mat4Datum1 == mat4Datum1);
			Assert::IsTrue(mat4Datum1 == mat4Datum2);
			Assert::IsFalse(mat4Datum1 == mat4Datum3);
			Assert::IsTrue(mat4Datum1 != mat4Datum3);
			Assert::IsFalse(mat4Datum1 != mat4Datum2);
			Assert::IsFalse(mat4Datum1 == unknownDatum1);

			glm::mat4 m4 = glm::mat4(1.0f);
			mat4Datum1.push_back(m4);
			mat4Datum2.push_back(m4);
			Assert::IsTrue(mat4Datum1 == mat4Datum2);
			mat4Datum2.push_back(glm::mat4(5.0f));
			Assert::IsFalse(mat4Datum1 == mat4Datum2);

			// Test equality for pointer
			Assert::IsTrue(pointerDatum1 == pointerDatum1);
			Assert::IsTrue(pointerDatum1 == pointerDatum2);
			Assert::IsFalse(pointerDatum1 == pointerDatum3);
			Assert::IsTrue(pointerDatum1 != pointerDatum3);
			Assert::IsFalse(pointerDatum1 != pointerDatum2);
			Assert::IsFalse(pointerDatum1 == scopeDatum1);

			Empty* e2 = new Empty();
			Empty* e3 = new Empty();
			pointerDatum1.push_back(e2);
			pointerDatum2.push_back(e2);
			Assert::IsTrue(pointerDatum1 == pointerDatum2);
			pointerDatum2.push_back(e3);
			Assert::IsFalse(pointerDatum1 == pointerDatum2);

			// Test equality for table
			Assert::IsTrue(scopeDatum1 == scopeDatum1);
			Assert::IsTrue(scopeDatum1 == scopeDatum2);
			Assert::IsFalse(scopeDatum1 == scopeDatum3);
			Assert::IsTrue(scopeDatum1 != scopeDatum3);
			Assert::IsFalse(scopeDatum1 != scopeDatum2);
			Assert::IsFalse(scopeDatum1 == intDatum1);

			Scope* scope4 = (Scope*)0x4;
			scopeDatum1.push_back(scope4);
			scopeDatum2.push_back(scope4);
			Assert::IsTrue(scopeDatum1 == scopeDatum2);
			scopeDatum2.push_back((Scope*)0x5);
			Assert::IsFalse(scopeDatum1 == scopeDatum2);

			delete e;
			delete e2;
			delete e3;
			delete ff;
		}

		TEST_METHOD(TestDatumEqualityScalar)
		{
			int i = 10;
			int i2 = 20;
			Datum intDatum;
			intDatum.push_back(10);

			float f = 0.69f;
			float f2 = 0.25f;
			Datum floatDatum;
			floatDatum.push_back(0.69f);

			std::string s = "string";
			std::string s2 = "string2";
			Datum stringDatum;
			stringDatum.push_back(s);

			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			Datum vet4Datum;
			vet4Datum.push_back(v);

			glm::mat4 m = glm::mat4(1.0f);
			glm::mat4 m2 = glm::mat4(5.0f);
			Datum mat4Datum;
			mat4Datum.push_back(m);

			Empty* e = new Empty();
			Foo* foo = new Foo(10);
			Datum pointerDatum;
			pointerDatum.push_back(e);

			Scope* scope1 = (Scope*)0x1;
			Scope* scope2 = (Scope*)0x2;
			Datum scopeDatum;
			scopeDatum.push_back(scope1);

			// Test equality for int
			Assert::IsTrue(intDatum == i);
			Assert::IsFalse(intDatum == i2);
			Assert::IsFalse(intDatum != i);
			Assert::IsTrue(intDatum != i2);
			Assert::IsFalse(intDatum == f);
			intDatum.push_back(10);
			Assert::IsFalse(intDatum == i);

			// Test equality for float
			Assert::IsTrue(floatDatum == f);
			Assert::IsFalse(floatDatum == f2);
			Assert::IsFalse(floatDatum != f);
			Assert::IsTrue(floatDatum != f2);
			Assert::IsFalse(floatDatum == s);
			floatDatum.push_back(0.69f);
			Assert::IsFalse(floatDatum == f);

			// Test equality for string
			Assert::IsTrue(stringDatum == s);
			Assert::IsFalse(stringDatum == s2);
			Assert::IsFalse(stringDatum != s);
			Assert::IsTrue(stringDatum != s2);
			Assert::IsFalse(stringDatum == v);
			stringDatum.push_back(s);
			Assert::IsFalse(stringDatum == s);

			// Test equality for glm::vec4
			Assert::IsTrue(vet4Datum == v);
			Assert::IsFalse(vet4Datum == v2);
			Assert::IsFalse(vet4Datum != v);
			Assert::IsTrue(vet4Datum != v2);
			Assert::IsFalse(vet4Datum == m);
			vet4Datum.push_back(v);
			Assert::IsFalse(vet4Datum == v);

			// Test equality for glm::mat4
			Assert::IsTrue(mat4Datum == m);
			Assert::IsFalse(mat4Datum == m2);
			Assert::IsFalse(mat4Datum != m);
			Assert::IsTrue(mat4Datum != m2);
			Assert::IsFalse(mat4Datum == i);
			mat4Datum.push_back(m);
			Assert::IsFalse(mat4Datum == m);

			// Test equality for pointer
			Assert::IsTrue(pointerDatum == e);
			Assert::IsFalse(pointerDatum == nullptr);
			Assert::IsFalse(pointerDatum != e);
			Assert::IsTrue(pointerDatum != foo);
			Assert::IsFalse(pointerDatum == scope1);
			pointerDatum.push_back(e);
			Assert::IsFalse(pointerDatum == e);

			// Test equality for table
			Assert::IsTrue(scopeDatum == scope1);
			Assert::IsFalse(scopeDatum == scope2);
			Assert::IsFalse(scopeDatum != scope1);
			Assert::IsTrue(scopeDatum != scope2);
			Assert::IsFalse(scopeDatum == e);
			scopeDatum.push_back(scope1);
			Assert::IsFalse(scopeDatum == scope1);

			// Test other
			Datum unknownDatum;
			Assert::IsFalse(unknownDatum == i);

			intDatum.clear();
			intDatum.push_back(10);
			Assert::IsFalse(intDatum == "char_arr");

			delete e;
			delete foo;

		}

		TEST_METHOD(TesDatumtSet)
		{
			// Test set for int
			Datum datum;
			datum.push_back(0);
			datum.push_back(20);
			datum.push_back(30);

			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 0);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 30);

			datum.set(0, 100);
			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 100);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 30);

			datum.set(2, 500);
			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 100);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 500);


			// Test set for float
			Datum datum2;
			datum2.push_back(0.69f);
			datum2.push_back(0.96f);
			datum2.push_back(0.25f);

			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.69f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.25f);

			datum2.set(0, 0.99f);
			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.99f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.25f);

			datum2.set(2, 0.55f);
			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.99f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.55f);

			// Test set for string
			Datum datum3;
			std::string s = "string";
			datum3.push_back(s);
			std::string s2 = "string2";
			datum3.push_back(s2);
			std::string s3 = "string3";
			datum3.push_back(s3);

			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string3"));

			std::string s4 = "string4";
			datum3.set(0, s4);
			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string4"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string3"));

			std::string s5 = "string5";
			datum3.set(2, s5);
			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string4"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string5"));

			// Test set for glm::vec4
			Datum datum4;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			datum4.push_back(v);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			datum4.push_back(v2);
			glm::vec4 v3 = glm::vec4(9.0f, 10.0f, 11.0f, 12.0f);
			datum4.push_back(v3);

			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v3);

			glm::vec4 v4 = glm::vec4(13.0f, 14.0f, 15.0f, 16.0f);
			datum4.set(0, v4);
			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v4);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v3);

			glm::vec4 v5 = glm::vec4(17.0f, 18.0f, 19.0f, 20.0f);
			datum4.set(2, v5);
			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v4);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v5);

			// Test set for glm::mat4
			Datum datum5;
			glm::mat4 m = glm::mat4(1.0f);
			datum5.push_back(m);
			glm::mat4 m2 = glm::mat4(2.0f);
			datum5.push_back(m2);
			glm::mat4 m3 = glm::mat4(3.0f);
			datum5.push_back(m3);

			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m3);

			glm::mat4 m4 = glm::mat4(4.0f);
			datum5.set(0, m4);
			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m4);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m3);

			glm::mat4 m5 = glm::mat4(5.0f);
			datum5.set(2, m5);
			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m4);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m5);

			// Test set for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Foo* f = new Foo(10);
			pointerDatum.push_back(f);
			FooFoo* ff = new FooFoo(20);
			pointerDatum.push_back(ff);

			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Foo*>(1)->Equals(f));
			Assert::IsTrue(pointerDatum.get<FooFoo*>(2)->Equals(ff));

			Empty* e2 = new Empty();
			pointerDatum.set(0, e2);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e2));
			Assert::IsTrue(pointerDatum.get<Foo*>(1)->Equals(f));
			Assert::IsTrue(pointerDatum.get<FooFoo*>(2)->Equals(ff));

			Foo* f2 = new Foo(30);
			pointerDatum.set(1, f2);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e2));
			Assert::IsTrue(pointerDatum.get<Foo*>(1)->Equals(f2));
			Assert::IsTrue(pointerDatum.get<FooFoo*>(2)->Equals(ff));

			// Test set for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Scope* scope2 = (Scope*)0x2;
			scopeDatum.push_back(scope2);
			Scope* scope3 = (Scope*)0x3;
			scopeDatum.push_back(scope3);

			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(2), scope3);

			Scope* scope4 = (Scope*)0x4;
			scopeDatum.set(0, scope4);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope4);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(2), scope3);

			Scope* scope5 = (Scope*)0x5;
			scopeDatum.set(2, scope5);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope4);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(2), scope5);

			// Test out of range exception			
			Assert::ExpectException<std::out_of_range>([&] { datum.set(3, 100); });
			Assert::ExpectException<std::out_of_range>([&] { datum2.set(3, 0.99f); });
			Assert::ExpectException<std::out_of_range>([&] { datum3.set(3, s4); });
			Assert::ExpectException<std::out_of_range>([&] { datum4.set(3, v4); });
			Assert::ExpectException<std::out_of_range>([&] { datum5.set(3, m4); });

			// Test type mismatch exception
			std::string s6 = "string6";
			glm::vec4 v6 = glm::vec4(21.0f, 22.0f, 23.0f, 24.0f);
			glm::mat4 m6 = glm::mat4(6.0f);
			Assert::ExpectException<std::logic_error>([&] { datum.set(0, s6); });
			Assert::ExpectException<std::logic_error>([&] { datum.set(0, 0.5f); });
			Assert::ExpectException<std::logic_error>([&] { datum2.set(0, 10); });
			Assert::ExpectException<std::logic_error>([&] { datum2.set(0, v6); });
			Assert::ExpectException<std::logic_error>([&] { datum2.set(0, m6); });
			Assert::ExpectException<std::logic_error>([&] { datum2.set(0, scope4); });
			Assert::ExpectException<std::logic_error>([&] { datum3.set(0, e2); });

			Assert::ExpectException<std::invalid_argument>([&] { datum3.set(0, "char_arr"); }); // char array
			Assert::ExpectException<std::invalid_argument>([&] { datum.set(0, 2147483648); }); // long

			delete e;
			delete e2;
			delete f;
			delete f2;
			delete ff;
		}

		TEST_METHOD(TestDatumSetFromString)
		{
			// Test set from string for int
			Datum datum;
			datum.push_back(0);
			datum.push_back(20);
			datum.push_back(30);

			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 0);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 30);

			datum.setFromString(0, "100");
			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 100);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 30);

			datum.setFromString(2, "500");
			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 100);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 500);

			// Test set from string for float
			Datum datum2;
			datum2.push_back(0.69f);
			datum2.push_back(0.96f);
			datum2.push_back(0.25f);

			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.69f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.25f);

			datum2.setFromString(0, "0.99");
			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.99f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.25f);

			datum2.setFromString(2, "0.55");
			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.99f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.55f);

			// Test set from string for string
			Datum datum3;
			std::string s = "string";
			datum3.push_back(s);
			std::string s2 = "string2";
			datum3.push_back(s2);
			std::string s3 = "string3";
			datum3.push_back(s3);

			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string3"));

			std::string s4 = "string4";
			datum3.setFromString(0, s4);
			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string4"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string3"));

			std::string s5 = "string5";
			datum3.setFromString(2, s5);
			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string4"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string5"));

			// Test set for glm::vec4
			Datum datum4;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			datum4.push_back(v);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			datum4.push_back(v2);
			glm::vec4 v3 = glm::vec4(9.0f, 10.0f, 11.0f, 12.0f);
			datum4.push_back(v3);

			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v3);

			glm::vec4 v4 = glm::vec4(13.0f, 14.0f, 15.0f, 16.0f);
			std::string v4s = glm::to_string(v4).c_str();
			datum4.setFromString(0, v4s);
			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v4);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v3);

			glm::vec4 v5 = glm::vec4(17.0f, 18.0f, 19.0f, 20.0f);
			std::string v5s = glm::to_string(v5).c_str();
			datum4.setFromString(2, v5s);
			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v4);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v5);

			// Test set for glm::mat4
			Datum datum5;
			glm::mat4 m = glm::mat4(1.0f);
			datum5.push_back(m);
			glm::mat4 m2 = glm::mat4(2.0f);
			datum5.push_back(m2);
			glm::mat4 m3 = glm::mat4(3.0f);
			datum5.push_back(m3);

			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m3);

			glm::mat4 m4 = glm::mat4(4.0f);
			std::string m4s = glm::to_string(m4).c_str();
			datum5.setFromString(0, m4s);
			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m4);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m3);

			glm::mat4 m5 = glm::mat4(5.0f);
			std::string m5s = glm::to_string(m5).c_str();
			datum5.setFromString(2, m5s);
			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m4);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m5);

			// Test out of range exception
			Datum unknownDatum;
			Assert::ExpectException<std::out_of_range>([&] { unknownDatum.setFromString(0, "10"); });

			Assert::ExpectException<std::out_of_range>([&] { datum.setFromString(0, "2147483648"); });
			Assert::ExpectException<std::out_of_range>([&] { datum.setFromString(3, m4s); });
			Assert::ExpectException<std::out_of_range>([&] { datum2.setFromString(3, "0.99"); });
			Assert::ExpectException<std::out_of_range>([&] { datum3.setFromString(3, "string4"); });
			Assert::ExpectException<std::out_of_range>([&] { datum4.setFromString(3, v4s); });
			Assert::ExpectException<std::out_of_range>([&] { datum5.setFromString(3, m4s); });

			// Test type mismatch exception
			std::string s6 = "string6";
			glm::vec4 v6 = glm::vec4(21.0f, 22.0f, 23.0f, 24.0f);
			glm::mat4 m6 = glm::mat4(6.0f);
			Assert::ExpectException<std::invalid_argument>([&] { datum.setFromString(0, s6); });
			Assert::ExpectException<std::invalid_argument>([&] { datum2.setFromString(0, s6); });
			Assert::ExpectException<std::invalid_argument>([&] { datum2.setFromString(0, glm::to_string(v6).c_str()); });
			Assert::ExpectException<std::invalid_argument>([&] { datum2.setFromString(0, glm::to_string(m6).c_str()); });
			Assert::ExpectException<std::invalid_argument>([&] { datum4.setFromString(0, "10"); });
			Assert::ExpectException<std::invalid_argument>([&] { datum5.setFromString(0, "0.5"); });
		}

		TEST_METHOD(TestDatumGet)
		{
			// Test get for int
			Datum datum;
			datum.push_back(0);
			datum.push_back(20);
			datum.push_back(30);

			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.get<int>(0), 0);
			Assert::AreEqual(datum.get<int>(1), 20);
			Assert::AreEqual(datum.get<int>(2), 30);

			// Test get for float
			Datum datum2;
			datum2.push_back(0.69f);
			datum2.push_back(0.96f);
			datum2.push_back(0.25f);

			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.get<float>(0), 0.69f);
			Assert::AreEqual(datum2.get<float>(1), 0.96f);
			Assert::AreEqual(datum2.get<float>(2), 0.25f);

			// Test get for string
			Datum datum3;
			std::string s = "string";
			datum3.push_back(s);
			std::string s2 = "string2";
			datum3.push_back(s2);
			std::string s3 = "string3";
			datum3.push_back(s3);

			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.get<std::string>(0), static_cast<std::string>("string"));
			Assert::AreEqual(datum3.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.get<std::string>(2), static_cast<std::string>("string3"));

			// Test get for glm::vec4
			Datum datum4;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			datum4.push_back(v);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			datum4.push_back(v2);
			glm::vec4 v3 = glm::vec4(9.0f, 10.0f, 11.0f, 12.0f);
			datum4.push_back(v3);

			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.get<glm::vec4>(0), v);
			Assert::AreEqual(datum4.get<glm::vec4>(1), v2);
			Assert::AreEqual(datum4.get<glm::vec4>(2), v3);

			// Test get for glm::mat4
			Datum datum5;
			glm::mat4 m = glm::mat4(1.0f);
			datum5.push_back(m);
			glm::mat4 m2 = glm::mat4(2.0f);
			datum5.push_back(m2);
			glm::mat4 m3 = glm::mat4(3.0f);
			datum5.push_back(m3);

			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.get<glm::mat4>(0), m);
			Assert::AreEqual(datum5.get<glm::mat4>(1), m2);
			Assert::AreEqual(datum5.get<glm::mat4>(2), m3);

			// Test get for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Foo* f = new Foo(10);
			pointerDatum.push_back(f);
			FooFoo* ff = new FooFoo(20);
			pointerDatum.push_back(ff);

			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Foo*>(1)->Equals(f));
			Assert::IsTrue(pointerDatum.get<FooFoo*>(2)->Equals(ff));

			// Test get for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Scope* scope2 = (Scope*)0x2;
			scopeDatum.push_back(scope2);
			Scope* scope3 = (Scope*)0x3;
			scopeDatum.push_back(scope3);

			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(2), scope3);

			// Test out of range exception
			Datum unknownDatum;
			Assert::ExpectException<std::out_of_range>([&] { unknownDatum.get<int>(0); });

			Assert::ExpectException<std::out_of_range>([&] { datum.get<int>(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum2.get<float>(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum3.get<std::string>(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum4.get<glm::vec4>(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum5.get<glm::mat4>(3); });

			// Test type mismatch exception
			Assert::ExpectException<std::logic_error>([&] { datum.get<float>(0); });
			Assert::ExpectException<std::logic_error>([&] { datum2.get<std::string>(0); });
			Assert::ExpectException<std::logic_error>([&] { datum3.get<glm::vec4>(0); });
			Assert::ExpectException<std::logic_error>([&] { datum4.get<glm::mat4>(0); });
			Assert::ExpectException<std::logic_error>([&] { datum5.get<int>(0); });
			Assert::ExpectException<std::logic_error>([&] { datum.get<Empty*>(0); });
			Assert::ExpectException<std::logic_error>([&] { datum.get<Scope*>(0); });

			// Test invalid argument exception
			Assert::ExpectException<std::invalid_argument>([&] { datum.get<char>(0); });

			delete e;
			delete f;
			delete ff;
		}

		TEST_METHOD(TestDatumGetConst)
		{
			// Test get const for int
			Datum intDatum;
			intDatum.push_back(99);
			const Datum intConstDatum(intDatum);

			Assert::AreEqual(intConstDatum.getSize(), (size_t)1);
			Assert::AreEqual(intConstDatum.get<int>(0), 99);

			// Test get const for float
			Datum floatDatum;
			floatDatum.push_back(0.69f);
			const Datum floatConstDatum(floatDatum);

			Assert::AreEqual(floatConstDatum.getSize(), (size_t)1);
			Assert::AreEqual(floatConstDatum.get<float>(0), 0.69f);

			// Test get const for string
			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);
			const Datum stringConstDatum(stringDatum);

			Assert::AreEqual(stringConstDatum.getSize(), (size_t)1);
			Assert::AreEqual(stringConstDatum.get<std::string>(0), s);

			// Test get const for glm::vec4
			Datum vec4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vec4Datum.push_back(v);
			const Datum vec4ConstDatum(vec4Datum);

			Assert::AreEqual(vec4ConstDatum.getSize(), (size_t)1);
			Assert::AreEqual(vec4ConstDatum.get<glm::vec4>(0), v);

			// Test get const for glm::mat4
			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);
			const Datum mat4ConstDatum(mat4Datum);

			Assert::AreEqual(mat4ConstDatum.getSize(), (size_t)1);
			Assert::AreEqual(mat4ConstDatum.get<glm::mat4>(0), m);

			// Test get const for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			const Datum pointerConstDatum(pointerDatum);

			Assert::AreEqual(pointerConstDatum.getSize(), (size_t)1);
			Assert::IsTrue(pointerConstDatum.get<Empty*>(0)->Equals(e));

			// Test get const for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			const Datum scopeConstDatum(scopeDatum);

			Assert::AreEqual(scopeConstDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeConstDatum.get<Scope*>(0), scope1);

			// Test out of range exception
			Assert::ExpectException<std::out_of_range>([&] { intConstDatum.get<int>(1); });
			Assert::ExpectException<std::out_of_range>([&] { floatConstDatum.get<float>(1); });
			Assert::ExpectException<std::out_of_range>([&] { stringConstDatum.get<std::string>(1); });
			Assert::ExpectException<std::out_of_range>([&] { vec4ConstDatum.get<glm::vec4>(1); });
			Assert::ExpectException<std::out_of_range>([&] { mat4ConstDatum.get<glm::mat4>(1); });

			// Test type mismatch exception
			Assert::ExpectException<std::logic_error>([&] { intConstDatum.get<float>(0); });
			Assert::ExpectException<std::logic_error>([&] { floatConstDatum.get<std::string>(0); });
			Assert::ExpectException<std::logic_error>([&] { stringConstDatum.get<glm::vec4>(0); });
			Assert::ExpectException<std::logic_error>([&] { vec4ConstDatum.get<glm::mat4>(0); });
			Assert::ExpectException<std::logic_error>([&] { mat4ConstDatum.get<int>(0); });
			Assert::ExpectException<std::logic_error>([&] { intConstDatum.get<Empty*>(0); });
			Assert::ExpectException<std::logic_error>([&] { intConstDatum.get<Scope*>(0); });

			// Test invalid argument exception
			Assert::ExpectException<std::invalid_argument>([&] { intConstDatum.get<double>(0); });

			delete e;
		}

		TEST_METHOD(TestDatumGetAsString)
		{
			// Test get as string for int
			Datum datum;
			datum.push_back(0);
			datum.push_back(20);
			datum.push_back(30);

			Assert::AreEqual(datum.getSize(), (size_t)3);
			Assert::AreEqual(datum.getAsString(0), static_cast<std::string>("0"));
			Assert::AreEqual(datum.getAsString(1), static_cast<std::string>("20"));
			Assert::AreEqual(datum.getAsString(2), static_cast<std::string>("30"));

			// Test get as string for float
			Datum datum2;
			datum2.push_back(0.69f);
			datum2.push_back(0.96f);
			datum2.push_back(0.25f);

			Assert::AreEqual(datum2.getSize(), (size_t)3);
			Assert::AreEqual(datum2.getAsString(0), static_cast<std::string>("0.690000"));
			Assert::AreEqual(datum2.getAsString(1), static_cast<std::string>("0.960000"));
			Assert::AreEqual(datum2.getAsString(2), static_cast<std::string>("0.250000"));

			// Test get as string for string
			Datum datum3;
			std::string s = "string";
			datum3.push_back(s);
			std::string s2 = "string2";
			datum3.push_back(s2);
			std::string s3 = "string3";
			datum3.push_back(s3);

			Assert::AreEqual(datum3.getSize(), (size_t)3);
			Assert::AreEqual(datum3.getAsString(0), static_cast<std::string>("string"));
			Assert::AreEqual(datum3.getAsString(1), static_cast<std::string>("string2"));
			Assert::AreEqual(datum3.getAsString(2), static_cast<std::string>("string3"));

			// Test get as string for glm::vec4
			Datum datum4;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			datum4.push_back(v);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			datum4.push_back(v2);

			glm::vec4 v3 = glm::vec4(9.0f, 10.0f, 11.0f, 12.0f);
			datum4.push_back(v3);

			Assert::AreEqual(datum4.getSize(), (size_t)3);
			Assert::AreEqual(datum4.getAsString(0), glm::to_string(v));
			Assert::AreEqual(datum4.getAsString(1), glm::to_string(v2));
			Assert::AreEqual(datum4.getAsString(2), glm::to_string(v3));

			// Test get as string for glm::mat4
			Datum datum5;
			glm::mat4 m = glm::mat4(1.0f);
			datum5.push_back(m);
			glm::mat4 m2 = glm::mat4(2.0f);
			datum5.push_back(m2);
			glm::mat4 m3 = glm::mat4(3.0f);
			datum5.push_back(m3);

			Assert::AreEqual(datum5.getSize(), (size_t)3);
			Assert::AreEqual(datum5.getAsString(0), glm::to_string(m));
			Assert::AreEqual(datum5.getAsString(1), glm::to_string(m2));
			Assert::AreEqual(datum5.getAsString(2), glm::to_string(m3));

			// Test get as string for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Foo* f = new Foo(10);
			pointerDatum.push_back(f);
			FooFoo* ff = new FooFoo(20);
			pointerDatum.push_back(ff);

			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::AreEqual(pointerDatum.getAsString(0), e->ToString());
			Assert::AreEqual(pointerDatum.getAsString(1), f->ToString());
			Assert::AreEqual(pointerDatum.getAsString(2), ff->ToString());

			// Test get as string for table
			Datum scopeDatum;
			Scope* scope1 = new Scope();
			scope1->append("A");
			scopeDatum.push_back(scope1);
			scope1->find("A")->push_back(10);

			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.getAsString(0), scope1->ToString());
 
			// Test out of range exception
			Datum unknownDatum;
			Assert::ExpectException<std::logic_error>([&] { unknownDatum.getAsString(0); });

			Assert::ExpectException<std::out_of_range>([&] { datum.getAsString(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum2.getAsString(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum3.getAsString(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum4.getAsString(3); });
			Assert::ExpectException<std::out_of_range>([&] { datum5.getAsString(3); });

			delete e;
			delete f;
			delete ff;
			delete scope1;
		}

		TEST_METHOD(TestDatumRemove)
		{
			// Test remove for int
			Datum intDatum;
			intDatum.push_back(99);
			intDatum.push_back(100);

			Assert::AreEqual(intDatum.getSize(), (size_t)2);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(intDatum.get<int>(0), 99);
			Assert::AreEqual(intDatum.get<int>(1), 100);

			intDatum.remove(0);
			Assert::AreEqual(intDatum.getSize(), (size_t)1);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(intDatum.get<int>(0), 100);

			// Test remove for float
			Datum floatDatum;
			floatDatum.push_back(0.69f);
			floatDatum.push_back(0.96f);

			Assert::AreEqual(floatDatum.getSize(), (size_t)2);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(floatDatum.get<float>(0), 0.69f);
			Assert::AreEqual(floatDatum.get<float>(1), 0.96f);

			floatDatum.remove(0);
			Assert::AreEqual(floatDatum.getSize(), (size_t)1);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(floatDatum.get<float>(0), 0.96f);

			// Test remove for string
			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);
			std::string s2 = "string2";
			stringDatum.push_back(s2);

			Assert::AreEqual(stringDatum.getSize(), (size_t)2);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(stringDatum.get<std::string>(0), static_cast<std::string>("string"));
			Assert::AreEqual(stringDatum.get<std::string>(1), static_cast<std::string>("string2"));

			stringDatum.remove(0);
			Assert::AreEqual(stringDatum.getSize(), (size_t)1);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(stringDatum.get<std::string>(0), static_cast<std::string>("string2"));

			// Test remove for glm::vec4
			Datum vec4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vec4Datum.push_back(v);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			vec4Datum.push_back(v2);

			Assert::AreEqual(vec4Datum.getSize(), (size_t)2);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)2);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(0), v);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(1), v2);

			vec4Datum.remove(0);
			Assert::AreEqual(vec4Datum.getSize(), (size_t)1);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)2);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(0), v2);

			// Test remove for glm::mat4
			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);
			glm::mat4 m2 = glm::mat4(2.0f);
			mat4Datum.push_back(m2);

			Assert::AreEqual(mat4Datum.getSize(), (size_t)2);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)2);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), m);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(1), m2);

			mat4Datum.remove(0);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)1);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)2);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), m2);

			// Test remove for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Foo* f = new Foo(10);
			pointerDatum.push_back(f);
			FooFoo* ff = new FooFoo(20);
			pointerDatum.push_back(ff);

			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Foo*>(1)->Equals(f));
			Assert::IsTrue(pointerDatum.get<FooFoo*>(2)->Equals(ff));

			pointerDatum.remove(0);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)2);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Foo*>(0)->Equals(f));
			Assert::IsTrue(pointerDatum.get<FooFoo*>(1)->Equals(ff));

			// Test remove for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Scope* scope2 = (Scope*)0x2;
			scopeDatum.push_back(scope2);
			Scope* scope3 = (Scope*)0x3;
			scopeDatum.push_back(scope3);

			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(2), scope3);

			scopeDatum.remove(0);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)2);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope3);

			// Test out of range exception
			Datum unknownDatum;
			Assert::ExpectException<std::out_of_range>([&] { unknownDatum.remove(0); });

			Assert::ExpectException<std::out_of_range>([&] { intDatum.remove(1); });
			Assert::ExpectException<std::out_of_range>([&] { floatDatum.remove(1); });
			Assert::ExpectException<std::out_of_range>([&] { stringDatum.remove(1); });
			Assert::ExpectException<std::out_of_range>([&] { vec4Datum.remove(1); });
			Assert::ExpectException<std::out_of_range>([&] { mat4Datum.remove(1); });

			delete e;
			delete f;
			delete ff;

		}

		TEST_METHOD(TestDatumResize)
		{
			// Test resize for int
			Datum intDatum;
			intDatum.push_back(99);
			Assert::AreEqual(intDatum.getSize(), (size_t)1);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)1);

			intDatum.resize(3);
			Assert::AreEqual(intDatum.getSize(), (size_t)3);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(intDatum.get<int>(0), 99);
			Assert::AreEqual(intDatum.get<int>(1), 0);
			Assert::AreEqual(intDatum.get<int>(2), 0);

			intDatum.resize(2);
			Assert::AreEqual(intDatum.getSize(), (size_t)2);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(intDatum.get<int>(0), 99);
			Assert::AreEqual(intDatum.get<int>(1), 0);

			// Test resize for float
			Datum floatDatum;
			floatDatum.push_back(0.69f);
			Assert::AreEqual(floatDatum.getSize(), (size_t)1);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)1);

			floatDatum.resize(3);
			Assert::AreEqual(floatDatum.getSize(), (size_t)3);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(floatDatum.get<float>(0), 0.69f);
			Assert::AreEqual(floatDatum.get<float>(1), 0.0f);
			Assert::AreEqual(floatDatum.get<float>(2), 0.0f);

			floatDatum.resize(2);
			Assert::AreEqual(floatDatum.getSize(), (size_t)2);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(floatDatum.get<float>(0), 0.69f);
			Assert::AreEqual(floatDatum.get<float>(1), 0.0f);

			// Test resize for string
			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);
			Assert::AreEqual(stringDatum.getSize(), (size_t)1);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)1);

			stringDatum.resize(3);
			Assert::AreEqual(stringDatum.getSize(), (size_t)3);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(stringDatum.get<std::string>(0), static_cast<std::string>("string"));
			Assert::AreEqual(stringDatum.get<std::string>(1), static_cast<std::string>(""));
			Assert::AreEqual(stringDatum.get<std::string>(2), static_cast<std::string>(""));

			stringDatum.resize(2);
			Assert::AreEqual(stringDatum.getSize(), (size_t)2);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(stringDatum.get<std::string>(0), static_cast<std::string>("string"));
			Assert::AreEqual(stringDatum.get<std::string>(1), static_cast<std::string>(""));

			// Test resize for glm::vec4
			Datum vec4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vec4Datum.push_back(v);
			Assert::AreEqual(vec4Datum.getSize(), (size_t)1);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)1);

			vec4Datum.resize(3);
			Assert::AreEqual(vec4Datum.getSize(), (size_t)3);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)3);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(0), v);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(1), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			Assert::AreEqual(vec4Datum.get<glm::vec4>(2), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			vec4Datum.resize(2);
			Assert::AreEqual(vec4Datum.getSize(), (size_t)2);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)2);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(0), v);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(1), glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

			// Test resize for glm::mat4
			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)1);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)1);

			mat4Datum.resize(3);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)3);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)3);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), m);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(1), glm::mat4(0.0f));
			Assert::AreEqual(mat4Datum.get<glm::mat4>(2), glm::mat4(0.0f));

			mat4Datum.resize(2);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)2);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)2);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), m);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(1), glm::mat4(0.0f));

			// Test resize for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)1);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)1);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));

			pointerDatum.resize(3);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Empty*>(1) == nullptr);
			Assert::IsTrue(pointerDatum.get<Empty*>(2) == nullptr);

			pointerDatum.resize(2);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)2);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)2);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Empty*>(1) == nullptr);

			// Test resize for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)1);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);

			scopeDatum.resize(3);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);
			Assert::IsTrue(scopeDatum.get<Scope*>(1) == nullptr);
			Assert::IsTrue(scopeDatum.get<Scope*>(2) == nullptr);

			scopeDatum.resize(2);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)2);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)2);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);
			Assert::IsTrue(scopeDatum.get<Scope*>(1) == nullptr);

			// Test resize exception
			Datum unknownDatum;
			Assert::ExpectException<std::logic_error>([&] { unknownDatum.resize(10); });

			delete e;
		}

		TEST_METHOD(TestDatumClear)
		{
			// Test clear for int
			Datum intDatum;
			intDatum.push_back(99);
			Assert::AreEqual(intDatum.getSize(), (size_t)1);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)1);

			intDatum.clear();
			Assert::AreEqual(intDatum.getSize(), (size_t)0);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)1);

			// Test clear for float
			Datum floatDatum;
			floatDatum.push_back(0.69f);
			Assert::AreEqual(floatDatum.getSize(), (size_t)1);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)1);

			floatDatum.clear();
			Assert::AreEqual(floatDatum.getSize(), (size_t)0);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)1);

			// Test clear for string
			Datum stringDatum;
			std::string s = "string";
			stringDatum.push_back(s);
			Assert::AreEqual(stringDatum.getSize(), (size_t)1);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)1);

			stringDatum.clear();
			Assert::AreEqual(stringDatum.getSize(), (size_t)0);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)1);

			// Test clear for glm::vec4
			Datum vec4Datum;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			vec4Datum.push_back(v);
			Assert::AreEqual(vec4Datum.getSize(), (size_t)1);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)1);

			vec4Datum.clear();
			Assert::AreEqual(vec4Datum.getSize(), (size_t)0);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)1);

			// Test clear for glm::mat4
			Datum mat4Datum;
			glm::mat4 m = glm::mat4(1.0f);
			mat4Datum.push_back(m);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)1);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)1);

			mat4Datum.clear();
			Assert::AreEqual(mat4Datum.getSize(), (size_t)0);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)1);

			// Test clear for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			pointerDatum.push_back(e);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)1);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)1);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));

			pointerDatum.clear();
			Assert::AreEqual(pointerDatum.getSize(), (size_t)0);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)1);

			// Test clear for table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			scopeDatum.push_back(scope1);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)1);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)1);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);

			scopeDatum.clear();
			Assert::AreEqual(scopeDatum.getSize(), (size_t)0);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)1);

			// Test exception
			Datum unknownDatum;
			//Assert::ExpectException<std::logic_error>([&] { unknownDatum.clear(); });

			delete e;
		}

		TEST_METHOD(TestDatumSetStorage)
		{
			// Test copy constructor
			Datum initalDatum;
			int* initialArray = new int[3] { 1, 2, 3 };
			initalDatum.setStorage(initialArray, 3);

			Datum CopyDatum(initalDatum);
			Assert::AreEqual(CopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(CopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(CopyDatum.get<int>(0), 1);
			Assert::AreEqual(CopyDatum.get<int>(1), 2);
			Assert::AreEqual(CopyDatum.get<int>(2), 3);

			// Test move constructor
			Datum MoveDatum(std::move(initalDatum));
			Assert::AreEqual(MoveDatum.getSize(), (size_t)3);
			Assert::AreEqual(MoveDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(MoveDatum.get<int>(0), 1);
			Assert::AreEqual(MoveDatum.get<int>(1), 2);
			Assert::AreEqual(MoveDatum.get<int>(2), 3);

			// Test copy assignment operator
			Datum AssignmentDatum;
			int* assignmentArray = new int[1] { 0 };
			AssignmentDatum.setStorage(assignmentArray, 1);
			AssignmentDatum = MoveDatum;
			delete[] assignmentArray;

			Assert::AreEqual(AssignmentDatum.getSize(), (size_t)3);
			Assert::AreEqual(AssignmentDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(AssignmentDatum.get<int>(0), 1);
			Assert::AreEqual(AssignmentDatum.get<int>(1), 2);
			Assert::AreEqual(AssignmentDatum.get<int>(2), 3);

			// Test move assignment operator
			Datum MoveAssignmentDatum;
			int* moveAssignmentArray = new int[1] { 0 };
			MoveAssignmentDatum.setStorage(moveAssignmentArray, 1);
			MoveAssignmentDatum = std::move(AssignmentDatum);
			delete[] moveAssignmentArray;

			Assert::AreEqual(MoveAssignmentDatum.getSize(), (size_t)3);
			Assert::AreEqual(MoveAssignmentDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(MoveAssignmentDatum.get<int>(0), 1);
			Assert::AreEqual(MoveAssignmentDatum.get<int>(1), 2);
			Assert::AreEqual(MoveAssignmentDatum.get<int>(2), 3);

			// Test set storage for int
			Datum intDatum;
			int* intArray = new int[3] { 1, 2, 3 };
			intDatum.setStorage(intArray, 3);
			Assert::AreEqual(intDatum.getSize(), (size_t)3);
			Assert::AreEqual(intDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(intDatum.get<int>(0), 1);
			Assert::AreEqual(intDatum.get<int>(1), 2);
			Assert::AreEqual(intDatum.get<int>(2), 3);
			Assert::AreEqual(intDatum.getAsString(2), static_cast<std::string>("3"));

			intDatum.set(0, 4);
			Assert::AreEqual(intDatum.get<int>(0), 4);

			intDatum.setFromString(1, "5");
			Assert::AreEqual(intDatum.get<int>(1), 5);
			Assert::AreEqual(intDatum.getAsString(1), static_cast<std::string>("5"));

			// Test set storage for float
			Datum floatDatum;
			float* floatArray = new float[3] { 1.0f, 2.0f, 3.0f };
			floatDatum.setStorage(floatArray, 3);
			Assert::AreEqual(floatDatum.getSize(), (size_t)3);
			Assert::AreEqual(floatDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(floatDatum.get<float>(0), 1.0f);
			Assert::AreEqual(floatDatum.get<float>(1), 2.0f);
			Assert::AreEqual(floatDatum.get<float>(2), 3.0f);

			floatDatum.set(0, 4.0f);
			Assert::AreEqual(floatDatum.get<float>(0), 4.0f);

			floatDatum.setFromString(1, "5.0");
			Assert::AreEqual(floatDatum.get<float>(1), 5.0f);
			Assert::AreEqual(floatDatum.getAsString(1), static_cast<std::string>("5.000000"));

			// Test set storage for string
			Datum stringDatum;
			std::string* stringArray = new std::string[3]{ "string1", "string2", "string3" };
			stringDatum.setStorage(stringArray, 3);
			Assert::AreEqual(stringDatum.getSize(), (size_t)3);
			Assert::AreEqual(stringDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(stringDatum.get<std::string>(0), static_cast<std::string>("string1"));
			Assert::AreEqual(stringDatum.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(stringDatum.get<std::string>(2), static_cast<std::string>("string3"));

			std::string s = "string4";
			stringDatum.set(0, s);
			Assert::AreEqual(stringDatum.get<std::string>(0), s);

			std::string s2 = "string5";
			stringDatum.setFromString(1, s2);
			Assert::AreEqual(stringDatum.get<std::string>(1), s2);
			Assert::AreEqual(stringDatum.getAsString(1), s2);

			// Test set storage for glm::vec4
			Datum vec4Datum;
			glm::vec4* vec4Array = new glm::vec4[3]
			{
				glm::vec4(1.0f, 2.0f, 3.0f, 4.0f),
				glm::vec4(5.0f, 6.0f, 7.0f, 8.0f),
				glm::vec4(9.0f, 10.0f, 11.0f, 12.0f)
			};
			vec4Datum.setStorage(vec4Array, 3);
			Assert::AreEqual(vec4Datum.getSize(), (size_t)3);
			Assert::AreEqual(vec4Datum.getCapacity(), (size_t)3);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(0), vec4Array[0]);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(1), vec4Array[1]);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(2), vec4Array[2]);

			glm::vec4 v = glm::vec4(13.0f, 14.0f, 15.0f, 16.0f);
			vec4Datum.set(0, v);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(0), v);

			std::string v2s = glm::to_string(glm::vec4(17.0f, 18.0f, 19.0f, 20.0f)).c_str();
			vec4Datum.setFromString(1, v2s);
			Assert::AreEqual(vec4Datum.get<glm::vec4>(1), glm::vec4(17.0f, 18.0f, 19.0f, 20.0f));
			Assert::AreEqual(vec4Datum.getAsString(1), v2s);

			// Test set storage for glm::mat4
			Datum mat4Datum;
			glm::mat4* mat4Array = new glm::mat4[3]
			{
				glm::mat4(1.0f),
				glm::mat4(2.0f),
				glm::mat4(3.0f)
			};
			mat4Datum.setStorage(mat4Array, 3);
			Assert::AreEqual(mat4Datum.getSize(), (size_t)3);
			Assert::AreEqual(mat4Datum.getCapacity(), (size_t)3);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), mat4Array[0]);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(1), mat4Array[1]);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(2), mat4Array[2]);

			glm::mat4 m = glm::mat4(4.0f);
			mat4Datum.set(0, m);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(0), m);

			std::string m2s = glm::to_string(glm::mat4(5.0f)).c_str();
			mat4Datum.setFromString(1, m2s);
			Assert::AreEqual(mat4Datum.get<glm::mat4>(1), glm::mat4(5.0f));
			Assert::AreEqual(mat4Datum.getAsString(1), m2s);

			// Test set storage for scope*
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			Scope** scopeArray = new Scope * [3] { scope1, scope1, scope1 };
			scopeDatum.setStorage(scopeArray, 3);
			Assert::AreEqual(scopeDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeDatum.get<Scope*>(1), scope1);
			Assert::AreEqual(scopeDatum.get<Scope*>(2), scope1);

			Scope* scope2 = new Scope();
			std::string scope2s = scope2->ToString();
			scopeDatum.set(0, scope2);
			Assert::AreEqual(scopeDatum.get<Scope*>(0), scope2);
			Assert::AreEqual(scopeDatum.getAsString(0), scope2s);

			// Test set storage for pointer*
			Datum pointerDatum;
			Empty* e = new Empty();
			Empty** pointerArray = new Empty * [3] { e, e, e };
			pointerDatum.setStorage(pointerArray, 3);
			Assert::AreEqual(pointerDatum.getSize(), (size_t)3);
			Assert::AreEqual(pointerDatum.getCapacity(), (size_t)3);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Empty*>(1)->Equals(e));
			Assert::IsTrue(pointerDatum.get<Empty*>(2)->Equals(e));

			Empty* e2 = new Empty();
			std::string e2s = e2->ToString();
			pointerDatum.set(0, e2);
			Assert::IsTrue(pointerDatum.get<Empty*>(0)->Equals(e2));
			Assert::AreEqual(pointerDatum.getAsString(0), e2s);

			// Test exception for initialized Datum
			Assert::ExpectException<std::logic_error>([&] { intDatum.setStorage(floatArray, 3); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.setStorage(intArray, 3); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.setStorage(stringArray, 3); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.setStorage(vec4Array, 3); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.setStorage(mat4Array, 3); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.setStorage(scopeArray, 3); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.setStorage(pointerArray, 3); });

			Datum testDatum;
			testDatum.push_back(0.69f);
			Assert::ExpectException<std::logic_error>([&] { testDatum.setStorage(intArray, 3); });

			// Test exception for setStorage for unsupported type
			Datum exceptionDatum;
			double* exceptionArray = new double[3] { 1.0, 2.0, 3.0 };
			Assert::ExpectException<std::logic_error>([&] { exceptionDatum.setStorage(exceptionArray, 0); });
			delete[] exceptionArray;

			// Test exception for set
			Assert::ExpectException<std::invalid_argument>([&] { intDatum.set(0, "char_str"); });
			Assert::ExpectException<std::invalid_argument>([&] { intDatum.set(0, 2147483648); });

			// Test exception for set type mismatch
			Assert::ExpectException<std::logic_error>([&] { intDatum.set(0, 1.0f); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.set(0, 1); });
			Assert::ExpectException<std::logic_error>([&] { stringDatum.set(0, m); });
			Assert::ExpectException<std::logic_error>([&] { vec4Datum.set(0, s); });
			Assert::ExpectException<std::logic_error>([&] { mat4Datum.set(0, v); });

			// Test exception for get
			Assert::ExpectException<std::invalid_argument>([&] { intDatum.get<char>(0); });

			// Test exception for remove
			Assert::ExpectException<std::runtime_error>([&] { intDatum.remove(0); });

			// Test exception for pop
			Assert::ExpectException<std::runtime_error>([&] { intDatum.pop(); });

			// Test exception for push back
			Assert::ExpectException<std::runtime_error>([&] { intDatum.push_back(1); });

			// Test exception for clear
			Assert::ExpectException<std::runtime_error>([&] { intDatum.clear(); });

			// Test exception for resize
			Assert::ExpectException<std::runtime_error>([&] { intDatum.resize(5); });

			// Test exception for assignment operator
			Datum unknownDatum;
			unknownDatum.push_back(1);
			Assert::ExpectException<std::runtime_error>([&] { floatDatum = unknownDatum; });
			Assert::ExpectException<std::runtime_error>([&] { floatDatum = intDatum; });
			Assert::ExpectException<std::runtime_error>([&] { floatDatum = std::move(intDatum); });
			Assert::ExpectException<std::runtime_error>([&] { intDatum = 2; });

			// Test exception for get type mismatch
			Assert::ExpectException<std::logic_error>([&] { intDatum.get<float>(0); });
			Assert::ExpectException<std::logic_error>([&] { floatDatum.get<std::string>(0); });
			Assert::ExpectException<std::logic_error>([&] { stringDatum.get<glm::vec4>(0); });
			Assert::ExpectException<std::logic_error>([&] { vec4Datum.get<glm::mat4>(0); });
			Assert::ExpectException<std::logic_error>([&] { mat4Datum.get<int>(0); });

			// delete the array
			delete[] initialArray;
			delete[] intArray;
			delete[] floatArray;
			delete[] stringArray;
			delete[] vec4Array;
			delete[] mat4Array;
			delete[] scopeArray;
			delete e;
			delete e2;
			delete[] pointerArray;
			delete scope2;
		}

		TEST_METHOD(TestDatumSetStorageCopy)
		{
			// Test copy for int
			Datum intDatum;
			int* intArray = new int[3] { 1, 2, 3 };
			intDatum.setStorage(intArray, 3);

			Datum intCopyDatum(intDatum);
			Assert::AreEqual(intCopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(intCopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(intCopyDatum.get<int>(0), 1);
			Assert::AreEqual(intCopyDatum.get<int>(1), 2);
			Assert::AreEqual(intCopyDatum.get<int>(2), 3);

			// Test copy for float
			Datum floatDatum;
			float* floatArray = new float[3] { 1.0f, 2.0f, 3.0f };
			floatDatum.setStorage(floatArray, 3);

			Datum floatCopyDatum(floatDatum);
			Assert::AreEqual(floatCopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(floatCopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(floatCopyDatum.get<float>(0), 1.0f);
			Assert::AreEqual(floatCopyDatum.get<float>(1), 2.0f);
			Assert::AreEqual(floatCopyDatum.get<float>(2), 3.0f);

			// Test copy for string
			Datum stringDatum;
			std::string* stringArray = new std::string[3]{ "string1", "string2", "string3" };
			stringDatum.setStorage(stringArray, 3);

			Datum stringCopyDatum(stringDatum);
			Assert::AreEqual(stringCopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(stringCopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(stringCopyDatum.get<std::string>(0), static_cast<std::string>("string1"));
			Assert::AreEqual(stringCopyDatum.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(stringCopyDatum.get<std::string>(2), static_cast<std::string>("string3"));

			// Test copy for glm::vec4
			Datum vec4Datum;
			glm::vec4* vec4Array = new glm::vec4[3]
			{
				glm::vec4(1.0f, 2.0f, 3.0f, 4.0f),
				glm::vec4(5.0f, 6.0f, 7.0f, 8.0f),
				glm::vec4(9.0f, 10.0f, 11.0f, 12.0f)
			};
			vec4Datum.setStorage(vec4Array, 3);

			Datum vec4CopyDatum(vec4Datum);
			Assert::AreEqual(vec4CopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(vec4CopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(vec4CopyDatum.get<glm::vec4>(0), vec4Array[0]);
			Assert::AreEqual(vec4CopyDatum.get<glm::vec4>(1), vec4Array[1]);
			Assert::AreEqual(vec4CopyDatum.get<glm::vec4>(2), vec4Array[2]);

			// Test copy for glm::mat4
			Datum mat4Datum;
			glm::mat4* mat4Array = new glm::mat4[3]
			{
				glm::mat4(1.0f),
				glm::mat4(2.0f),
				glm::mat4(3.0f)
			};
			mat4Datum.setStorage(mat4Array, 3);

			Datum mat4CopyDatum(mat4Datum);
			Assert::AreEqual(mat4CopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(mat4CopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(mat4CopyDatum.get<glm::mat4>(0), mat4Array[0]);
			Assert::AreEqual(mat4CopyDatum.get<glm::mat4>(1), mat4Array[1]);
			Assert::AreEqual(mat4CopyDatum.get<glm::mat4>(2), mat4Array[2]);

			// Test copy for Table
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			Scope** scopeArray = new Scope * [3] { scope1, scope1, scope1 };
			scopeDatum.setStorage(scopeArray, 3);

			Datum scopeCopyDatum(scopeDatum);
			Assert::AreEqual(scopeCopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeCopyDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(scopeCopyDatum.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeCopyDatum.get<Scope*>(1), scope1);
			Assert::AreEqual(scopeCopyDatum.get<Scope*>(2), scope1);

			// Test copy for pointer
			Datum pointerDatum;
			Empty* e = new Empty();
			Empty** pointerArray = new Empty * [3] { e, e, e };
			pointerDatum.setStorage(pointerArray, 3);

			Datum pointerCopyDatum(pointerDatum);
			Assert::AreEqual(pointerCopyDatum.getSize(), (size_t)3);
			Assert::AreEqual(pointerCopyDatum.getCapacity(), (size_t)3);
			Assert::IsTrue(pointerCopyDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerCopyDatum.get<Empty*>(1)->Equals(e));
			Assert::IsTrue(pointerCopyDatum.get<Empty*>(2)->Equals(e));

			// delete the array
			delete[] intArray;
			delete[] floatArray;
			delete[] stringArray;
			delete[] vec4Array;
			delete[] mat4Array;
			delete[] scopeArray;
			delete e;
			delete[] pointerArray;
		}

		TEST_METHOD(TestDatumGetExteranlConst)
		{
			// Test get const for int
			Datum intDatum;
			int* intArray = new int[3] { 1, 2, 3 };
			intDatum.setStorage(intArray, 3);
			const Datum intConstDatum = intDatum;

			Assert::AreEqual(intConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(intConstDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(intConstDatum.get<int>(0), 1);
			Assert::AreEqual(intConstDatum.get<int>(1), 2);
			Assert::AreEqual(intConstDatum.get<int>(2), 3);

			// Test get const for float
			Datum floatDatum;
			float* floatArray = new float[3] { 1.0f, 2.0f, 3.0f };
			floatDatum.setStorage(floatArray, 3);
			const Datum floatConstDatum = floatDatum;

			Assert::AreEqual(floatConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(floatConstDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(floatConstDatum.get<float>(0), 1.0f);
			Assert::AreEqual(floatConstDatum.get<float>(1), 2.0f);
			Assert::AreEqual(floatConstDatum.get<float>(2), 3.0f);

			// Test get const for string
			Datum stringDatum;
			std::string* stringArray = new std::string[3]{ "string1", "string2", "string3" };
			stringDatum.setStorage(stringArray, 3);
			const Datum stringConstDatum = stringDatum;

			Assert::AreEqual(stringConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(stringConstDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(stringConstDatum.get<std::string>(0), static_cast<std::string>("string1"));
			Assert::AreEqual(stringConstDatum.get<std::string>(1), static_cast<std::string>("string2"));
			Assert::AreEqual(stringConstDatum.get<std::string>(2), static_cast<std::string>("string3"));

			// Test get const for glm::vec4
			Datum vec4Datum;
			glm::vec4* vec4Array = new glm::vec4[3]
			{
				glm::vec4(1.0f, 2.0f, 3.0f, 4.0f),
				glm::vec4(5.0f, 6.0f, 7.0f, 8.0f),
				glm::vec4(9.0f, 10.0f, 11.0f, 12.0f)
			};
			vec4Datum.setStorage(vec4Array, 3);
			const Datum vec4ConstDatum = vec4Datum;

			Assert::AreEqual(vec4ConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(vec4ConstDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(vec4ConstDatum.get<glm::vec4>(0), vec4Array[0]);
			Assert::AreEqual(vec4ConstDatum.get<glm::vec4>(1), vec4Array[1]);
			Assert::AreEqual(vec4ConstDatum.get<glm::vec4>(2), vec4Array[2]);

			// Test get const for glm::mat4
			Datum mat4Datum;
			glm::mat4* mat4Array = new glm::mat4[3]
			{
				glm::mat4(1.0f),
				glm::mat4(2.0f),
				glm::mat4(3.0f)
			};
			mat4Datum.setStorage(mat4Array, 3);
			const Datum mat4ConstDatum = mat4Datum;

			Assert::AreEqual(mat4ConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(mat4ConstDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(mat4ConstDatum.get<glm::mat4>(0), mat4Array[0]);
			Assert::AreEqual(mat4ConstDatum.get<glm::mat4>(1), mat4Array[1]);
			Assert::AreEqual(mat4ConstDatum.get<glm::mat4>(2), mat4Array[2]);

			// Test get const for scope*
			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			Scope** scopeArray = new Scope * [3] { scope1, scope1, scope1 };
			scopeDatum.setStorage(scopeArray, 3);
			const Datum scopeConstDatum = scopeDatum;

			Assert::AreEqual(scopeConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(scopeConstDatum.getCapacity(), (size_t)3);
			Assert::AreEqual(scopeConstDatum.get<Scope*>(0), scope1);
			Assert::AreEqual(scopeConstDatum.get<Scope*>(1), scope1);
			Assert::AreEqual(scopeConstDatum.get<Scope*>(2), scope1);

			// Test get cons for pointer*
			Datum pointerDatum;
			Empty* e = new Empty();
			Empty** pointerArray = new Empty * [3] { e, e, e };
			pointerDatum.setStorage(pointerArray, 3);
			const Datum pointerConstDatum = pointerDatum;

			Assert::AreEqual(pointerConstDatum.getSize(), (size_t)3);
			Assert::AreEqual(pointerConstDatum.getCapacity(), (size_t)3);
			Assert::IsTrue(pointerConstDatum.get<Empty*>(0)->Equals(e));
			Assert::IsTrue(pointerConstDatum.get<Empty*>(1)->Equals(e));
			Assert::IsTrue(pointerConstDatum.get<Empty*>(2)->Equals(e));

			//Test invalid get const
			Assert::ExpectException<std::out_of_range>([&] { intConstDatum.get<int>(3); });
			Assert::ExpectException<std::invalid_argument>([&] { floatConstDatum.get<double>(0); });

			// Test type mismatch exception
			Assert::ExpectException<std::logic_error>([&] { intConstDatum.get<float>(0); });
			Assert::ExpectException<std::logic_error>([&] { floatConstDatum.get<std::string>(0); });
			Assert::ExpectException<std::logic_error>([&] { stringConstDatum.get<glm::vec4>(0); });
			Assert::ExpectException<std::logic_error>([&] { vec4ConstDatum.get<glm::mat4>(0); });
			Assert::ExpectException<std::logic_error>([&] { mat4ConstDatum.get<int>(0); });


			// delete the array
			delete[] intArray;
			delete[] floatArray;
			delete[] stringArray;
			delete[] vec4Array;
			delete[] mat4Array;
			delete[] scopeArray;
			delete e;
			delete[] pointerArray;
		}

		TEST_METHOD(TestDatumEqualityExternal)
		{
			Datum intDatum;
			int* intArray = new int[3] { 1, 2, 3 };
			intDatum.setStorage(intArray, 3);
			Datum intDatum2;
			int* intArray2 = new int[3] { 1, 2, 3 };
			intDatum2.setStorage(intArray2, 3);
			Datum intDatum3;
			int* intArray3 = new int[3] { 1, 2, 4 };
			intDatum3.setStorage(intArray3, 3);

			Datum floatDatum;
			float* floatArray = new float[3] { 1.0f, 2.0f, 3.0f };
			floatDatum.setStorage(floatArray, 3);
			Datum floatDatum2;
			float* floatArray2 = new float[3] { 1.0f, 2.0f, 3.0f };
			floatDatum2.setStorage(floatArray2, 3);
			Datum floatDatum3;
			float* floatArray3 = new float[3] { 1.0f, 2.0f, 4.0f };
			floatDatum3.setStorage(floatArray3, 3);

			Datum stringDatum;
			std::string* stringArray = new std::string[3]{ "string1", "string2", "string3" };
			stringDatum.setStorage(stringArray, 3);
			Datum stringDatum2;
			std::string* stringArray2 = new std::string[3]{ "string1", "string2", "string3" };
			stringDatum2.setStorage(stringArray2, 3);
			Datum stringDatum3;
			std::string* stringArray3 = new std::string[3]{ "string1", "string2", "string4" };
			stringDatum3.setStorage(stringArray3, 3);

			Datum vec4Datum;
			glm::vec4* vec4Array = new glm::vec4[3]
			{
				glm::vec4(1.0f, 2.0f, 3.0f, 4.0f),
				glm::vec4(5.0f, 6.0f, 7.0f, 8.0f),
				glm::vec4(9.0f, 10.0f, 11.0f, 12.0f)
			};
			vec4Datum.setStorage(vec4Array, 3);
			Datum vec4Datum2;
			glm::vec4* vec4Array2 = new glm::vec4[3]
			{
				glm::vec4(1.0f, 2.0f, 3.0f, 4.0f),
				glm::vec4(5.0f, 6.0f, 7.0f, 8.0f),
				glm::vec4(9.0f, 10.0f, 11.0f, 12.0f)
			};
			vec4Datum2.setStorage(vec4Array2, 3);
			Datum vec4Datum3;
			glm::vec4* vec4Array3 = new glm::vec4[3]
			{
				glm::vec4(2.0f, 3.0f, 4.0f, 5.0f),
				glm::vec4(5.0f, 6.0f, 7.0f, 8.0f),
				glm::vec4(9.0f, 10.0f, 11.0f, 12.0f)
			};
			vec4Datum3.setStorage(vec4Array3, 3);

			Datum mat4Datum;
			glm::mat4* mat4Array = new glm::mat4[3]
			{
				glm::mat4(1.0f),
				glm::mat4(2.0f),
				glm::mat4(3.0f)
			};
			mat4Datum.setStorage(mat4Array, 3);
			Datum mat4Datum2;
			glm::mat4* mat4Array2 = new glm::mat4[3]
			{
				glm::mat4(1.0f),
				glm::mat4(2.0f),
				glm::mat4(3.0f)
			};
			mat4Datum2.setStorage(mat4Array2, 3);
			Datum mat4Datum3;
			glm::mat4* mat4Array3 = new glm::mat4[3]
			{
				glm::mat4(1.0f),
				glm::mat4(2.0f),
				glm::mat4(4.0f)
			};
			mat4Datum3.setStorage(mat4Array3, 3);

			Datum scopeDatum;
			Scope* scope1 = (Scope*)0x1;
			Scope** scopeArray = new Scope * [3] { scope1, scope1, scope1 };
			scopeDatum.setStorage(scopeArray, 3);
			Datum scopeDatum2;
			Scope* scope2 = (Scope*)0x2;
			Scope** scopeArray2 = new Scope * [3] { scope1, scope1, scope1 };
			scopeDatum2.setStorage(scopeArray2, 3);
			Datum scopeDatum3;
			Scope* scope3 = (Scope*)0x3;
			Scope** scopeArray3 = new Scope * [3] { scope1, scope2, scope3 };

			Datum pointerDatum;
			Empty* e = new Empty();
			Empty** pointerArray = new Empty * [3] { e, e, e };
			pointerDatum.setStorage(pointerArray, 3);
			Datum pointerDatum2;
			Empty* e2 = new Empty();
			Empty** pointerArray2 = new Empty * [3] { e, e, e };
			pointerDatum2.setStorage(pointerArray2, 3);
			Datum pointerDatum3;
			Empty* e3 = new Empty();
			Empty** pointerArray3 = new Empty * [3] { e, e2, e3 };

			// Test equality
			Assert::IsTrue(intDatum == intDatum2);
			Assert::IsTrue(floatDatum == floatDatum2);
			Assert::IsTrue(stringDatum == stringDatum2);
			Assert::IsTrue(vec4Datum == vec4Datum2);
			Assert::IsTrue(mat4Datum == mat4Datum2);
			Assert::IsTrue(scopeDatum == scopeDatum2);
			Assert::IsTrue(pointerDatum == pointerDatum2);

			Assert::IsFalse(intDatum == intDatum3);
			Assert::IsFalse(floatDatum == floatDatum3);
			Assert::IsFalse(stringDatum == stringDatum3);
			Assert::IsFalse(vec4Datum == vec4Datum3);
			Assert::IsFalse(mat4Datum == mat4Datum3);
			Assert::IsFalse(scopeDatum == scopeDatum3);
			Assert::IsFalse(pointerDatum == pointerDatum3);

			// Test inequality
			Assert::IsTrue(intDatum != intDatum3);
			Assert::IsTrue(floatDatum != floatDatum3);
			Assert::IsTrue(stringDatum != stringDatum3);
			Assert::IsTrue(vec4Datum != vec4Datum3);
			Assert::IsTrue(mat4Datum != mat4Datum3);
			Assert::IsTrue(scopeDatum != scopeDatum3);
			Assert::IsTrue(pointerDatum != pointerDatum3);

			// Test different size
			Datum intDatum4;
			int* intArray4 = new int[4] { 1, 2, 4, 5 };
			intDatum4.setStorage(intArray4, 4);
			Assert::IsTrue(intDatum != intDatum4);


			// Test internal vs external
			Datum unknownDatumInternal;

			Datum intDatumInternal;
			Datum intDatumInternal2;
			Datum intDatumInternal3;
			intDatumInternal.push_back(1); intDatumInternal.push_back(2); intDatumInternal.push_back(3);
			intDatumInternal2.push_back(1); intDatumInternal2.push_back(2); intDatumInternal2.push_back(4);
			intDatumInternal3.push_back(1); 

			Datum floatDatumInternal;
			Datum floatDatumInternal2;
			Datum floatDatumInternal3;
			floatDatumInternal.push_back(1.0f); floatDatumInternal.push_back(2.0f); floatDatumInternal.push_back(3.0f);
			floatDatumInternal2.push_back(1.0f); floatDatumInternal2.push_back(2.0f); floatDatumInternal2.push_back(4.0f);
			floatDatumInternal3.push_back(1.0f);

			Datum stringDatumInternal;
			Datum stringDatumInternal2;
			Datum stringDatumInternal3;
			std::string s = "string1"; std::string s2 = "string2"; std::string s3 = "string3"; std::string s4 = "string4";
			stringDatumInternal.push_back(s); stringDatumInternal.push_back(s2); stringDatumInternal.push_back(s3);
			stringDatumInternal2.push_back(s); stringDatumInternal2.push_back(s2); stringDatumInternal2.push_back(s4);
			stringDatumInternal3.push_back(s);

			Datum vec4DatumInternal;
			Datum vec4DatumInternal2;
			Datum vec4DatumInternal3;
			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f); 
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f); 
			glm::vec4 v3 = glm::vec4(9.0f, 10.0f, 11.0f, 12.0f);
			glm::vec4 v4 = glm::vec4(2.0f, 3.0f, 4.0f, 5.0f);
			vec4DatumInternal.push_back(v); vec4DatumInternal.push_back(v2); vec4DatumInternal.push_back(v3);
			vec4DatumInternal2.push_back(v); vec4DatumInternal2.push_back(v2); vec4DatumInternal2.push_back(v4);
			vec4DatumInternal3.push_back(v);
			
			Datum mat4DatumInternal;
			Datum mat4DatumInternal2;
			Datum mat4DatumInternal3;
			glm::mat4 m = glm::mat4(1.0f);
			glm::mat4 m2 = glm::mat4(2.0f);
			glm::mat4 m3 = glm::mat4(3.0f);
			glm::mat4 m4 = glm::mat4(4.0f);
			mat4DatumInternal.push_back(m); mat4DatumInternal.push_back(m2); mat4DatumInternal.push_back(m3);
			mat4DatumInternal2.push_back(m); mat4DatumInternal2.push_back(m2); mat4DatumInternal2.push_back(m4);
			mat4DatumInternal3.push_back(m);

			Datum scopeDatumInternal;
			Datum scopeDatumInternal2;
			Datum scopeDatumInternal3;
			Scope* scope1Internal = (Scope*)0x1;
			Scope* scope2Internal = (Scope*)0x2;
			Scope* scope3Internal = (Scope*)0x3;
			scopeDatumInternal.push_back(scope1Internal); scopeDatumInternal.push_back(scope1Internal); scopeDatumInternal.push_back(scope1Internal);
			scopeDatumInternal2.push_back(scope1Internal); scopeDatumInternal2.push_back(scope2Internal); scopeDatumInternal2.push_back(scope3Internal);
			scopeDatumInternal3.push_back(scope1Internal);

			Datum pointerDatumInternal;
			Datum pointerDatumInternal2;
			Datum pointerDatumInternal3;
			Empty* eInternal = e;
			Empty* e2Internal = e2;
			Empty* e3Internal = e3;
			pointerDatumInternal.push_back(eInternal); pointerDatumInternal.push_back(eInternal); pointerDatumInternal.push_back(eInternal);
			pointerDatumInternal2.push_back(eInternal); pointerDatumInternal2.push_back(e2Internal); pointerDatumInternal2.push_back(e3Internal);
			pointerDatumInternal3.push_back(eInternal);

			// Test equality
			Assert::IsTrue(intDatum == intDatumInternal);
			Assert::IsTrue(floatDatum == floatDatumInternal);
			Assert::IsTrue(stringDatum == stringDatumInternal);
			Assert::IsTrue(vec4Datum == vec4DatumInternal);
			Assert::IsTrue(mat4Datum == mat4DatumInternal);
			Assert::IsTrue(scopeDatum == scopeDatumInternal);
			Assert::IsTrue(pointerDatum == pointerDatumInternal);
			Assert::IsTrue(intDatumInternal == intDatum);
			Assert::IsTrue(floatDatumInternal == floatDatum);
			Assert::IsTrue(stringDatumInternal == stringDatum);
			Assert::IsTrue(vec4DatumInternal == vec4Datum);
			Assert::IsTrue(mat4DatumInternal == mat4Datum);
			Assert::IsTrue(scopeDatumInternal == scopeDatum);
			Assert::IsTrue(pointerDatumInternal == pointerDatum);

			Assert::IsFalse(intDatum == intDatumInternal2);
			Assert::IsFalse(floatDatum == floatDatumInternal2);
			Assert::IsFalse(stringDatum == stringDatumInternal2);
			Assert::IsFalse(vec4Datum == vec4DatumInternal2);
			Assert::IsFalse(mat4Datum == mat4DatumInternal2);
			Assert::IsFalse(scopeDatum == scopeDatumInternal2);
			Assert::IsFalse(pointerDatum == pointerDatumInternal2);
			Assert::IsFalse(intDatumInternal2 == intDatum);
			Assert::IsFalse(floatDatumInternal2 == floatDatum);
			Assert::IsFalse(stringDatumInternal2 == stringDatum);
			Assert::IsFalse(vec4DatumInternal2 == vec4Datum);
			Assert::IsFalse(mat4DatumInternal2 == mat4Datum);
			Assert::IsFalse(scopeDatumInternal2 == scopeDatum);
			Assert::IsFalse(pointerDatumInternal2 == pointerDatum);

			Assert::Assert::IsFalse(intDatum == intDatumInternal3);
			Assert::Assert::IsFalse(floatDatum == floatDatumInternal3);
			Assert::Assert::IsFalse(stringDatum == stringDatumInternal3);
			Assert::Assert::IsFalse(vec4Datum == vec4DatumInternal3);
			Assert::Assert::IsFalse(mat4Datum == mat4DatumInternal3);
			Assert::Assert::IsFalse(scopeDatum == scopeDatumInternal3);
			Assert::Assert::IsFalse(pointerDatum == pointerDatumInternal3);
			Assert::Assert::IsFalse(intDatumInternal3 == intDatum);
			Assert::Assert::IsFalse(floatDatumInternal3 == floatDatum);
			Assert::Assert::IsFalse(stringDatumInternal3 == stringDatum);
			Assert::Assert::IsFalse(vec4DatumInternal3 == vec4Datum);
			Assert::Assert::IsFalse(mat4DatumInternal3 == mat4Datum);
			Assert::Assert::IsFalse(scopeDatumInternal3 == scopeDatum);
			Assert::Assert::IsFalse(pointerDatumInternal3 == pointerDatum);

			// Test inequality
			Assert::IsTrue(intDatum != intDatumInternal2);
			Assert::IsTrue(floatDatum != floatDatumInternal2);
			Assert::IsTrue(stringDatum != stringDatumInternal2);
			Assert::IsTrue(vec4Datum != vec4DatumInternal2);
			Assert::IsTrue(mat4Datum != mat4DatumInternal2);
			Assert::IsTrue(scopeDatum != scopeDatumInternal2);
			Assert::IsTrue(pointerDatum != pointerDatumInternal2);
			Assert::IsTrue(intDatumInternal2 != intDatum);
			Assert::IsTrue(floatDatumInternal2 != floatDatum);
			Assert::IsTrue(stringDatumInternal2 != stringDatum);
			Assert::IsTrue(vec4DatumInternal2 != vec4Datum);
			Assert::IsTrue(mat4DatumInternal2 != mat4Datum);
			Assert::IsTrue(scopeDatumInternal2 != scopeDatum);
			Assert::IsTrue(pointerDatumInternal2 != pointerDatum);

			// Delete the array
			delete[] intArray;
			delete[] intArray2;
			delete[] intArray3;
			delete[] intArray4;
			delete[] floatArray;
			delete[] floatArray2;
			delete[] floatArray3;
			delete[] stringArray;
			delete[] stringArray2;
			delete[] stringArray3;
			delete[] vec4Array;
			delete[] vec4Array2;
			delete[] vec4Array3;
			delete[] mat4Array;
			delete[] mat4Array2;
			delete[] mat4Array3;
			delete[] scopeArray;
			delete[] scopeArray2;
			delete[] scopeArray3;
			delete[] pointerArray;
			delete[] pointerArray2;
			delete[] pointerArray3;
			delete e;
			delete e2;
			delete e3;
		}

		TEST_METHOD(TestDatumEqualityScalarExternal)
		{
			int i = 10;
			int i2 = 20;
			int* intArray = new int[1] { 10 };
			Datum intDatum;
			intDatum.setStorage(intArray, 1);

			float f = 10.0f;
			float f2 = 20.0f;
			float* floatArray = new float[1] { 10.0f };
			Datum floatDatum;
			floatDatum.setStorage(floatArray, 1);

			std::string s = "string";
			std::string s2 = "string2";
			std::string* stringArray = new std::string[1]{ "string" };
			Datum stringDatum;
			stringDatum.setStorage(stringArray, 1);

			glm::vec4 v = glm::vec4(1.0f, 2.0f, 3.0f, 4.0f);
			glm::vec4 v2 = glm::vec4(5.0f, 6.0f, 7.0f, 8.0f);
			glm::vec4* vec4Array = new glm::vec4[1]{ v };
			Datum vec4Datum;
			vec4Datum.setStorage(vec4Array, 1);

			glm::mat4 m = glm::mat4(1.0f);
			glm::mat4 m2 = glm::mat4(2.0f);
			glm::mat4* mat4Array = new glm::mat4[1]{ m };
			Datum mat4Datum;
			mat4Datum.setStorage(mat4Array, 1);

			Scope* scope1 = (Scope*)0x1;
			Scope* scope2 = (Scope*)0x2;
			Scope** scopeArray = new Scope * [1] { scope1 };
			Datum scopeDatum;
			scopeDatum.setStorage(scopeArray, 1);

			Empty* e = new Empty();
			Empty* e2 = new Empty();
			Empty** pointerArray = new Empty * [1] { e };
			Datum pointerDatum;
			pointerDatum.setStorage(pointerArray, 1);

			// Test equality
			Assert::IsTrue(intDatum == i);
			Assert::IsTrue(floatDatum == f);
			Assert::IsTrue(stringDatum == s);
			Assert::IsTrue(vec4Datum == v);
			Assert::IsTrue(mat4Datum == m);
			Assert::IsTrue(scopeDatum == scope1);
			Assert::IsTrue(pointerDatum == e);

			Assert::IsFalse(intDatum == i2);
			Assert::IsFalse(intDatum == f);
			Assert::IsFalse(floatDatum == f2);
			Assert::IsFalse(floatDatum == s);
			Assert::IsFalse(stringDatum == s2);
			Assert::IsFalse(stringDatum == v);
			Assert::IsFalse(vec4Datum == v2);
			Assert::IsFalse(vec4Datum == m);
			Assert::IsFalse(mat4Datum == m2);
			Assert::IsFalse(mat4Datum == i);
			Assert::IsFalse(scopeDatum == scope2);
			Assert::IsFalse(scopeDatum == e2);
			Assert::IsFalse(pointerDatum == e2);

			// Test inequality
			Assert::IsTrue(intDatum != i2);
			Assert::IsTrue(floatDatum != f2);
			Assert::IsTrue(stringDatum != s2);
			Assert::IsTrue(vec4Datum != v2);
			Assert::IsTrue(mat4Datum != m2);
			Assert::IsTrue(scopeDatum != scope2);
			Assert::IsTrue(pointerDatum != e2);

			// Test other
			Assert::IsFalse(intDatum == "char_arr");

			int* intArray2 = new int[3] { 1, 2, 3 };
			Datum intDatum2;
			intDatum2.setStorage(intArray2, 3);
			Assert::IsFalse(intDatum2 == 1);

			// Delete the array
			delete[] intArray;
			delete[] intArray2;
			delete[] floatArray;
			delete[] stringArray;
			delete[] vec4Array;
			delete[] mat4Array;
			delete[] scopeArray;
			delete e;
			delete e2;
			delete[] pointerArray;
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
