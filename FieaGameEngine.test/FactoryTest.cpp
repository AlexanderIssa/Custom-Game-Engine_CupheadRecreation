#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedWarlock.h"
#include "GameObject.h"
#include "GameClock.h"
#include "Monster.h"
#include "FactoryManager.h"
#include "Factory.h"

#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "ActionListSwitch.h"

#include "ParseCoordinator.h"
#include "IParseHandler.h"
#include "IntegerHandler.h"
#include "FloatHandler.h"
#include "StringHandler.h"
#include "Vector4Handler.h"
#include "Matrix4Handler.h"
#include "AttributedHandler.h"
#include "AttributedWrapper.h"
#include <sstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::GameEngine
{
	//MAKE_FACTORY(Scope, Scope);
	MAKE_FACTORY(Scope, AttributedFoo);
	MAKE_FACTORY(Scope, AttributedWarlock);
	MAKE_FACTORY(Scope, GameObject);
	MAKE_FACTORY(Scope, Monster);
	//MAKE_FACTORY(Scope, Action);
	MAKE_FACTORY(Scope, ActionIncrement);
	MAKE_FACTORY(Scope, ActionList);
	MAKE_FACTORY(Scope, ActionListWhile);
	MAKE_FACTORY(Scope, ActionListSwitch);
}

typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Datum::DataType DataType;
typedef Fiea::GameEngine::Scope Scope;
typedef Fiea::GameEngine::AttributedFoo AttributedFoo;
typedef Fiea::GameEngine::AttributedBar AttributedBar;
typedef Fiea::GameEngine::AttributedWarlock AttributedWarlock;
typedef Fiea::GameEngine::GameObject GameObject;
typedef Fiea::GameEngine::Monster Monster;
typedef Fiea::GameEngine::TypeManager TypeManager;

typedef Fiea::GameEngine::Action Action;
typedef Fiea::GameEngine::ActionIncrement ActionIncrement;
typedef Fiea::GameEngine::ActionList ActionList;
typedef Fiea::GameEngine::ActionListWhile ActionListWhile;
typedef Fiea::GameEngine::ActionListSwitch ActionListSwitch;

typedef Fiea::GameEngine::FactoryManager<Scope> FactoryManager;
typedef Fiea::GameEngine::Factory<Scope> ScopeFactory;
typedef Fiea::GameEngine::AttributedFooFactory AttributedFooFactory;
typedef Fiea::GameEngine::AttributedWarlockFactory AttributedWarlockFactory;
typedef Fiea::GameEngine::GameObjectFactory GameObjectFactory;
typedef Fiea::GameEngine::MonsterFactory MonsterFactory;
typedef Fiea::GameEngine::ActionIncrementFactory ActionIncrementFactory;
typedef Fiea::GameEngine::ActionListFactory ActionListFactory;
typedef Fiea::GameEngine::ActionListWhileFactory ActionListWhileFactory;
typedef Fiea::GameEngine::ActionListSwitchFactory ActionListSwitchFactory;

typedef Fiea::GameEngine::AttributedWrapper AttributedWrppaer;
typedef Fiea::GameEngine::ParseCoordinator ParseCoordinator;
typedef Fiea::GameEngine::IParseHandler IParseHandler;
typedef Fiea::GameEngine::IntegerHandler IntegerHandler;
typedef Fiea::GameEngine::FloatHandler FloatHandler;
typedef Fiea::GameEngine::StringHandler StringHandler;
typedef Fiea::GameEngine::Vector4Handler Vector4Handler;
typedef Fiea::GameEngine::Matrix4Handler Matrix4Handler;
typedef Fiea::GameEngine::AttributedHandler AttributedHandler;

