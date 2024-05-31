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

#include "EventDispatcher.h"
#include "Reaction.h"
#include "DispatchEventAction.h"
#include "ThrowBoneEvent.h"
#include <functional>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

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
	MAKE_FACTORY(Scope, Reaction);
	/* MAKE_FACTORY(Scope, ThrowBoneReaction); */
	MAKE_FACTORY(Scope, DispatchEventAction);
	MAKE_FACTORY(IEvent, ThrowBoneEvent);
}

typedef Fiea::GameEngine::RTTI RTTI;
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

typedef Fiea::GameEngine::IEvent IEvent;
typedef Fiea::GameEngine::FactoryManager<Scope> FactoryManager;
typedef Fiea::GameEngine::FactoryManager<IEvent> EventFactoryManager;
typedef Fiea::GameEngine::Factory<Scope> ScopeFactory;
typedef Fiea::GameEngine::AttributedFooFactory AttributedFooFactory;
typedef Fiea::GameEngine::AttributedWarlockFactory AttributedWarlockFactory;
typedef Fiea::GameEngine::GameObjectFactory GameObjectFactory;
typedef Fiea::GameEngine::MonsterFactory MonsterFactory;
typedef Fiea::GameEngine::ActionIncrementFactory ActionIncrementFactory;
typedef Fiea::GameEngine::ActionListFactory ActionListFactory;
typedef Fiea::GameEngine::ActionListWhileFactory ActionListWhileFactory;
typedef Fiea::GameEngine::ActionListSwitchFactory ActionListSwitchFactory;

