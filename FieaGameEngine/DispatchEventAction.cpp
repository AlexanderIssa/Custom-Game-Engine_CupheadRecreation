#include "pch.h"
#include "DispatchEventAction.h"
#include "FactoryManager.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		// ------------------------------------
		// ---------- Base Action -------------
		// ------------------------------------
		RTTI_DEFINITIONS(DispatchEventAction);


		DispatchEventAction::DispatchEventAction() : Action(DispatchEventAction::TypeIdClass()), myEvent(nullptr), myID(0)
		{
		}

		DispatchEventAction::~DispatchEventAction()
		{
			myID = 0;
			delete myEvent;
		}

		void DispatchEventAction::update(const GameTime& time)
		{
			// Make a new event using the Factory
			Factory<IEvent>* associatedFactory = FactoryManager<IEvent>::find(myEventName);
			assert(associatedFactory != nullptr);
			myEvent = associatedFactory->create(); // creates new ThrowBoneEvent. Saves inside IEvent*
			myID = associatedFactory->classID(); // Saves the ID as well
			// TODO: how to set variables inside this IEvent?
			// Enqueue with a delay of zero
			EventDispatcher::enqueue(myID, myEvent, time, 0.0f);
		}

		std::vector<Signature> DispatchEventAction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_eventName"s, Datum::DataType::STRING, 1, offsetof(DispatchEventAction, myEventName) }
			};
		}

		DispatchEventAction* DispatchEventAction::clone() const
		{
			return new DispatchEventAction(*this);
		}

		void DispatchEventAction::onEnable()
		{
		}

		void DispatchEventAction::onDisable()
		{
		}

		// ----------------------------------------
		// -------- ThrowBone EventAction ---------
		// ----------------------------------------
	}
}