typedef Fiea::GameEngine::GameClock GameClock;
typedef Fiea::GameEngine::GameTime GameTime;

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
	inline std::wstring ToString<GameObject>(const GameObject* t)
	{
		RETURN_WIDE_STRING(L"GameObject Ptr");
	}

	template<>
	inline std::wstring ToString<GameObject>(GameObject* t)
	{
		RETURN_WIDE_STRING(L"GameObject Ptr");
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
	TEST_CLASS(FieaGameEngineFactoryTest)
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
			FactoryManager::clear();

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

		TEST_METHOD(TestCreateFactory)
		{
			// Add a factory
			FactoryManager::add(std::make_unique<AttributedFooFactory>());

			// Find the factory and create a new object
			ScopeFactory* fooFactory2 = FactoryManager::find("AttributedFoo");
			AttributedFoo* foo = fooFactory2->create()->As<AttributedFoo>();

			// Create a new object through the manager
			AttributedFoo* foo2 = FactoryManager::create("AttributedFoo")->As<AttributedFoo>();

			// Test exception
			Assert::ExpectException<std::invalid_argument>([&] {FactoryManager::create("AttributedBar"); });

			// Clean up
			FactoryManager::remove("AttributedFoo");
			delete foo;
			delete foo2;
		}

		TEST_METHOD(TestParsingWithAttributedFactory)
		{
			// Add a factory
			FactoryManager::add(std::make_unique<AttributedWarlockFactory>());

			// Create a parser
			std::string file = "warlock_json.json";

			AttributedWrppaer attributedWrapper;
			ParseCoordinator tableParser(attributedWrapper);
			Assert::IsTrue(tableParser.getWrapper()->Equals(&attributedWrapper));

			// Add handlers for parsing different types
			auto handlerRefInt = tableParser.addHandler(std::make_unique<IntegerHandler>());
			auto handlerRefFloat = tableParser.addHandler(std::make_unique<FloatHandler>());
			auto handlerRefString = tableParser.addHandler(std::make_unique<StringHandler>());
			auto handlerRefVec4 = tableParser.addHandler(std::make_unique<Vector4Handler>());
			auto handlerRefMat4 = tableParser.addHandler(std::make_unique<Matrix4Handler>());
			auto handlerRefTable = tableParser.addHandler(std::make_unique<AttributedHandler>());

			// Dserialize the object from JSON to the table
			bool deserialized = tableParser.deserializeObjectFromFile(file);
			Assert::IsTrue(deserialized);

			// Check if the object is created
			AttributedWarlock* table = attributedWrapper.getRootTable()->find("AttributedWarlock_wyll")->get<Scope*>(0)->As<AttributedWarlock>();
			Assert::IsNotNull(table);
			
			// Check the root data in the table
			Assert::AreEqual(25, table->find("int_newAge")->get<int>(0));
			Assert::AreEqual(100.0f, table->find("float_newMana")->get<float>(0));
			Assert::AreEqual(std::string("Blade_of_Frontiers"), table->find("str_newName")->get<std::string>(0));
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), table->find("vec4_newPosition")->get<glm::vec4>(0));
			Assert::AreEqual(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), table->find("mat4_newTransform")->get<glm::mat4>(0));

			// Check array of integers
			Assert::AreEqual(3, table->find("int_target_count")->get<int>(0));
			Assert::AreEqual(2, table->find("int_target_count")->get<int>(1));
			Assert::AreEqual(1, table->find("int_target_count")->get<int>(2));

			// Check array of floats
			Assert::AreEqual(50.0f, table->find("float_target_timer")->get<float>(0));
			Assert::AreEqual(35.7f, table->find("float_target_timer")->get<float>(1));
			Assert::AreEqual(83.5f, table->find("float_target_timer")->get<float>(2));

			// Check array of strings
			Assert::AreEqual(std::string("goblin"), table->find("str_target_enemy")->get<std::string>(0));
			Assert::AreEqual(std::string("imp"), table->find("str_target_enemy")->get<std::string>(1));
			Assert::AreEqual(std::string("troll"), table->find("str_target_enemy")->get<std::string>(2));

			// Check array of vec4
			Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), table->find("vec4_target_position")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f), table->find("vec4_target_position")->get<glm::vec4>(1));
			Assert::AreEqual(glm::vec4(9.0f, 10.0f, 11.0f, 12.0f), table->find("vec4_target_position")->get<glm::vec4>(2));

			// Check array of mat4
			Assert::AreEqual(glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f), table->find("mat4_target_transform")->get<glm::mat4>(0));

			// Check the first level nested data in the table
			auto obj1 = table->find("obj_newSpellbook")->get<Scope*>(0);
			Assert::IsNotNull(obj1);

			// Check the second level nested data in the table
			auto obj2_1 = obj1->find("obj_page1")->get<Scope*>(0);
			Assert::AreEqual(std::string("Eldritch_Blast"), obj2_1->find("str_spell1")->get<std::string>(0));
			Assert::AreEqual(std::string("Hex"), obj2_1->find("str_spell2")->get<std::string>(0));
			Assert::AreEqual(std::string("Darkness"), obj2_1->find("str_spell3")->get<std::string>(0));

			auto obj2_2 = obj1->find("obj_page2")->get<Scope*>(0);
			Assert::AreEqual(std::string("Fireball"), obj2_2->find("str_spell1")->get<std::string>(0));

			// Check the array of warlocks
			auto child1 = table->find("AttributedWarlock_friends")->get<Scope*>(0)->As<AttributedWarlock>();
			Assert::AreEqual(26, child1->find("int_age")->get<int>(0));
			Assert::AreEqual(std::string("lockA"), child1->find("str_name")->get<std::string>(0));

			auto child2 = table->find("AttributedWarlock_friends")->get<Scope*>(1)->As<AttributedWarlock>();
			Assert::AreEqual(76, child2->find("int_age")->get<int>(0));
			Assert::AreEqual(std::string("lockB"), child2->find("str_name")->get<std::string>(0));

			// Test remove factory
			FactoryManager::remove("AttributedWarlock");
		}

		TEST_METHOD(TestParsingWithGameObjectFactory)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();
			GameClock clock([&now]() { return now; });
			GameTime time = clock.Current();
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<GameObjectFactory>());

			// Create a parser
			std::string file = "game_object_json.json";

			AttributedWrppaer attributedWrapper;
			ParseCoordinator tableParser(attributedWrapper);
			Assert::IsTrue(tableParser.getWrapper()->Equals(&attributedWrapper));

			// Add handlers for parsing different types
			auto handlerRefInt = tableParser.addHandler(std::make_unique<IntegerHandler>());
			auto handlerRefFloat = tableParser.addHandler(std::make_unique<FloatHandler>());
			auto handlerRefString = tableParser.addHandler(std::make_unique<StringHandler>());
			auto handlerRefVec4 = tableParser.addHandler(std::make_unique<Vector4Handler>());
			auto handlerRefMat4 = tableParser.addHandler(std::make_unique<Matrix4Handler>());
			auto handlerRefTable = tableParser.addHandler(std::make_unique<AttributedHandler>());

			// Dserialize the object from JSON to the table
			bool deserialized = tableParser.deserializeObjectFromFile(file);
			Assert::IsTrue(deserialized);

			// Check if the object is created
			GameObject* masterGameObject = attributedWrapper.getRootTable()->find("GameObject_npc")->get<Scope*>(0)->As<GameObject>();
			Assert::IsNotNull(masterGameObject);

			// Modify prescripted data
			masterGameObject->name = "Peter";
			masterGameObject->transform.position = glm::vec4(0.0f);
			masterGameObject->transform.rotation = glm::vec4(0.0f);
			masterGameObject->transform.scale = glm::vec4(1.0f);

			// Add child game objects
			GameObject* childObj1 = new GameObject();
			GameObject* childObj2 = new GameObject();
			masterGameObject->addChild(childObj1, "GameObject_peter_jr");
			masterGameObject->addChild(childObj2, "GameObject_vivian");

			Assert::ExpectException<std::invalid_argument>([&] {masterGameObject->addChild(nullptr, "GameObject_null"); });

			GameObject* childObj3 = masterGameObject->getChild("GameObject_peter_jr");
			Assert::ExpectException<std::invalid_argument>([&] {masterGameObject->addChild(childObj3, "GameObject_peter_jr"); });

			// Test Update
			masterGameObject->update(time);

			// Check the children
			Assert::AreEqual(std::string("GameObject_peter_jr"), masterGameObject->getChild("GameObject_peter_jr")->name);
			Assert::AreEqual(std::string("GameObject_vivian"), masterGameObject->getChild("GameObject_vivian")->name);
			//Assert::ExpectException<std::invalid_argument>([&] {masterGameObject->getChild("GameObject_jeremy"); });

			// Check prescripted data
			Assert::AreEqual(std::string("Peter"), masterGameObject->find("str_name")->get<std::string>(0));
			Assert::AreEqual(glm::vec4(0.0f), masterGameObject->find("vec4_position")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(0.0f), masterGameObject->find("vec4_rotation")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(1.0f), masterGameObject->find("vec4_scale")->get<glm::vec4>(0));

			// Test get const child
			const GameObject* constTable = attributedWrapper.getRootTable()->find("GameObject_npc")->get<Scope*>(0)->As<GameObject>();
			const GameObject* constChild1 = constTable->getChild("GameObject_peter_jr");
			const GameObject* constChild2 = constTable->getChild("GameObject_vivian");
			Assert::AreEqual(std::string("GameObject_peter_jr"), constChild1->name);
			Assert::AreEqual(std::string("GameObject_vivian"), constChild2->name);
			//Assert::ExpectException<std::invalid_argument>([&] {constTable->getChild("GameObject_jeremy"); });

			// Test remove children
			masterGameObject->removeChild("GameObject_peter_jr");
			masterGameObject->removeChild("GameObject_vivian");

			Assert::ExpectException<std::invalid_argument>([&] {masterGameObject->removeChild("GameObject_peter_jr"); });

			// ================================= Test with data from json file =================================

			// Check the root data in the table
			Assert::AreEqual(25, masterGameObject->find("int_age")->get<int>(0));
			Assert::AreEqual(100.0f, masterGameObject->find("float_mana")->get<float>(0));

			// Check the first level nested data in the table
			auto obj1 = masterGameObject->find("obj_spellbook")->get<Scope*>(0);
			Assert::IsNotNull(obj1);

			// Check the second level nested data in the table
			auto obj2_1 = obj1->find("obj_page1")->get<Scope*>(0);
			Assert::AreEqual(std::string("Eldritch_Blast"), obj2_1->find("str_spell1")->get<std::string>(0));
			Assert::AreEqual(std::string("Hex"), obj2_1->find("str_spell2")->get<std::string>(0));
			Assert::AreEqual(std::string("Darkness"), obj2_1->find("str_spell3")->get<std::string>(0));

			auto obj2_2 = obj1->find("obj_page2")->get<Scope*>(0);
			Assert::AreEqual(std::string("Fireball"), obj2_2->find("str_spell1")->get<std::string>(0));

			// Check the array of gameObjects
			/*auto child1 = masterGameObject->find("GameObject_friends")->get<Scope*>(0)->As<GameObject>();
			Assert::AreEqual(26, child1->find("int_age")->get<int>(0));

			auto child2 = masterGameObject->find("GameObject_friends")->get<Scope*>(1)->As<GameObject>();
			Assert::AreEqual(76, child2->find("int_age")->get<int>(0));*/

			// ================================= Test append aux attributes =================================

			masterGameObject->appendAuxiliaryAttribute("int_age");
			masterGameObject->appendAuxiliaryAttribute("int_age");
			masterGameObject->find("int_age")->push_back(69);
			Assert::AreEqual(69, masterGameObject->find("int_age")->get<int>(1));
			Assert::ExpectException<std::invalid_argument>([&] {masterGameObject->appendAuxiliaryAttribute("str_name"); });

			// ========================================== Clean up ==========================================

			// Test remove factory
			FactoryManager::remove("GameObject");
		}

		TEST_METHOD(TestParsingWithMonsterFactory)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();
			GameClock clock([&now]() { return now; });
			GameTime time = clock.Current();
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<GameObjectFactory>());
			FactoryManager::add(std::make_unique<MonsterFactory>());
			FactoryManager::add(std::make_unique<ActionListFactory>());

			// Create a parser
			std::string file = "Monster_json.json";

			AttributedWrppaer attributedWrapper;
			ParseCoordinator tableParser(attributedWrapper);
			Assert::IsTrue(tableParser.getWrapper()->Equals(&attributedWrapper));

			// Add handlers for parsing different types
			auto handlerRefInt = tableParser.addHandler(std::make_unique<IntegerHandler>());
			auto handlerRefFloat = tableParser.addHandler(std::make_unique<FloatHandler>());
			auto handlerRefString = tableParser.addHandler(std::make_unique<StringHandler>());
			auto handlerRefVec4 = tableParser.addHandler(std::make_unique<Vector4Handler>());
			auto handlerRefMat4 = tableParser.addHandler(std::make_unique<Matrix4Handler>());
			auto handlerRefTable = tableParser.addHandler(std::make_unique<AttributedHandler>());

			// Dserialize the object from JSON to the table
			bool deserialized = tableParser.deserializeObjectFromFile(file);
			Assert::IsTrue(deserialized);

			// Check if the object is created
			Monster* masterMonster = attributedWrapper.getRootTable()->find("Monster_boss")->get<Scope*>(0)->As<Monster>();
			Assert::IsNotNull(masterMonster);

			// Modify prescripted data
			//masterMonster->name = "Troll";
			masterMonster->transform.position = glm::vec4(0.0f);
			masterMonster->transform.rotation = glm::vec4(0.0f);
			masterMonster->transform.scale = glm::vec4(1.0f);

			// Add child game objects
			GameObject* hobgoblinObj = new GameObject();
			GameObject* goblinObj = new GameObject();
			masterMonster->addChild(hobgoblinObj, "GameObject_hobgoblin");
			hobgoblinObj->addChild(goblinObj, "GameObject_goblin");

			Assert::AreEqual(std::string("Troll"), masterMonster->name);
			Assert::ExpectException<std::invalid_argument>([&] {masterMonster->addChild(nullptr, "GameObject_null"); });

			GameObject* childObj3 = masterMonster->getChild("GameObject_hobgoblin");
			Assert::ExpectException<std::invalid_argument>([&] {masterMonster->addChild(childObj3, "GameObject_hobgoblin"); });

			// Test Update
			masterMonster->update(time);

			// Check the children
			Assert::AreEqual(std::string("GameObject_hobgoblin"), masterMonster->getChild("GameObject_hobgoblin")->name);
			Assert::AreEqual(std::string("GameObject_goblin"), hobgoblinObj->getChild("GameObject_goblin")->name);
			//Assert::ExpectException<std::invalid_argument>([&] {masterMonster->getChild("GameObject_orc"); });

			// Check prescripted data
			Assert::AreEqual(std::string("Troll"), masterMonster->find("str_name")->get<std::string>(0));
			Assert::AreEqual(glm::vec4(0.0f), masterMonster->find("vec4_position")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(0.0f), masterMonster->find("vec4_rotation")->get<glm::vec4>(0));
			Assert::AreEqual(glm::vec4(1.0f), masterMonster->find("vec4_scale")->get<glm::vec4>(0));

			// ================================= Test with data from json file =================================

			// Check the root data in the table
			Assert::AreEqual(105, masterMonster->find("int_age")->get<int>(0));
			Assert::AreEqual(100.0f, masterMonster->find("float_health")->get<float>(0));

			// Check the first level nested data in the table
			auto obj1 = masterMonster->find("obj_skillbook")->get<Scope*>(0);
			Assert::IsNotNull(obj1);

			// Check the second level nested data in the table
			auto obj2_1 = obj1->find("obj_page1")->get<Scope*>(0);
			Assert::AreEqual(std::string("Kick"), obj2_1->find("str_skill1")->get<std::string>(0));
			Assert::AreEqual(std::string("Side_Kick"), obj2_1->find("str_skill2")->get<std::string>(0));
			Assert::AreEqual(std::string("Drop_Kick"), obj2_1->find("str_skill3")->get<std::string>(0));

			auto obj2_2 = obj1->find("obj_page2")->get<Scope*>(0);
			Assert::AreEqual(std::string("Fire_Breath"), obj2_2->find("str_skill1")->get<std::string>(0));

			// Check the array of gameObjects
			//auto child1 = masterMonster->find("obj_minions")->get<Scope*>(0)->As<Monster>();
			//Assert::AreEqual(28, child1->find("int_age")->get<int>(0));
			//Assert::AreEqual(50.0f, child1->find("float_health")->get<float>(0));

			//auto child2 = masterMonster->find("obj_minions")->get<Scope*>(1)->As<Monster>();
			//Assert::AreEqual(56, child2->find("int_age")->get<int>(0));
			//Assert::AreEqual(50.0f, child2->find("float_health")->get<float>(0));

			//// Test Update
			//child1->update(time);
			//child2->update(time);
		}

		TEST_METHOD(TestParsingWithActionFactory)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();    
			GameClock clock([&now]() { return now; });    
			GameTime time = clock.Current();    
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<MonsterFactory>());
			FactoryManager::add(std::make_unique<ActionIncrementFactory>());
			FactoryManager::add(std::make_unique<ActionListFactory>());
			FactoryManager::add(std::make_unique<ActionListWhileFactory>());
			FactoryManager::add(std::make_unique<ActionListSwitchFactory>());

			// Create a parser
			std::string file = "Monster_json_2.json";

			AttributedWrppaer attributedWrapper;
			ParseCoordinator tableParser(attributedWrapper);
			Assert::IsTrue(tableParser.getWrapper()->Equals(&attributedWrapper));

			// Add handlers for parsing different types
			auto handlerRefInt = tableParser.addHandler(std::make_unique<IntegerHandler>());
			auto handlerRefFloat = tableParser.addHandler(std::make_unique<FloatHandler>());
			auto handlerRefString = tableParser.addHandler(std::make_unique<StringHandler>());
			auto handlerRefVec4 = tableParser.addHandler(std::make_unique<Vector4Handler>());
			auto handlerRefMat4 = tableParser.addHandler(std::make_unique<Matrix4Handler>());
			auto handlerRefTable = tableParser.addHandler(std::make_unique<AttributedHandler>());

			// Dserialize the object from JSON to the table
			bool deserialized = tableParser.deserializeObjectFromFile(file);
			Assert::IsTrue(deserialized);

			// Check if the object is created
			Monster* masterMonster = attributedWrapper.getRootTable()->find("Monster_boss")->get<Scope*>(0)->As<Monster>();
			Assert::IsNotNull(masterMonster);
			masterMonster->enabled = true;
			masterMonster->alive = true;

			// Add actions
			masterMonster->createAction("ActionIncrement");
			masterMonster->createAction("ActionList");
			masterMonster->createAction("ActionListWhile");
			masterMonster->createAction("ActionListSwitch");

			ActionIncrement* actionIncrement1 = masterMonster->getAction("ActionIncrement")->As<ActionIncrement>();
			ActionList* actionList1 = masterMonster->getAction("ActionList")->As<ActionList>();
			ActionListWhile* actionListWhile1 = masterMonster->getAction("ActionListWhile")->As<ActionListWhile>();
			ActionListSwitch* actionListSwitch1 = masterMonster->getAction("ActionListSwitch")->As<ActionListSwitch>();

			// Modify actionIncrement1 target to test update
			actionIncrement1->key = "float_health";
			actionIncrement1->value = 10.0f;
	
			// Add actions to the action list
			actionList1->createAction("ActionIncrement");
			ActionIncrement* actionIncrement2 = actionList1->getAction("ActionIncrement")->As<ActionIncrement>();

			// Modify actionIncrement2 target to test update
			actionIncrement2->key = "float_stamina";
			actionIncrement2->value = 20.0f;

			// Initialize the actionListWhile1
			ActionIncrement* preAction = new ActionIncrement();
			preAction->key = "int_age";
			preAction->value = 1;

			ActionIncrement* postAction = new ActionIncrement();
			postAction->key = "int_age";
			postAction->value = 1;

			actionListWhile1->SetPreamble(preAction);
			actionListWhile1->SetIncrement(postAction);
			actionListWhile1->SetCondition(10);
			actionListWhile1->createAction("ActionIncrement");
			ActionIncrement* whileAction = actionListWhile1->getAction("ActionIncrement")->As<ActionIncrement>();
			whileAction->key = "float_hunger";
			whileAction->value = -1.0f;

			// Test Update
			masterMonster->update(time);

			// Check the actions
			Assert::AreEqual(110.0f, masterMonster->find("float_health")->get<float>(0));
			Assert::AreEqual(220.0f, masterMonster->find("float_stamina")->get<float>(0));
			Assert::AreEqual(111, masterMonster->find("int_age")->get<int>(0));
			Assert::AreEqual(15.0f, masterMonster->find("float_hunger")->get<float>(0));

			// Test ActionListSwitch
			actionListSwitch1->addCase("case1", "ActionIncrement");
			actionListSwitch1->addCase("case2", "ActionIncrement" , true);
			actionListSwitch1->addCase("case3", "ActionIncrement");
			actionListSwitch1->addCase("case4", "ActionIncrement");
			ActionIncrement* caseAction1 = actionListSwitch1->getAction("ActionIncrement", 0)->As<ActionIncrement>();
			ActionIncrement* caseAction2 = actionListSwitch1->getAction("ActionIncrement", 1)->As<ActionIncrement>();
			ActionIncrement* caseAction3 = actionListSwitch1->getAction("ActionIncrement", 2)->As<ActionIncrement>();
			ActionIncrement* caseAction4 = actionListSwitch1->getAction("ActionIncrement", 3)->As<ActionIncrement>();
			caseAction1->key = "int_age";
			caseAction1->value = 1;
			caseAction2->key = "float_health";
			caseAction2->value = 10.0f;
			caseAction3->key = "float_stamina";
			caseAction3->value = 20.0f;
			caseAction4->key = "float_hunger";
			caseAction4->value = -1.0f;

			actionListSwitch1->setCurrentCase("case1");
			masterMonster->update(time);
			Assert::AreEqual(113, masterMonster->find("int_age")->get<int>(0));
			Assert::AreEqual(120.0f, masterMonster->find("float_health")->get<float>(0));
			Assert::AreEqual(240.0f, masterMonster->find("float_stamina")->get<float>(0));
			Assert::AreEqual(15.0f, masterMonster->find("float_hunger")->get<float>(0));

			actionListSwitch1->setCurrentCase("case2");
			masterMonster->update(time);
			Assert::AreEqual(114, masterMonster->find("int_age")->get<int>(0));
			Assert::AreEqual(140.0f, masterMonster->find("float_health")->get<float>(0));
			Assert::AreEqual(280.0f, masterMonster->find("float_stamina")->get<float>(0));
			Assert::AreEqual(15.0f, masterMonster->find("float_hunger")->get<float>(0));

			// Remove cases
			actionListSwitch1->removeCase("case3");
			actionListSwitch1->removeCase("case2");
			actionListSwitch1->removeCase("case4");
			actionListSwitch1->removeCase("case1");

			// Test exception
			Assert::ExpectException<std::invalid_argument>([&] {actionListSwitch1->setCurrentCase("wrong");; });
			Assert::ExpectException<std::invalid_argument>([&] {actionListSwitch1->addCase("", "ActionIncrement"); });
			Assert::ExpectException<std::invalid_argument>([&] {actionListSwitch1->addCase(" ", "ActionIncrement"); });
			Assert::ExpectException<std::invalid_argument>([&] {actionListSwitch1->addCase("	", "ActionIncrement"); });
			Assert::ExpectException<std::invalid_argument>([&] {actionListSwitch1->setCurrentCase("case1"); });
			Assert::ExpectException<std::invalid_argument>([&] {actionListSwitch1->removeCase("case1"); });

			// Test const get
			const Monster* constMonster = attributedWrapper.getRootTable()->find("Monster_boss")->get<Scope*>(0)->As<Monster>();
			const Action* constAction = constMonster->getAction("ActionIncrement");
			const ActionList* constActionList = constMonster->getAction("ActionList")->As<ActionList>();
			const Action* constActionInActionList = constActionList->getAction("ActionIncrement");

			// Test add to and remove frome actionlist
			ActionList* actionList2 = new ActionList();
			actionList1->addAction(actionList2);
			
			actionList1->removeAction("ActionList");
			Assert::ExpectException<std::invalid_argument>([&] {actionList1->removeAction("ActionList"); });

			// Test exception
			Assert::ExpectException<std::invalid_argument>([&] {actionListWhile1->SetPreamble(nullptr); });
			Assert::ExpectException<std::invalid_argument>([&] {actionListWhile1->SetIncrement(nullptr); });
			Assert::ExpectException<std::invalid_argument>([&] {actionList1->getAction("ActionList"); });
			Assert::ExpectException<std::invalid_argument>([&] {constActionList->getAction("ActionList"); });
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}