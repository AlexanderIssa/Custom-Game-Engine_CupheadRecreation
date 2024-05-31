#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedWarlock.h"
#include "Scope.h"
#include "TypeManager.h"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Datum::DataType DataType;
typedef Fiea::GameEngine::Scope Scope;
typedef Fiea::GameEngine::AttributedFoo AttributedFoo;
typedef Fiea::GameEngine::AttributedBar AttributedBar;
typedef Fiea::GameEngine::AttributedWarlock AttributedWarlock;
typedef Fiea::GameEngine::TypeManager TypeManager;

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
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(AttributedFoo* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedBar>(const AttributedBar* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedBar>(AttributedBar* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedWarlock>(const AttributedWarlock* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<AttributedWarlock>(AttributedWarlock* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
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
	TEST_CLASS(FieaGameEngineAttributeTest)
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

		TEST_METHOD(TestAttributedFooConstructor)
		{
			// Default constructor
			AttributedFoo foo;
			Assert::IsTrue(foo.isAttribute("this"));
			Assert::IsTrue(foo.isPrescribedAttribute("this"));
			Assert::IsFalse(foo.isAuxiliaryAttribute("this"));

			AttributedBar bar;
			Assert::IsTrue(bar.isAttribute("this"));
			Assert::IsTrue(bar.isPrescribedAttribute("this"));
			Assert::IsFalse(bar.isAuxiliaryAttribute("this"));

			AttributedWarlock warlock;
			Assert::IsTrue(warlock.isAttribute("this"));
			Assert::IsTrue(warlock.isPrescribedAttribute("this"));
			Assert::IsFalse(warlock.isAuxiliaryAttribute("this"));

			// Copy constructor
			foo.appendAuxiliaryAttribute("test");
			bar.appendAuxiliaryAttribute("test");
			warlock.appendAuxiliaryAttribute("test");

			AttributedFoo fooCopy(foo);
			Assert::IsTrue(fooCopy.isAttribute("this"));
			Assert::IsTrue(fooCopy.isPrescribedAttribute("this"));
			Assert::IsFalse(fooCopy.isAuxiliaryAttribute("this"));
			Assert::IsTrue(fooCopy.isAttribute("test"));
			Assert::IsFalse(fooCopy.isPrescribedAttribute("test"));
			Assert::IsTrue(fooCopy.isAuxiliaryAttribute("test"));

			AttributedBar barCopy(bar);
			Assert::IsTrue(barCopy.isAttribute("this"));
			Assert::IsTrue(barCopy.isPrescribedAttribute("this"));
			Assert::IsFalse(barCopy.isAuxiliaryAttribute("this"));
			Assert::IsTrue(barCopy.isAttribute("test"));
			Assert::IsFalse(barCopy.isPrescribedAttribute("test"));
			Assert::IsTrue(barCopy.isAuxiliaryAttribute("test"));

			AttributedWarlock warlockCopy(warlock);
			Assert::IsTrue(warlockCopy.isAttribute("this"));
			Assert::IsTrue(warlockCopy.isPrescribedAttribute("this"));
			Assert::IsFalse(warlockCopy.isAuxiliaryAttribute("this"));
			Assert::IsTrue(warlockCopy.isAttribute("test"));
			Assert::IsFalse(warlockCopy.isPrescribedAttribute("test"));
			Assert::IsTrue(warlockCopy.isAuxiliaryAttribute("test"));

			// Move constructor
			AttributedFoo fooMove(std::move(fooCopy));
			Assert::IsTrue(fooMove.isAttribute("this"));
			Assert::IsTrue(fooMove.isPrescribedAttribute("this"));
			Assert::IsFalse(fooMove.isAuxiliaryAttribute("this"));
			Assert::IsTrue(fooMove.isAttribute("test"));
			Assert::IsFalse(fooMove.isPrescribedAttribute("test"));
			Assert::IsTrue(fooMove.isAuxiliaryAttribute("test"));

			AttributedBar barMove(std::move(barCopy));
			Assert::IsTrue(barMove.isAttribute("this"));
			Assert::IsTrue(barMove.isPrescribedAttribute("this"));
			Assert::IsFalse(barMove.isAuxiliaryAttribute("this"));
			Assert::IsTrue(barMove.isAttribute("test"));
			Assert::IsFalse(barMove.isPrescribedAttribute("test"));
			Assert::IsTrue(barMove.isAuxiliaryAttribute("test"));

			AttributedWarlock warlockMove(std::move(warlockCopy));	
			Assert::IsTrue(warlockMove.isAttribute("this"));
			Assert::IsTrue(warlockMove.isPrescribedAttribute("this"));
			Assert::IsFalse(warlockMove.isAuxiliaryAttribute("this"));
			Assert::IsTrue(warlockMove.isAttribute("test"));
			Assert::IsFalse(warlockMove.isPrescribedAttribute("test"));
			Assert::IsTrue(warlockMove.isAuxiliaryAttribute("test"));

			// Other test
			std::string s = fooMove.ToString();
			Assert::AreEqual(s, std::string("AttributedFoo"));

			s = barMove.ToString();
			Assert::AreEqual(s, std::string("AttributedBar"));

			s = warlockMove.ToString();
			Assert::AreEqual(s, std::string("AttributedWarlock"));
		}

		TEST_METHOD(TestAttributedFooAssignment)
		{
			AttributedFoo foo;
			foo.appendAuxiliaryAttribute("test");

			AttributedBar bar;
			bar.appendAuxiliaryAttribute("test");

			AttributedWarlock warlock;
			warlock.appendAuxiliaryAttribute("test");

			// Copy assignment
			AttributedFoo fooCopy;
			fooCopy = foo;
			Assert::IsTrue(fooCopy.isAttribute("this"));
			Assert::IsTrue(fooCopy.isPrescribedAttribute("this"));
			Assert::IsFalse(fooCopy.isAuxiliaryAttribute("this"));
			Assert::IsTrue(fooCopy.isAttribute("test"));
			Assert::IsFalse(fooCopy.isPrescribedAttribute("test"));
			Assert::IsTrue(fooCopy.isAuxiliaryAttribute("test"));

			AttributedBar barCopy;
			barCopy = bar;
			Assert::IsTrue(barCopy.isAttribute("this"));
			Assert::IsTrue(barCopy.isPrescribedAttribute("this"));
			Assert::IsFalse(barCopy.isAuxiliaryAttribute("this"));
			Assert::IsTrue(barCopy.isAttribute("test"));
			Assert::IsFalse(barCopy.isPrescribedAttribute("test"));
			Assert::IsTrue(barCopy.isAuxiliaryAttribute("test"));

			AttributedWarlock warlockCopy;
			warlockCopy = warlock;
			Assert::IsTrue(warlockCopy.isAttribute("this"));
			Assert::IsTrue(warlockCopy.isPrescribedAttribute("this"));
			Assert::IsFalse(warlockCopy.isAuxiliaryAttribute("this"));
			Assert::IsTrue(warlockCopy.isAttribute("test"));
			Assert::IsFalse(warlockCopy.isPrescribedAttribute("test"));
			Assert::IsTrue(warlockCopy.isAuxiliaryAttribute("test"));

			// Move assignment
			AttributedFoo fooMove;
			fooMove = std::move(fooCopy);

			Assert::IsTrue(fooMove.isAttribute("this"));
			Assert::IsTrue(fooMove.isPrescribedAttribute("this"));
			Assert::IsFalse(fooMove.isAuxiliaryAttribute("this"));
			Assert::IsTrue(fooMove.isAttribute("test"));
			Assert::IsFalse(fooMove.isPrescribedAttribute("test"));
			Assert::IsTrue(fooMove.isAuxiliaryAttribute("test"));

			AttributedBar barMove;
			barMove = std::move(barCopy);

			Assert::IsTrue(barMove.isAttribute("this"));
			Assert::IsTrue(barMove.isPrescribedAttribute("this"));
			Assert::IsFalse(barMove.isAuxiliaryAttribute("this"));
			Assert::IsTrue(barMove.isAttribute("test"));
			Assert::IsFalse(barMove.isPrescribedAttribute("test"));
			Assert::IsTrue(barMove.isAuxiliaryAttribute("test"));

			AttributedWarlock warlockMove;
			warlockMove = std::move(warlockCopy);

			Assert::IsTrue(warlockMove.isAttribute("this"));
			Assert::IsTrue(warlockMove.isPrescribedAttribute("this"));
			Assert::IsFalse(warlockMove.isAuxiliaryAttribute("this"));
			Assert::IsTrue(warlockMove.isAttribute("test"));
			Assert::IsFalse(warlockMove.isPrescribedAttribute("test"));
			Assert::IsTrue(warlockMove.isAuxiliaryAttribute("test"));
		}

		TEST_METHOD(TestAttributedIs)
		{
			AttributedFoo foo;
			foo.appendAuxiliaryAttribute("test");
			
			Assert::IsTrue(foo.isAttribute("this"));
			Assert::IsTrue(foo.isPrescribedAttribute("this"));
			Assert::IsFalse(foo.isAuxiliaryAttribute("this"));
			Assert::IsTrue(foo.isAttribute("test"));
			Assert::IsFalse(foo.isPrescribedAttribute("test"));
			Assert::IsTrue(foo.isAuxiliaryAttribute("test"));
			Assert::IsTrue(foo.isAttribute("externalInteger"));
			Assert::IsTrue(foo.isPrescribedAttribute("externalInteger"));
			Assert::IsFalse(foo.isAuxiliaryAttribute("externalInteger"));

			Assert::IsFalse(foo.isAttribute("HAHA"));

			AttributedBar bar;
			bar.appendAuxiliaryAttribute("test");

			Assert::IsTrue(bar.isAttribute("this"));
			Assert::IsTrue(bar.isPrescribedAttribute("this"));
			Assert::IsFalse(bar.isAuxiliaryAttribute("this"));
			Assert::IsTrue(bar.isAttribute("test"));
			Assert::IsFalse(bar.isPrescribedAttribute("test"));
			Assert::IsTrue(bar.isAuxiliaryAttribute("test"));
			Assert::IsTrue(bar.isAttribute("externalInteger"));
			Assert::IsTrue(bar.isPrescribedAttribute("externalInteger"));
			Assert::IsFalse(bar.isAuxiliaryAttribute("externalInteger"));
			Assert::IsTrue(bar.isPrescribedAttribute("externalString"));
			Assert::IsFalse(bar.isAuxiliaryAttribute("externalString"));
			Assert::IsTrue(bar.isPrescribedAttribute("externalStringChild"));
			Assert::IsFalse(bar.isAuxiliaryAttribute("externalStringChild"));

			AttributedWarlock warlock;
			warlock.appendAuxiliaryAttribute("test");
			
			Assert::IsTrue(warlock.isAttribute("this"));
			Assert::IsTrue(warlock.isPrescribedAttribute("this"));
			Assert::IsFalse(warlock.isAuxiliaryAttribute("this"));
			Assert::IsTrue(warlock.isAttribute("test"));
			Assert::IsFalse(warlock.isPrescribedAttribute("test"));
			Assert::IsTrue(warlock.isAuxiliaryAttribute("test"));
			Assert::IsTrue(warlock.isAttribute("name"));
			Assert::IsTrue(warlock.isPrescribedAttribute("name"));
			Assert::IsFalse(warlock.isAuxiliaryAttribute("name"));

			Assert::IsFalse(warlock.isAttribute("HAHA"));	 
		}

		TEST_METHOD(TestAttributedAppendAuxiliaryAttribute)
		{
			// Test for AttributedFoo
			AttributedFoo foo;
			foo.appendAuxiliaryAttribute("test");
			Datum& testDatum = foo.appendAuxiliaryAttribute("test");

			AttributedFoo* fooPtr = foo.find("this")->get<AttributedFoo*>(0);

			Assert::AreEqual(fooPtr, &foo);
			Assert::ExpectException<std::invalid_argument>([&foo] { foo.appendAuxiliaryAttribute("externalInteger"); });

			// Test for AttributeBar
			AttributedBar bar;
			bar.appendAuxiliaryAttribute("test");
			Datum& testDatum2 = bar.appendAuxiliaryAttribute("test");

			AttributedBar* barPtr = bar.find("this")->get<AttributedBar*>(0);

			Assert::AreEqual(barPtr, &bar);
			Assert::ExpectException<std::invalid_argument>([&bar] { bar.appendAuxiliaryAttribute("externalInteger"); });

			// Test for AttributedWarlock
			AttributedWarlock warlock;
			Datum& datumName = warlock["name"];
			Datum& datumAge = warlock["age"];
			Datum& datumManaPoints = warlock["manaPoints"];
			Datum& datumSpellBook = warlock["spellBook"];
			Datum& datumStartPosition = warlock["startPosition"];
			Datum& datumTransform4D = warlock["transform4D"];
			Datum& datumTempHP = warlock.appendAuxiliaryAttribute("tempHP");
			
			// Set warlock's information
			datumName.get<std::string>(0) = "Wyll_Ravengard";
			datumAge.get<int>(0) = 24;
			datumManaPoints.get<float>(0) = 100.0f;
			datumStartPosition.get<glm::vec4>(0) = glm::vec4(1.0f);
			datumTransform4D.get<glm::mat4>(0) = glm::mat4(1.0f);
			datumTempHP.push_back(16);

			// Give him spells
			Scope* spellBook = datumSpellBook.get<Scope*>(0);
			std::string spell1 = "Eldritch_Blast";
			std::string spell2 = "Hex";
			std::string spell3 = "Darkness";
			spellBook->append("spell_page_1").push_back(spell1);
			spellBook->append("spell_page_1").push_back(spell2);
			spellBook->append("spell_page_1").push_back(spell3);

			// Check if the information is set correctly, even nested scopes
			Datum& datumNameAux = *warlock.find("name");
			Datum& datumAgeAux = *warlock.find("age");
			Datum& datumManaPointsAux = *warlock.find("manaPoints");
			Datum& datumSpellBookAux = *warlock.find("spellBook");
			Datum& datumStartPositionAux = *warlock.find("startPosition");
			Datum& datumTransform4DAux = *warlock.find("transform4D");

			// Test if it returns the same Datum
			Datum& datumTempHPAux = warlock.appendAuxiliaryAttribute("tempHP");
			Assert::AreEqual(datumTempHPAux, datumTempHP);
			Assert::AreEqual(&datumTempHPAux, &datumTempHP);

			Assert::AreEqual(datumNameAux.get<std::string>(0), static_cast<std::string>("Wyll_Ravengard"));
			Assert::AreEqual(datumAgeAux.get<int>(0), 24);
			Assert::AreEqual(datumManaPointsAux.get<float>(0), 100.0f);
			Assert::AreEqual(datumStartPositionAux.get<glm::vec4>(0), glm::vec4(1.0f));
			Assert::AreEqual(datumTransform4DAux.get<glm::mat4>(0), glm::mat4(1.0f));
			Assert::AreEqual(datumTempHPAux.get<int>(0), 16);

			Scope* spellBookAux = datumSpellBookAux.get<Scope*>(0);
			Assert::AreEqual(spellBookAux->find("spell_page_1")->get<std::string>(0), spell1);
			Assert::AreEqual(spellBookAux->find("spell_page_1")->get<std::string>(1), spell2);
			Assert::AreEqual(spellBookAux->find("spell_page_1")->get<std::string>(2), spell3);

			// Test 2-way binding for name
			warlock.name = "Blade_of_Frontiers";
			Assert::AreEqual(warlock["name"].get<std::string>(0), static_cast<std::string>("Blade_of_Frontiers"));
			std::string newName = "Blade_of_Avernus";
			warlock["name"].get<std::string>(0) = newName;
			Assert::AreEqual(warlock.name, newName);

			// Test 2-way binding for age
			warlock.age = 25;
			Assert::AreEqual(warlock["age"].get<int>(0), 25);
			int newAge = 26;
			warlock["age"].get<int>(0) = newAge;
			Assert::AreEqual(warlock.age, newAge);

            // Test 2-way binding for manaPoints
            warlock.manaPoints = 200.0f;
			Assert::AreEqual(warlock["manaPoints"].get<float>(0), 200.0f);
			float newManaPoints = 300.0f;
			warlock["manaPoints"].get<float>(0) = newManaPoints;
			Assert::AreEqual(warlock.manaPoints, newManaPoints);

			// Test 2-way binding for startPosition
			warlock.startPosition = glm::vec4(2.0f);
			Assert::AreEqual(warlock["startPosition"].get<glm::vec4>(0), glm::vec4(2.0f));
			glm::vec4 newStartPosition = glm::vec4(3.0f);
			warlock["startPosition"].get<glm::vec4>(0) = newStartPosition;
			Assert::AreEqual(warlock.startPosition, newStartPosition);

			// Test 2-way binding for transform4D
			warlock.transform4D = glm::mat4(2.0f);
			Assert::AreEqual(warlock["transform4D"].get<glm::mat4>(0), glm::mat4(2.0f));
			glm::mat4 newTransform4D = glm::mat4(3.0f);
			warlock["transform4D"].get<glm::mat4>(0) = newTransform4D;
			Assert::AreEqual(warlock.transform4D, newTransform4D);

			// Test 2-way binding for spellBook
			std::string newSpell1 = "Fireball";
			warlock.spellBook["spell_page_1"].push_back(newSpell1);
			Assert::AreEqual(warlock["spellBook"].get<Scope*>(0)->find("spell_page_1")->get<std::string>(3), newSpell1);
			std::string newSpell2 = "Hold_Person";
			warlock["spellBook"].get<Scope*>(0)->find("spell_page_1")->get<std::string>(3) = newSpell2;
			Assert::AreEqual(warlock.spellBook["spell_page_1"].get<std::string>(3), newSpell2);

			// Test 2-way binding for AttributeBar (Child of AttributedFoo)
			bar.externalStringChild = "Hello";
			Assert::AreEqual(bar["externalStringChild"].get<std::string>(0), static_cast<std::string>("Hello"));
			std::string newString = "World";
			bar["externalStringChild"].get<std::string>(0) = newString;
			Assert::AreEqual(bar.externalStringChild, newString);

			Assert::ExpectException<std::invalid_argument>([&warlock] { warlock.appendAuxiliaryAttribute("name"); });
		}

		TEST_METHOD(TestAttributedEquality)
		{
			AttributedFoo foo;
			AttributedFoo foo2;
			Assert::IsTrue(foo == foo);
			Assert::IsFalse(foo != foo);
			Assert::IsFalse(foo == foo2);
			Assert::IsTrue(foo != foo2);

			AttributedWarlock warlock;
			AttributedWarlock warlock2;
			Assert::IsTrue(warlock == warlock);
			Assert::IsFalse(warlock != warlock);
			Assert::IsFalse(warlock == warlock2);
			Assert::IsTrue(warlock != warlock2);

			Assert::IsFalse(foo == warlock);
			Assert::IsTrue(foo != warlock);
			Assert::IsFalse(warlock == foo);
			Assert::IsTrue(warlock != foo);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}