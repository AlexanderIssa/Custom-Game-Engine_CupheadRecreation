#include "pch.h"
#include "Reaction.h"

#include "ThrowBoneEvent.h"

// For getting IdType from string
#include "FactoryManager.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		// ------------------------------------
		// -------- Base Reaction -------------
		// ------------------------------------
		RTTI_DEFINITIONS(Reaction);

		Reaction::Reaction() : ActionList(Reaction::TypeIdClass()), myNotifyCallback(nullptr), myEventName("DefaultEventName")
		{

		}

		/*
		Reaction::Reaction(IdType idOfChild) : ActionList(idOfChild), myNotifyCallback(nullptr)
		{
			//FactoryManager<Scope>::find(
			//callbacks = new ActionList();
			//adopt(std::move(callbacks), "obj_callbacks");
		}
		*/


		Reaction::~Reaction()
		{
			UnsubscribeFromCorrespondingEvent();
		}

		Reaction::Reaction(const Reaction&) : myNotifyCallback(nullptr), myID(0)
		{
			assert(false);
			// Currently, copy constructor of Reaction is unimplemented. will create a shallow copy of mEventCallback, and bad things will happen.
		}

		Reaction* Reaction::clone() const
		{
			return new Reaction(*this);
		}

		std::vector<Signature> Reaction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_eventName"s, Datum::DataType::STRING, 1, offsetof(Reaction, myEventName) }
				//{ "obj_callbacks"s, Datum::DataType::TABLE, 0, 0 }
			};
		}

		Datum& Reaction::appendAuxiliaryAttribute(const std::string& key)
		{
			assert(false); // Dummy function to fulfill override requirements. This should never be called
			return *(new Datum()); // Add insult to injury. Free memory leak!
		}

		void Reaction::Notify(IEvent* payload)
		{
			// TODO: processing of payload somehow
			//payload->As<ThrowBoneEvent>()->color;
			//payload->As<ThrowBoneEvent>()->mass;
			update(CreateElapsedTime(0));
		}

		void Reaction::SubscribeToCorrespondingEvent()
		{
			// Find the IdType of the associated event using FactoryManager's map
			Factory<IEvent>* associatedFactory = FactoryManager<IEvent>::find(myEventName);
			assert(associatedFactory != nullptr);
			myID = associatedFactory->classID();

			// Add a listener to EventDispatcher based on what myID is
			auto bindedFunction = BIND_FUNC(this, Reaction::Notify);
			myNotifyCallback = new EventCallback(bindedFunction);
			EventDispatcher::addListener(myID, myNotifyCallback);
		}

		void Reaction::UnsubscribeFromCorrespondingEvent()
		{
			EventDispatcher::removeListener(myID, myNotifyCallback);
			delete myNotifyCallback; myNotifyCallback = nullptr;
		}

		/*
		// ------------------------------------
		// -------- ThrowBoneReaction ---------
		// ------------------------------------
		RTTI_DEFINITIONS(ThrowBoneReaction);

		// Cloneable interface
		ThrowBoneReaction* ThrowBoneReaction::clone() const
		{
			return new ThrowBoneReaction(*this);
		}

		// Signatures
		std::vector<Signature> ThrowBoneReaction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
			};
		}

		// Default constructor
		ThrowBoneReaction::ThrowBoneReaction() : Reaction(TypeIdClass())
		{
			SubscribeToCorrespondingEvent();
		}

		// Destructor
		ThrowBoneReaction::~ThrowBoneReaction()
		{
			UnsubscribeFromCorrespondingEvent();
		}

		// Subscribe to the event that this reaction corresponds with: ThrowBoneEvent
		void ThrowBoneReaction::SubscribeToCorrespondingEvent()
		{
			auto bindedFunction = BIND_FUNC(this, Reaction::Notify);
			myNotifyCallback = new EventCallback(bindedFunction);
			EventDispatcher::addListener<ThrowBoneEvent>(myNotifyCallback);
		}

		// Unsubscribe from the even that this reaction corresponds with: ThrowBoneEvent
		void ThrowBoneReaction::UnsubscribeFromCorrespondingEvent()
		{
			EventDispatcher::removeListener<ThrowBoneEvent>(myNotifyCallback);
			delete myNotifyCallback; myNotifyCallback = nullptr;
		}

		void ThrowBoneReaction::Notify(IEvent* payload)
		{
			// TODO: processing of payload somehow
			//payload->As<ThrowBoneEvent>()->color;
			//payload->As<ThrowBoneEvent>()->mass;
			update(CreateElapsedTime(0));
		}
		*/

	}
}