#pragma once
#include "GameClock.h"
#include "RenderManager.h"
#include "Window.h"
#include "InputManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		using Millis = long long;

		class Game
		{
		public:
			Game() = default;
			~Game() = default;
			void Run();

		private:
			void Init();
			void LoadResources();
			void RegisterSignatures();
			void PollingEvent();

			void Update(const GameTime& time);

			void Render();
			void Cleanup();

		private:
			Window window{};
		};
	}
}