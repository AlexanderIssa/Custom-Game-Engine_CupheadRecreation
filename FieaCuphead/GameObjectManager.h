#pragma once
#include "GameObject.h"
#include "GameClock.h"
#include <vector>
#include <unordered_map>

namespace Fiea
{
	namespace GameEngine
	{
		class GameObjectManager
		{
		public:

			GameObjectManager() = default;

			// Default destructor
			~GameObjectManager() = default;

			// Initialize the game object manager
			static void SetWorld(GameObject* world);

			// Add a game object to the pool
			static void Add(GameObject* gameObject);

			// Update the game objects
			static void Update(const GameTime& time);

		private:
			// Helper function to extract game objects from the world
			static void ExtractGameObjects(GameObject* gameObject);

			// Refresh the pools
			static void RefreshPools();

			inline static GameObject* world;
			inline static std::vector<GameObject*> activeGameObjects;
			inline static std::vector<GameObject*> inactiveGameObjects;
		};
	}
}