#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "AttributedWarlock.h"
#include "GameObject.h"
#include "Monster.h"
#include "Action.h"
#include "ActionIncrement.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "ActionListSwitch.h"
#include "Scope.h"
#include "TypeManager.h"
#include "EventDispatcher.h"

#include "Reaction.h"
#include "DispatchEventAction.h"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Datum::DataType DataType;
typedef Fiea::GameEngine::Scope Scope;
typedef Fiea::GameEngine::AttributedFoo AttributedFoo;
typedef Fiea::GameEngine::AttributedBar AttributedBar;
typedef Fiea::GameEngine::AttributedWarlock AttributedWarlock;
typedef Fiea::GameEngine::GameObject GameObject;
typedef Fiea::GameEngine::Monster Monster;
typedef Fiea::GameEngine::Action Action;
typedef Fiea::GameEngine::ActionIncrement ActionIncrement;
typedef Fiea::GameEngine::ActionList ActionList;
typedef Fiea::GameEngine::ActionListWhile ActionListWhile;
typedef Fiea::GameEngine::ActionListSwitch ActionListSwitch;	
typedef Fiea::GameEngine::TypeManager TypeManager;
typedef Fiea::GameEngine::EventDispatcher EventDispatcher;
typedef Fiea::GameEngine::Reaction Reaction;
typedef Fiea::GameEngine::DispatchEventAction DispatchEventAction;


namespace FieaGameEnginetest
{
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		// Manully set the types parent-child relationship for now
		GameObject::registerSignatures();
		TypeManager::registerTypeAsChild(Monster::TypeIdClass(), GameObject::TypeIdClass(), Monster::prescribedAttributes());

		Action::registerSignatures();
		TypeManager::registerTypeAsChild(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::prescribedAttributes());
		TypeManager::registerTypeAsChild(ActionList::TypeIdClass(), Action::TypeIdClass(), ActionList::prescribedAttributes());
		TypeManager::registerTypeAsChild(ActionListWhile::TypeIdClass(), ActionList::TypeIdClass(), ActionListWhile::prescribedAttributes());
		TypeManager::registerTypeAsChild(ActionListSwitch::TypeIdClass(), ActionList::TypeIdClass(), ActionListSwitch::prescribedAttributes());

		TypeManager::registerTypeAsChild(Reaction::TypeIdClass(), ActionList::TypeIdClass(), Reaction::prescribedAttributes());
		TypeManager::registerTypeAsChild(DispatchEventAction::TypeIdClass(), Action::TypeIdClass(), DispatchEventAction::prescribedAttributes());

		AttributedFoo::registerSignatures();
		TypeManager::registerTypeAsChild(AttributedBar::TypeIdClass(), AttributedFoo::TypeIdClass(), AttributedBar::prescribedAttributes());

		AttributedWarlock::registerSignatures();
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		TypeManager::clear();
	}
}