#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "TestParseHandler.h"
#include "TestParseIntHandler.h"
#include "IntegerHandler.h"
#include "FloatHandler.h"
#include "StringHandler.h"
#include "Vector4Handler.h"
#include "Matrix4Handler.h"
#include "TableHandler.h"
#include "TableWrapper.h"
#include <sstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::shared_ptr; 
using std::weak_ptr; 
using std::unique_ptr;

typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Datum::DataType DataType;
typedef Fiea::GameEngine::Scope Scope;
typedef Fiea::GameEngine::TableWrapper TableWrppaer;
typedef Fiea::GameEngine::ParseCoordinator ParseCoordinator;
typedef Fiea::GameEngine::IParseHandler IParseHandler;
typedef Fiea::GameEngine::test::TestParseHandler TestParseHandler;
typedef Fiea::GameEngine::TestParseIntHandler TestParseIntHandler;
typedef Fiea::GameEngine::IntegerHandler IntegerHandler;
typedef Fiea::GameEngine::FloatHandler FloatHandler;
typedef Fiea::GameEngine::StringHandler StringHandler;
typedef Fiea::GameEngine::Vector4Handler Vector4Handler;
typedef Fiea::GameEngine::Matrix4Handler Matrix4Handler;
typedef Fiea::GameEngine::TableHandler TableHandler;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
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
	TEST_CLASS(FieaGameEngineParserTest)
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

		TEST_METHOD(TestHandler)
		{
			std::string nested = R"({
				"int":1,
				"obj":{
					"obj":{
						"int":3,
						"str":"def"
					},
					"int":1,
					"str":"abc"
				},
				"float":2.0
			})";

			TestParseHandler::Wrapper wrapper;
			ParseCoordinator parser(wrapper);
			Assert::IsTrue(parser.getWrapper()->Equals(&wrapper));

			auto handlerRef = parser.addHandler(std::make_unique<TestParseHandler>());
			bool deserialized = parser.deserializeObject(nested);
			Assert::IsTrue(deserialized);

			auto handler = parser.getHandler<TestParseHandler>(handlerRef);
			Assert::IsTrue(handler->isInitialized());
			Assert::AreEqual(static_cast<size_t>(8), handler->startCount);
			Assert::AreEqual(static_cast<size_t>(8), handler->endCount);
			Assert::AreEqual(static_cast<size_t>(2), wrapper.maxDepth);

			std::string wrong = "wrong";
			Assert::ExpectException<std::runtime_error>([&] { parser.deserializeObject(wrong); });
		}

		TEST_METHOD(TestIntHandler)
		{
			std::string nested = R"({
				"int":1
			})";

			int integer = 0;
			TestParseIntHandler::Wrapper wrapper(&integer);
			ParseCoordinator parser(wrapper);
			Assert::IsTrue(parser.getWrapper()->Equals(&wrapper));

			auto handlerRef = parser.addHandler(std::make_unique<TestParseIntHandler>());
			bool deserialized = parser.deserializeObject(nested);
			Assert::IsTrue(deserialized);

			auto handler = parser.getHandler<TestParseIntHandler>(handlerRef);
			Assert::IsTrue(handler->isInitialized());
			Assert::AreEqual(static_cast<size_t>(1), handler->count);
			Assert::AreEqual(1, integer);

			std::string tooMany = R"({
				"int":1,
				"int":2
			})";
			deserialized = parser.deserializeObject(tooMany);
			Assert::IsFalse(deserialized);

			std::string wrong = "wrong";
			Assert::ExpectException<std::runtime_error>([&] { parser.deserializeObject(wrong); });
		}

		TEST_METHOD(TestTableHandler)
		{
			std::string nested = R"-({
				"int_1":1,
				"float_1":1.0,
				"str_1":"abc",
				"vec4_1": "vec4(1.0, 2.0, 3.0, 4.0)",
				"mat4_1": "mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))",
				"obj_1":{
					"obj_1":{
						"int_1":3
					},
					"obj_2":{
						"float_1":3,
						"vec4_1": "vec4(1.0, 2.0, 3.0, 4.0)"
					},
					"int_1":2,
					"str_1":"def"
				},
				"int_array_1": [1, 2, 3],
				"float_array_1": [1.0, 2.0, 3.0],
				"str_array_1": ["abc", "def", "ghi"],
				"vec4_array_1": ["vec4(1.0, 2.0, 3.0, 4.0)", "vec4(5.0, 6.0, 7.0, 8.0)", "vec4(9.0, 10.0, 11.0, 12.0)"],
				"mat4_array_1": ["mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))", "mat4x4((2.0, 0.0, 0.0, 0.0), (0.0, 2.0, 0.0, 0.0), (0.0, 0.0, 2.0, 0.0), (0.0, 0.0, 0.0, 2.0))"]
			})-";

			Scope* table = new Scope();
			TableWrppaer wrapper(table);
			ParseCoordinator parser(wrapper);
			Assert::IsTrue(parser.getWrapper()->Equals(&wrapper));
			Assert::AreEqual(table, wrapper.getRootTable());

			// add handlers for parsing different types
			auto handlerRefInt = parser.addHandler(std::make_unique<IntegerHandler>());
			auto handlerRefFloat = parser.addHandler(std::make_unique<FloatHandler>());
			auto handlerRefString = parser.addHandler(std::make_unique<StringHandler>());
			auto handlerRefVec4 = parser.addHandler(std::make_unique<Vector4Handler>());
			auto handlerRefMat4 = parser.addHandler(std::make_unique<Matrix4Handler>());
			auto handlerRefTable = parser.addHandler(std::make_unique<TableHandler>());

			// deserialize the object from JSON to the table
			bool deserialized = parser.deserializeObject(nested);
			Assert::IsTrue(deserialized);

			// check the data in the root table
			Assert::AreEqual(1, table->find("int_1")->get<int>(0));
			Assert::AreEqual(1.0f, table->find("float_1")->get<float>(0));
			Assert::AreEqual(std::string("abc"), table->find("str_1")->get<std::string>(0));
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), table->find("vec4_1")->get<glm::vec4>(0));
			Assert::AreEqual(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), table->find("mat4_1")->get<glm::mat4>(0));

			// check int array data in the root table
			Assert::AreEqual(1, table->find("int_array_1")->get<int>(0));
			Assert::AreEqual(2, table->find("int_array_1")->get<int>(1));
			Assert::AreEqual(3, table->find("int_array_1")->get<int>(2));

			// check float array data in the root table
			Assert::AreEqual(1.0f, table->find("float_array_1")->get<float>(0));
			Assert::AreEqual(2.0f, table->find("float_array_1")->get<float>(1));
			Assert::AreEqual(3.0f, table->find("float_array_1")->get<float>(2));

			// check string array data in the root table
			Assert::AreEqual(std::string("abc"), table->find("str_array_1")->get<std::string>(0));
			Assert::AreEqual(std::string("def"), table->find("str_array_1")->get<std::string>(1));
			Assert::AreEqual(std::string("ghi"), table->find("str_array_1")->get<std::string>(2));

			// check vec4 array data in the root table
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), table->find("vec4_array_1")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f), table->find("vec4_array_1")->get<glm::vec4>(1));
			Assert::AreEqual(glm::vec4(9.0f, 10.0f, 11.0f, 12.0f), table->find("vec4_array_1")->get<glm::vec4>(2));

			// check mat4 array data in the root table
			Assert::AreEqual(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), table->find("mat4_array_1")->get<glm::mat4>(0));
			Assert::AreEqual(glm::mat4(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f), table->find("mat4_array_1")->get<glm::mat4>(1));

			// check the data in the first level nested table
			auto obj1 = table->find("obj_1")->get<Scope*>(0);
			Assert::AreEqual(2, obj1->find("int_1")->get<int>(0));
			Assert::AreEqual(std::string("def"), obj1->find("str_1")->get<std::string>(0));

			// check the data in the second level nested table
			auto obj2_1 = obj1->find("obj_1")->get<Scope*>(0);
			Assert::AreEqual(3, obj2_1->find("int_1")->get<int>(0));
			auto obj2_2 = obj1->find("obj_2")->get<Scope*>(0);
			Assert::AreEqual(3.0f, obj2_2->find("float_1")->get<float>(0));
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), obj2_2->find("vec4_1")->get<glm::vec4>(0));

			// get the handlers from the parser and check if they are initialized
			auto handlerInt = parser.getHandler<IntegerHandler>(handlerRefInt);
			auto handlerFloat = parser.getHandler<FloatHandler>(handlerRefFloat);
			auto handlerString = parser.getHandler<StringHandler>(handlerRefString);
			auto handlerVec4 = parser.getHandler<Vector4Handler>(handlerRefVec4);
			auto handlerMat4 = parser.getHandler<Matrix4Handler>(handlerRefMat4);
			auto handlerTable = parser.getHandler<TableHandler>(handlerRefTable);
			Assert::IsTrue(handlerInt->isInitialized());
			Assert::IsTrue(handlerFloat->isInitialized());
			Assert::IsTrue(handlerString->isInitialized());
			Assert::IsTrue(handlerVec4->isInitialized());
			Assert::IsTrue(handlerMat4->isInitialized());
			Assert::IsTrue(handlerTable->isInitialized());

			// test unknow key-value pair exception
			std::string nestedWrong = R"-({
				"obj_1":{
					"obj_1":{
						"int_1":3
					},
					"obj_2":{
						"Imposter":"HAHA"
					}
				}
			})-";
			bool deserializedWrong = parser.deserializeObject(nestedWrong);
			Assert::IsFalse(deserializedWrong);

			// test wroin vec4 and mat4 format
			std::string nestedWrongVec4 = R"-({
				"vec4_1": "vec4(1.0, 2.0, 3.0)"
			})-";
			Assert::ExpectException<std::invalid_argument>([&] { parser.deserializeObject(nestedWrongVec4); });

			std::string nestedWrongMat4 = R"-({
				"mat4_1": "69"
			})-";
			Assert::ExpectException<std::invalid_argument>([&] { parser.deserializeObject(nestedWrongMat4); });

			// remove all handlers
			parser.removeHandler(handlerRefInt);
			parser.removeHandler(handlerRefFloat);
			parser.removeHandler(handlerRefString);
			parser.removeHandler(handlerRefVec4);
			parser.removeHandler(handlerRefMat4);
			parser.removeHandler(handlerRefTable);

			// delete memory
			delete table;
		}

		TEST_METHOD(TestParserReadStream)
		{
			// Test for test table
			std::string nested = R"({
				"int":1,
				"obj":{
					"obj":{
						"int":3,
						"str":"def"
					},
					"int":1,
					"str":"abc"
				},
				"float":2.0
			})";

			std::istringstream iss(nested);
			TestParseHandler::Wrapper wrapper;
			ParseCoordinator parser(wrapper);

			auto handlerRef = parser.addHandler(std::make_unique<TestParseHandler>());
			bool deserialized = parser.deserializeObject(iss);
			Assert::IsTrue(deserialized);

			auto handler = parser.getHandler<TestParseHandler>(handlerRef);
			Assert::IsTrue(handler->isInitialized());
			Assert::AreEqual(static_cast<size_t>(8), handler->startCount);
			Assert::AreEqual(static_cast<size_t>(8), handler->endCount);
			Assert::AreEqual(static_cast<size_t>(2), wrapper.maxDepth);

			std::string wrong = "wrong";
			std::istringstream issWrong(wrong);
			Assert::ExpectException<std::runtime_error>([&] { parser.deserializeObject(issWrong); });
		}

		TEST_METHOD(TestParserReadFile)
		{
			// Test read from test file
			std::string file = "test_json.json";

			TestParseHandler::Wrapper wrapper;
			ParseCoordinator parser(wrapper);

			auto handlerRef = parser.addHandler(std::make_unique<TestParseHandler>());
			bool deserialized = parser.deserializeObjectFromFile(file);
			Assert::IsTrue(deserialized);

			auto handler = parser.getHandler<TestParseHandler>(handlerRef);
			Assert::IsTrue(handler->isInitialized());
			Assert::AreEqual(static_cast<size_t>(8), handler->startCount);
			Assert::AreEqual(static_cast<size_t>(8), handler->endCount);
			Assert::AreEqual(static_cast<size_t>(2), wrapper.maxDepth);

			std::string wrong = "wrong.json";
			Assert::ExpectException<std::runtime_error>([&] { parser.deserializeObjectFromFile(wrong); });

			// Test read from table file
			std::string tableFile = "table_json.json";

			Scope* table = new Scope();
			TableWrppaer tableWrapper(table);
			ParseCoordinator tableParser(tableWrapper);

			// add handlers for parsing different types
			auto handlerRefInt = tableParser.addHandler(std::make_unique<IntegerHandler>());
			auto handlerRefFloat = tableParser.addHandler(std::make_unique<FloatHandler>());
			auto handlerRefString = tableParser.addHandler(std::make_unique<StringHandler>());
			auto handlerRefVec4 = tableParser.addHandler(std::make_unique<Vector4Handler>());
			auto handlerRefMat4 = tableParser.addHandler(std::make_unique<Matrix4Handler>());
			auto handlerRefTable = tableParser.addHandler(std::make_unique<TableHandler>());

			// deserialize the object from JSON to the table
			deserialized = tableParser.deserializeObjectFromFile(tableFile);
			Assert::IsTrue(deserialized);

			// check the data in the root table
			Assert::AreEqual(1, table->find("int_1")->get<int>(0));
			Assert::AreEqual(1.0f, table->find("float_1")->get<float>(0));
			Assert::AreEqual(std::string("abc"), table->find("str_1")->get<std::string>(0));
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), table->find("vec4_1")->get<glm::vec4>(0));
			Assert::AreEqual(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), table->find("mat4_1")->get<glm::mat4>(0));

			// check int array data in the root table
			Assert::AreEqual(1, table->find("int_array_1")->get<int>(0));
			Assert::AreEqual(2, table->find("int_array_1")->get<int>(1));
			Assert::AreEqual(3, table->find("int_array_1")->get<int>(2));

			// check float array data in the root table
			Assert::AreEqual(1.0f, table->find("float_array_1")->get<float>(0));
			Assert::AreEqual(2.0f, table->find("float_array_1")->get<float>(1));
			Assert::AreEqual(3.0f, table->find("float_array_1")->get<float>(2));

			// check string array data in the root table
			Assert::AreEqual(std::string("abc"), table->find("str_array_1")->get<std::string>(0));
			Assert::AreEqual(std::string("def"), table->find("str_array_1")->get<std::string>(1));
			Assert::AreEqual(std::string("ghi"), table->find("str_array_1")->get<std::string>(2));

			// check vec4 array data in the root table
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), table->find("vec4_array_1")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f), table->find("vec4_array_1")->get<glm::vec4>(1));
			Assert::AreEqual(glm::vec4(9.0f, 10.0f, 11.0f, 12.0f), table->find("vec4_array_1")->get<glm::vec4>(2));

			// check mat4 array data in the root table
			Assert::AreEqual(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), table->find("mat4_array_1")->get<glm::mat4>(0));
			Assert::AreEqual(glm::mat4(2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f), table->find("mat4_array_1")->get<glm::mat4>(1));

			// check the data in the first level nested table
			auto obj1 = table->find("obj_1")->get<Scope*>(0);
			Assert::AreEqual(2, obj1->find("int_1")->get<int>(0));
			Assert::AreEqual(std::string("def"), obj1->find("str_1")->get<std::string>(0));

			// check the data in the second level nested table
			auto obj2_1 = obj1->find("obj_1")->get<Scope*>(0);
			Assert::AreEqual(3, obj2_1->find("int_1")->get<int>(0));
			auto obj2_2 = obj1->find("obj_2")->get<Scope*>(0);
			Assert::AreEqual(3.0f, obj2_2->find("float_1")->get<float>(0));
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), obj2_2->find("vec4_1")->get<glm::vec4>(0));

			// get the handlers from the parser and check if they are initialized
			auto handlerInt = tableParser.getHandler<IntegerHandler>(handlerRefInt);
			auto handlerFloat = tableParser.getHandler<FloatHandler>(handlerRefFloat);
			auto handlerString = tableParser.getHandler<StringHandler>(handlerRefString);
			auto handlerVec4 = tableParser.getHandler<Vector4Handler>(handlerRefVec4);
			auto handlerMat4 = tableParser.getHandler<Matrix4Handler>(handlerRefMat4);
			auto handlerTable = tableParser.getHandler<TableHandler>(handlerRefTable);
			Assert::IsTrue(handlerInt->isInitialized());
			Assert::IsTrue(handlerFloat->isInitialized());
			Assert::IsTrue(handlerString->isInitialized());
			Assert::IsTrue(handlerVec4->isInitialized());
			Assert::IsTrue(handlerMat4->isInitialized());
			Assert::IsTrue(handlerTable->isInitialized());

			// delete memory
			delete table;
		}

		TEST_METHOD(TestOtherStuffs)
		{
			// Test create method for test handlers
			TestParseHandler testHandler1;
			IParseHandler* testHandlerPtr = testHandler1.create();
			Assert::IsNotNull(testHandlerPtr);
			delete testHandlerPtr;

			TestParseIntHandler testIntHandler1;
			IParseHandler* testIntHandlerPtr = testIntHandler1.create();
			Assert::IsNotNull(testIntHandlerPtr);
			delete testIntHandlerPtr;

			// Test create method for handlers
			IntegerHandler intHandler1;
			IParseHandler* intHandlerPtr = intHandler1.create();
			Assert::IsNotNull(intHandlerPtr);
			delete intHandlerPtr;

			FloatHandler floatHandler1;
			IParseHandler* floatHandlerPtr = floatHandler1.create();
			Assert::IsNotNull(floatHandlerPtr);
			delete floatHandlerPtr;

			StringHandler stringHandler1;
			IParseHandler* stringHandlerPtr = stringHandler1.create();
			Assert::IsNotNull(stringHandlerPtr);
			delete stringHandlerPtr;

			Vector4Handler vec4Handler1;
			IParseHandler* vec4HandlerPtr = vec4Handler1.create();
			Assert::IsNotNull(vec4HandlerPtr);
			delete vec4HandlerPtr;

			Matrix4Handler mat4Handler1;
			IParseHandler* mat4HandlerPtr = mat4Handler1.create();
			Assert::IsNotNull(mat4HandlerPtr);
			delete mat4HandlerPtr;

			TableHandler tableHandler1;
			IParseHandler* tableHandlerPtr = tableHandler1.create();
			Assert::IsNotNull(tableHandlerPtr);
			delete tableHandlerPtr;
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}
