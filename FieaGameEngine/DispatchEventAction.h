#pragma once
#include "Action.h"
#include "IEvent.h"

namespace Fiea
{
	namespace GameEngine
	{
		// ------------------------------------
		// ---------- Base Action -------------
		// ------------------------------------
		class DispatchEventAction final : public Action
		{
			RTTI_DECLARATIONS(DispatchEventAction, Action);
		public:
			DispatchEventAction();
			~DispatchEventAction();


			// Will call an event EVERY update
			// Perhaps there can be another event action that calls an event on a delay
			virtual void update(const GameTime& time) override;

			// Inherited from Attributed
			static std::vector<Signature> prescribedAttributes();
			// Inherited from Action
			[[nodiscard]] virtual DispatchEventAction* clone() const override;
			virtual void onEnable() override;
			virtual void onDisable() override;
		private:
			std::string myEventName;
		private:
			IEvent* myEvent;
			IdType myID;
		};

		/*
		// ----------------------------------------
		// -------- ThrowBone EventAction ---------
		// ----------------------------------------
		class DispatchThrowBoneEvent final : public IDispatchEventAction
		{
			RTTI_DECLARATIONS(DispatchThrowBoneEvent, IDispatchEventAction);
		public:
		private:
			std::string color;
			float mass;
		};
		*/
	}
}