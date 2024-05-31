#pragma once
#include "Attributed.h"
#include "IEvent.h"
#include "EventDispatcher.h"
#include "ActionList.h"
#include "TypeManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		// ------------------------------------
		// -------- Base Reaction -------------
		// ------------------------------------
		class Reaction final : public ActionList
		{
			RTTI_DECLARATIONS(Reaction, ActionList);
		public:
			Reaction();
			virtual ~Reaction();
			Reaction(const Reaction&);
			// Inherited from Attributed
			[[nodiscard]] virtual Reaction* clone() const;
			static std::vector<Signature> prescribedAttributes();
			virtual Datum& appendAuxiliaryAttribute(const std::string& key) override;
			// The callback to send to the event system
			virtual void Notify(IEvent* payload);
		public:
			virtual void SubscribeToCorrespondingEvent();
			virtual void UnsubscribeFromCorrespondingEvent();
		private:
			EventCallback* myNotifyCallback;
			RTTI::IdType myID;
			std::string myEventName;
		public: //public for debug purposes
			// ActionList* callbacks;
		};
		/*
		// ------------------------------------
		// -------- ThrowBoneReaction ---------
		// ------------------------------------
		class ThrowBoneReaction final : public Reaction
		{
			RTTI_DECLARATIONS(ThrowBoneReaction, Reaction);
		public:
			ThrowBoneReaction();
			~ThrowBoneReaction();

			// Cloneable interface
			[[nodiscard]] virtual ThrowBoneReaction* clone() const override;

			// Inerhtied from Attributed
			static std::vector<Signature> prescribedAttributes();
			// The callback to send the event system
			virtual void Notify(IEvent* payload) override;
		protected:
			virtual void SubscribeToCorrespondingEvent() override;
			virtual void UnsubscribeFromCorrespondingEvent() override;
		private:
		};
		*/
	}
}