typedef Fiea::GameEngine::EventDispatcher EventDispatcher;
typedef Fiea::GameEngine::Reaction Reaction;
typedef Fiea::GameEngine::ReactionFactory ReactionFactory;
typedef Fiea::GameEngine::DispatchEventAction DispatchEventAction;
typedef Fiea::GameEngine::DispatchEventActionFactory DispatchEventActionFactory;
typedef Fiea::GameEngine::ThrowBoneEvent ThrowBoneEvent;
typedef Fiea::GameEngine::ThrowBoneEventFactory ThrowBoneEventFactory;
//typedef Fiea::GameEngine::ThrowBoneReactionFactory ThrowBoneReactionFactory;

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
	TEST_CLASS(FieaGameEngineReactionTest)
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
			EventFactoryManager::clear();
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

		TEST_METHOD(ReactionParseTest)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();    
			GameClock clock([&now]() { return now; });    
			GameTime time = clock.Current();    
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<GameObjectFactory>());
			FactoryManager::add(std::make_unique<ActionIncrementFactory>());
			FactoryManager::add(std::make_unique<ReactionFactory>());
			EventFactoryManager::add(std::make_unique<ThrowBoneEventFactory>());

			// Create a parser
			std::string nested = R"({
				"GameObject_World": {
					"float_bones" : 1,
					"Reaction_react1": {
						"str_eventName" : "ThrowBoneEvent",
						"ActionIncrement_callbacks" : {
							"str_key": "float_bones",
							"float_value": 0.5
						}
					}
				}
			})";
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
			bool deserialized = tableParser.deserializeObject(nested);
			Assert::IsTrue(deserialized);

			// Make sure the wrapper looks like
			/*
			World
				|->float_bones: 1
				|->reactions: Reaction_reactions
									|->"str_eventName" : "ThrowBoneEvent",
									|->actions: ActionIncrement
														|->key:"float_bones"
														|->value:1

			*/
			
			Scope* root = attributedWrapper.getRootTable();
			Assert::AreNotEqual(root->find("GameObject_World"), (Datum*)nullptr);

			Scope* world = root->find("GameObject_World")->get<Scope*>(0);
			Assert::AreNotEqual(world->find("obj_reactions"), (Datum*)nullptr); 
			Assert::AreNotEqual(world->find("float_bones"), (Datum*)nullptr); 

			float bones = world->find("float_bones")->get<float>(0);
			Assert::AreEqual(bones, (float)1);
			
			Scope* reactions = world->find("obj_reactions")->get<Scope*>(0);
			Assert::AreNotEqual(reactions->find("Reaction"), (Datum*)nullptr);
			
			Scope* tbr = reactions->find("Reaction")->get<Scope*>(0);
			Reaction* tbr2 = tbr->As<Reaction>();
			Assert::IsFalse(tbr2 == nullptr);
			Assert::AreNotEqual(tbr->find("obj_actions"), (Datum*)nullptr);
			Assert::AreNotEqual(tbr->find("str_eventName"), (Datum*)nullptr);

			std::string name = tbr->find("str_eventName")->get<std::string>(0);
			Assert::AreEqual(name, "ThrowBoneEvent"s);
			
			Scope* actions = tbr->find("obj_actions")->get<Scope*>(0);
			Assert::AreNotEqual(actions->find("ActionIncrement"), (Datum*)nullptr);

			Scope* ai = actions->find("ActionIncrement")->get<Scope*>(0);
			ActionIncrement* ai2 = ai->As<ActionIncrement>();
			Assert::IsFalse(ai2 == nullptr);

			Assert::AreEqual(ai2->find("str_key")->get<std::string>(0), "float_bones"s);
			Assert::AreEqual(ai2->find("float_value")->get<float>(0), (float)0.5);
		}

		TEST_METHOD(ReactionWithEventCallTest)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();    
			GameClock clock([&now]() { return now; });    
			GameTime time = clock.Current();    
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<GameObjectFactory>());
			FactoryManager::add(std::make_unique<ActionIncrementFactory>());
			FactoryManager::add(std::make_unique<ReactionFactory>());
			EventFactoryManager::add(std::make_unique<ThrowBoneEventFactory>());


			// Create a parser
			std::string nested = R"({
				"GameObject_World": {
					"float_bones" : 1,
					"Reaction_react1": {
						"str_eventName" : "ThrowBoneEvent",
						"ActionIncrement_callbacks" : {
							"str_key": "float_bones",
							"float_value": 0.5
						}
					}
				}
			})";
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
			bool deserialized = tableParser.deserializeObject(nested);
			Assert::IsTrue(deserialized);

			// test throwing the event multiple times
			Scope* root = attributedWrapper.getRootTable();
			Scope* world = root->find("GameObject_World")->get<Scope*>(0);

			Assert::AreEqual(world->find("float_bones")->get<float>(0), (float)1);
			ThrowBoneEvent* tbEvent = new ThrowBoneEvent();
			tbEvent->color = "Asdf";
			tbEvent->mass = 1234;
			EventDispatcher::dispatchEvent(tbEvent);
			Assert::AreEqual(world->find("float_bones")->get<float>(0), (float)1.5);
			EventDispatcher::dispatchEvent(tbEvent);
			EventDispatcher::dispatchEvent(tbEvent);
			Assert::AreEqual(world->find("float_bones")->get<float>(0), (float)2.5);
			delete tbEvent;
		}

		TEST_METHOD(EventActionParseTest)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();    
			GameClock clock([&now]() { return now; });    
			GameTime time = clock.Current();    
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<GameObjectFactory>());
			FactoryManager::add(std::make_unique<ActionIncrementFactory>());
			FactoryManager::add(std::make_unique<ReactionFactory>());
			FactoryManager::add(std::make_unique<DispatchEventActionFactory>());
			EventFactoryManager::add(std::make_unique<ThrowBoneEventFactory>());

			// Create a parser
			std::string nested = R"({
				"GameObject_World": {
					"DispatchEventAction_myDispatch": {
						"str_eventName" : "ThrowBoneEvent"
					}
				}
			})";
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
			bool deserialized = tableParser.deserializeObject(nested);
			Assert::IsTrue(deserialized);
		}

		TEST_METHOD(EventActionCallTest)
		{
			// Time
			int ms = 1000;
			std::chrono::time_point now = std::chrono::high_resolution_clock::now();    
			GameClock clock([&now]() { return now; });    
			GameTime time = clock.Current();    
			now += std::chrono::milliseconds(ms);

			// Add a factory
			FactoryManager::add(std::make_unique<GameObjectFactory>());
			FactoryManager::add(std::make_unique<ActionIncrementFactory>());
			FactoryManager::add(std::make_unique<ReactionFactory>());
			FactoryManager::add(std::make_unique<DispatchEventActionFactory>());
			EventFactoryManager::add(std::make_unique<ThrowBoneEventFactory>());

			// Create a parser
			std::string nested = R"({
				"GameObject_World": {
					"DispatchEventAction_myDispatch": {
						"str_eventName" : "ThrowBoneEvent"
					}
				}
			})";
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
			bool deserialized = tableParser.deserializeObject(nested);
			Assert::IsTrue(deserialized);

			// Get world object
			Scope* root = attributedWrapper.getRootTable();
			GameObject* world = root->find("GameObject_World")->get<Scope*>(0)->As<GameObject>();

			// Call update and ensure that event gets thrown
			float i = 0;
			std::function<void(IEvent*)> callback = [&i](IEvent* temp) { i += 0.5; };
			EventDispatcher::addListener<ThrowBoneEvent>(&callback);

			Assert::AreEqual(i, (float)0);
			world->enabled = true;
			world->update(Fiea::GameEngine::CreateElapsedTime(1));
			EventDispatcher::update(Fiea::GameEngine::CreateElapsedTime(1));
			Assert::AreEqual(i, (float)0.5);
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}