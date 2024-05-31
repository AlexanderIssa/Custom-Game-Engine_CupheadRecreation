#pragma once
#include "IEvent.h"
#include "glm/glm.hpp"

namespace Fiea
{
	namespace GameEngine
	{
		// gross code: an empty IEvent in the CupheadEvents namespace that can be used to send empty payloads
		namespace CupheadEvents
		{
			static IEvent EmptyIEvent;
		}

		class OnGameStartEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnGameStartEvent, IEvent);
		};

		class OnTauntStartEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnTauntStartEvent, IEvent);
		};

		class OnFightStartEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnFightStartEvent, IEvent);
		};

		class OnPeaSpawnEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnPeaSpawnEvent, IEvent);
		public:
			glm::vec4 pos;
		};

		class OnStartJumpEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnStartJumpEvent, IEvent);
		};

		class OnStartDashEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnStartDashEvent, IEvent);
		};

		class OnCupheadTakeDamageEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnCupheadTakeDamageEvent, IEvent);

		public:
			int health = 0;
		};

		class OnCupheadDieEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnCupheadDieEvent, IEvent);
		};

		class OnBossDefeatEvent final : public IEvent
		{
			RTTI_DECLARATIONS(OnBossDefeatEvent, IEvent);
		};
	}
}