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
	inline std::wstring ToString<Scope>(const Scope* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum& t)
	{
		RETURN_WIDE_STRING(L"Datum Ref");
	}

	template<>
	inline std::wstring ToString<Datum>(const Datum* t)
	{
		RETURN_WIDE_STRING(L"Datum Ptr");
	}

	template<>
	inline std::wstring ToString<Datum>(Datum* t)
	{
		RETURN_WIDE_STRING(L"Datum Ptr");
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
	TEST_CLASS(FieaGameEngineScopeTest)
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

		TEST_METHOD(TestScopeConstruct)
		{
			// Defulat constructor
			Scope scope;
			Assert::AreEqual(scope.getSize(), (size_t)0);
			Assert::AreEqual(scope.getCapacity(), (size_t)0);

			// Size constructor
			Scope sizeScope(10);
			Assert::AreEqual(sizeScope.getSize(), (size_t)0);
			Assert::AreEqual(sizeScope.getCapacity(), (size_t)10);

			// Copy constructor
			Scope copyScope(sizeScope);
			Assert::AreEqual(copyScope.getSize(), (size_t)0);
			Assert::AreEqual(copyScope.getCapacity(), (size_t)10);

			// Move constructor
			scope.append("a");
			Scope moveScope(std::move(scope));
			Assert::AreEqual(moveScope.getSize(), (size_t)1);
			Assert::AreEqual(moveScope.getCapacity(), (size_t)1);

			// Copy assignment operator
			Scope assignmentScope;
			assignmentScope = sizeScope;
			Assert::AreEqual(assignmentScope.getSize(), (size_t)0);
			Assert::AreEqual(assignmentScope.getCapacity(), (size_t)10);
			
			// Destructor test starts here
			Scope* scopePtr = new Scope();
			
			scopePtr->append("a");
			scopePtr->append("b");
			scopePtr->find("a")->push_back(10);
			scopePtr->find("b")->push_back(20);
			Assert::AreEqual(scopePtr->getSize(), (size_t)2);
			Assert::AreEqual(scopePtr->getCapacity(), (size_t)2);
			Assert::IsTrue(scopePtr->getParent() == nullptr);
			Assert::AreEqual(scopePtr->find("a")->get<int32_t>(0), 10);
			Assert::AreEqual(scopePtr->find("b")->get<int32_t>(0), 20);

			copyScope.append("ac");
			copyScope.append("bc");
			copyScope.find("ac")->push_back(0.69f);
			copyScope.find("bc")->push_back(0.96f);
			Assert::AreEqual(copyScope.getSize(), (size_t)2);
			Assert::AreEqual(copyScope.getCapacity(), (size_t)10);
			Assert::IsTrue(copyScope.getParent() == nullptr);
			Assert::AreEqual(copyScope.find("ac")->get<float>(0), 0.69f);
			Assert::AreEqual(copyScope.find("bc")->get<float>(0), 0.96f);

			// Test destructing nested scope 
			scopePtr->adopt(copyScope, "copy");
			Assert::AreEqual(copyScope.getParent(), scopePtr);

			delete scopePtr;
		}	

		TEST_METHOD(TestScopeAssignment)
		{
			// Create a scope
			Scope scope;
			scope.append("a");
			scope.append("b");

			std::string s = "Hello";
			scope.find("a")->push_back(s);
			scope.find("b")->push_back(10);
			scope.find("b")->push_back(20);

			// And assign it to another scope
			Scope assignmentScope;
			assignmentScope = scope;

			Assert::AreEqual(assignmentScope.getSize(), (size_t)2);
			Assert::AreEqual(assignmentScope.getCapacity(), (size_t)2);
			Assert::IsTrue(assignmentScope.getParent() == nullptr);
			Assert::AreEqual(assignmentScope.find("a")->get<std::string>(0), s);
			Assert::AreEqual(assignmentScope.find("b")->get<int>(0), 10);
			Assert::AreEqual(assignmentScope.find("b")->get<int>(1), 20);

			// move assignment
			Scope moveScope;
			moveScope = std::move(assignmentScope);
			Assert::AreEqual(moveScope.getSize(), (size_t)2);
			Assert::AreEqual(moveScope.getCapacity(), (size_t)2);
			Assert::IsTrue(moveScope.getParent() == nullptr);
			Assert::AreEqual(moveScope.find("a")->get<std::string>(0), s);
			Assert::AreEqual(moveScope.find("b")->get<int>(0), 10);
			Assert::AreEqual(moveScope.find("b")->get<int>(1), 20);

		}

		TEST_METHOD(TestScopeBracket)
		{
			Scope scope;
			scope.append("a");
			scope.append("b");
			scope.find("a")->push_back(0.69f);
			scope.find("b")->push_back(0.96f);

			Assert::AreEqual(scope.getSize(), (size_t)2);
			Assert::AreEqual(scope.getCapacity(), (size_t)2);
			Assert::IsTrue(scope.getParent() == nullptr);

			// Test bracket operator for index
			Datum& datumIndexA = scope[0];
			Datum& datumIndexB = scope[1];
			Assert::AreEqual(datumIndexA, *scope.find("a"));
			Assert::AreEqual(datumIndexB, *scope.find("b"));
			
			// Test bracket operator for append wraping
			Datum& datumA = scope["a"];
			Datum& datumB = scope["b"];
			Datum& datumC = scope["c"];
			Assert::AreEqual(datumA, *scope.find("a"));
			Assert::AreEqual(datumB, *scope.find("b"));
			Assert::AreEqual(datumC, *scope.find("c"));
			Assert::AreEqual(datumA.get<float>(0), 0.69f);
			Assert::AreEqual(datumB.get<float>(0), 0.96f);
			Assert::AreEqual(datumC.getDataType(), DataType::UNKNOWN);

			// Test the index order after copy
			Scope copyScope = scope;
			Datum& copyDatumIndexA = copyScope[0];
			Datum& copyDatumIndexB = copyScope[1];
			Datum& copyDatumIndexC = copyScope[2];
			Assert::AreEqual(copyDatumIndexA, *copyScope.find("a"));
			Assert::AreEqual(copyDatumIndexB, *copyScope.find("b"));
			Assert::AreEqual(copyDatumIndexC, *copyScope.find("c"));

			// Test out of range exception
			Assert::ExpectException<std::out_of_range>([&] { scope[3]; });
			Assert::ExpectException<std::out_of_range>([&] { scope[99]; });
		}

		TEST_METHOD(TestScopeEqual)
		{
			Scope scope;
			scope.append("a");
			scope.append("b");
			glm::vec4 v1(1, 2, 3, 4);
			glm::vec4 v2(5, 6, 7, 8);
			scope.find("a")->push_back(v1);
			scope.find("b")->push_back(v2);

			Scope scope2;
			scope2.append("a");
			scope2.append("b");
			scope2.find("a")->push_back(v1);
			scope2.find("b")->push_back(v2);

			Scope scope3;
			scope3.append("a");
			scope3.find("a")->push_back(v1);

			Scope scope4;
			scope4.append("a");
			scope4.append("b");
			scope4.find("a")->push_back(10);
			scope4.find("b")->push_back(20);

			Scope scope5;
			scope5.append("a");
			scope5.append("b");
			glm::vec4 v3(1, 2, 3, 5);
			glm::vec4 v4(5, 6, 7, 8);
			scope5.find("a")->push_back(v3);
			scope5.find("b")->push_back(v4);

			Assert::IsTrue(scope == scope2);
			Assert::IsFalse(scope != scope2);
			Assert::IsFalse(scope == scope3);
			Assert::IsTrue(scope != scope3);
			Assert::IsFalse(scope == scope4);
			Assert::IsTrue(scope != scope4);
			Assert::IsFalse(scope == scope5);

			// Test scope
			Scope* scopePtr = &scope;
			Scope* scopePtr2 = &scope3;

			Scope scope6;
			scope6.appendScope("child");
			scope6.find("child")->push_back(scopePtr);

			Scope scope7;
			scope7.appendScope("child");
			scope7.find("child")->push_back(scopePtr);

			Scope scope8;
			scope8.appendScope("child");
			scope8.find("child")->push_back(scopePtr2);

			Scope scope9;
			scope9.appendScope("child1");
			scope9.appendScope("child2");

			Scope scope10;
			scope10.appendScope("child1");
			scope10.find("child1")->push_back(scopePtr);
			scope10.find("child1")->push_back(scopePtr2);

			Assert::IsTrue(scope6 == scope7);
			Assert::IsFalse(scope6 != scope7);
			Assert::IsFalse(scope6 == scope8);
			Assert::IsTrue(scope6 != scope8);
			Assert::IsFalse(scope6 == scope9);
			Assert::IsFalse(scope6 == scope10);

			// Test Scope.Equals()
			Assert::IsTrue(scope6.Equals(&scope6));
			Assert::IsTrue(scope6.Equals(&scope7));
			Assert::IsFalse(scope6.Equals(&scope8));
			Assert::IsFalse(scope6.Equals(&scope9));
			Assert::IsFalse(scope6.Equals(nullptr));

			Foo foo(10);
			Assert::IsFalse(scope6.Equals(&foo));

		}

		TEST_METHOD(TestScopeFind)
		{
			// Test find without const
			Scope scope;
			scope.append("a");
			scope.append("b");
			glm::vec4 v1(1, 2, 3, 4);
			glm::vec4 v2(5, 6, 7, 8);
			scope.find("a")->push_back(v1);
			scope.find("b")->push_back(v2);

			Assert::AreEqual(scope.find("a")->get<glm::vec4>(0), v1);
			Assert::AreEqual(scope.find("b")->get<glm::vec4>(0), v2);
			Assert::IsTrue(scope.find("c") == nullptr);

			// Test find with const
			const Scope constScope = scope;
			Assert::AreEqual(constScope.find("a")->get<glm::vec4>(0), v1);
			Assert::AreEqual(constScope.find("b")->get<glm::vec4>(0), v2);
			Assert::IsTrue(constScope.find("c") == nullptr);

			// Test findContainedScope
			Scope* scopePtr = new Scope();
			scopePtr->append("a");
			scopePtr->append("b");
			scopePtr->find("a")->push_back(10);
			scopePtr->find("b")->push_back(20);
			Assert::AreEqual(scopePtr->getSize(), (size_t)2);
			Assert::AreEqual(scopePtr->getCapacity(), (size_t)2);
			Assert::IsTrue(scopePtr->getParent() == nullptr);
			Assert::AreEqual(scopePtr->find("a")->get<int32_t>(0), 10);
			Assert::AreEqual(scopePtr->find("b")->get<int32_t>(0), 20);

			// Nested scope
			Scope* nestedScope = new Scope();
			nestedScope->append("c");
			nestedScope->find("c")->push_back(30);

			scopePtr->adopt(*nestedScope, "nested");
			Assert::AreEqual(scopePtr->getSize(), (size_t)3);
			Assert::AreEqual(scopePtr->getCapacity(), (size_t)3);
			Assert::AreEqual(nestedScope->getParent(), scopePtr);

			size_t index = 0;
			Datum* datumContainingNestedScope = scopePtr->findContainedScope(nestedScope, index);
			Assert::AreEqual(datumContainingNestedScope, scopePtr->find("nested"));
			Assert::AreEqual(index, (size_t)2);

			datumContainingNestedScope = scopePtr->findContainedScope(scopePtr, index);
			Assert::IsTrue(datumContainingNestedScope == nullptr);

			// Delete pointers
			delete scopePtr;
			delete nestedScope;
		}

		TEST_METHOD(TestScopeSearch)
		{
			// Create a root scope
			Scope scope;
			scope.append("a");
			scope.append("b");
			std::string s = "Hello";
			std::string s2 = "World";
			scope.find("a")->push_back(s);
			scope.find("b")->push_back(s2);

			// Test search in root scope
			Datum* foundDatumA = scope.search("a");
			Datum* foundDatumB = scope.search("b");
			Assert::AreEqual(foundDatumA, scope.find("a"));
			Assert::AreEqual(foundDatumB, scope.find("b"));
			Assert::AreEqual(foundDatumA->get<std::string>(0), s);
			Assert::AreEqual(foundDatumB->get<std::string>(0), s2);

			// Create a nested scope
			Scope childScope;
			childScope.append("c");
			childScope.append("d");
			std::string s3 = "Hola";
			std::string s4 = "Mundo";
			childScope.find("c")->push_back(s3);
			childScope.find("d")->push_back(s4);

			// Adopt the nested scope
			scope.adopt(childScope, "child");

			// Test search in nested scope
			Scope* foundScope = nullptr;
			Scope** foundScopePtr = &foundScope;
			Datum* foundDatumC = childScope.search("c");
			Datum* foundDatumD = childScope.search("d");
			foundDatumA = childScope.search("a");
			foundDatumB = childScope.search("b", foundScopePtr);

			Assert::AreEqual(*foundScopePtr, &scope);

			Assert::AreEqual(scope.getSize(), (size_t)3);
			Assert::AreEqual(scope.getCapacity(), (size_t)3);
			Assert::AreEqual(childScope.getParent(), &scope);
			Assert::AreEqual(foundDatumC, childScope.find("c"));
			Assert::AreEqual(foundDatumD, childScope.find("d"));
			Assert::AreEqual(foundDatumA, scope.find("a"));
			Assert::AreEqual(foundDatumB, scope.find("b"));

			Assert::AreEqual(foundDatumC->get<std::string>(0), s3);
			Assert::AreEqual(foundDatumD->get<std::string>(0), s4);
			Assert::AreEqual(foundDatumA->get<std::string>(0), s);
			Assert::AreEqual(foundDatumB->get<std::string>(0), s2);

			// Test search in const scope
			const Scope constScope = scope;
			const Datum* constFoundDatumA = constScope.search("a");
			const Datum* constFoundDatumB = constScope.search("b");

			Assert::AreEqual(constScope.getSize(), (size_t)3);
			Assert::AreEqual(constScope.getCapacity(), (size_t)3);
			Assert::AreEqual(constFoundDatumA, constScope.find("a"));
			Assert::AreEqual(constFoundDatumB, constScope.find("b"));
			Assert::AreEqual(constFoundDatumA->get<std::string>(0), s);
			Assert::AreEqual(constFoundDatumB->get<std::string>(0), s2);

			// Test search in const nested scope
			const Scope* constFoundScope = nullptr;
			const Scope** constFoundScopePtr = &constFoundScope;
			const Scope* constChildScope = constScope.find("child")->get<Scope*>(0);
			const Datum* constFoundDatumC = constChildScope->search("c");
			const Datum* constFoundDatumD = constChildScope->search("d");
			constFoundDatumA = constChildScope->search("a");
			constFoundDatumB = constChildScope->search("b", constFoundScopePtr);

			Assert::AreEqual(*constFoundScopePtr, &constScope);

			Assert::AreEqual(constChildScope->getSize(), (size_t)2);
			Assert::AreEqual(constChildScope->getCapacity(), (size_t)2);
			Assert::AreEqual(constFoundDatumC, constChildScope->find("c"));
			Assert::AreEqual(constFoundDatumD, constChildScope->find("d"));
			Assert::AreEqual(constFoundDatumA, constScope.find("a"));
			Assert::AreEqual(constFoundDatumB, constScope.find("b"));

			Assert::AreEqual(constFoundDatumC->get<std::string>(0), s3);
			Assert::AreEqual(constFoundDatumD->get<std::string>(0), s4);
			Assert::AreEqual(constFoundDatumA->get<std::string>(0), s);
			Assert::AreEqual(constFoundDatumB->get<std::string>(0), s2);
		}

		TEST_METHOD(TestScopeTree)
		{
			Scope rootScope;
			rootScope.append("a");
			rootScope.append("b");
			glm::vec4 v1(1, 2, 3, 4);
			glm::vec4 v2(5, 6, 7, 8);
			rootScope.find("a")->push_back(v1);
			rootScope.find("b")->push_back(v2);

			Scope childScope;
			childScope.append("c");
			std::string s3 = "Hola";
			std::string s4 = "Mundo";
			childScope.find("c")->push_back(s3);
			childScope.find("c")->push_back(s3);

			Scope grandChildScope;
			grandChildScope.append("d");
			grandChildScope.find("d")->push_back(10);
			grandChildScope.find("d")->push_back(20);
			grandChildScope.find("d")->push_back(30);

			// Test inheritace
			rootScope.appendScope("child1");
			rootScope.adopt(childScope, "child1");
			childScope.appendScope("grandchild1");
			childScope.appendScope("grandchild2");
			childScope.append("grandchild3");
			childScope.appendScope("grandchild3");
			childScope.adopt(grandChildScope, "grandchild4");

			Assert::AreEqual(rootScope.getSize(), (size_t)3);
			Assert::AreEqual(rootScope.getCapacity(), (size_t)3);
			Assert::AreEqual(childScope.getSize(), (size_t)5);
			Assert::AreEqual(childScope.getCapacity(), (size_t)5);
			Assert::AreEqual(grandChildScope.getSize(), (size_t)1);
			Assert::AreEqual(grandChildScope.getCapacity(), (size_t)1);

			Assert::AreEqual(rootScope.find("child1")->get<Scope*>(1), &childScope);
			Assert::AreEqual(childScope.find("grandchild4")->get<Scope*>(0), &grandChildScope);
			
			// Test exception for invalid adopt
			Assert::ExpectException<std::invalid_argument>([&] { rootScope.adopt(rootScope, "child1"); });
			Assert::ExpectException<std::invalid_argument>([&] { childScope.adopt(childScope, "child1"); });
			Assert::ExpectException<std::invalid_argument>([&] { childScope.adopt(rootScope, "child1"); });
			Assert::ExpectException<std::invalid_argument>([&] { grandChildScope.adopt(childScope, "child1"); });
			Assert::ExpectException<std::invalid_argument>([&] { grandChildScope.adopt(rootScope, "child1"); });
			
			rootScope.append("child2");
			rootScope.find("child2")->push_back(10);
			Assert::ExpectException<std::invalid_argument>([&] { rootScope.adopt(childScope, "child2"); });

			// Test root adopt grandchild
			rootScope.adopt(grandChildScope, "child1");
			Assert::AreEqual(rootScope.getSize(), (size_t)4);
			Assert::AreEqual(rootScope.getCapacity(), (size_t)4);
			Assert::AreEqual(rootScope.find("child1")->get<Scope*>(2), &grandChildScope);

			// Test exception for invalid appendScope
			childScope.append("grandchild5");
			childScope.find("grandchild5")->push_back(10);
			Assert::ExpectException<std::invalid_argument>([&] { childScope.appendScope("grandchild5"); });
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}