#pragma once
#include "GameObject.h"
#include "AttributedWrapper.h"
#include <vector>

namespace Fiea
{
	namespace GameEngine
	{
		class ObjectPool
		{
		public:
			// Default constructor does nothing
			ObjectPool() = delete;

			// Desirable constructor to create an object pool from a file
			ObjectPool(const std::string& fileName, size_t mPoolSize = 5);

			// Desirable constructor to create an object pool from other object
			ObjectPool(GameObject* gameObject, size_t mPoolSize = 5);

			// Destructor to clean up the object pool
			~ObjectPool();

			// Get a pooled object by name
			GameObject* GetPooledObject();

		private:
			GameObject* source;
			std::string mFileName;
			ParseCoordinator* tableParser;
			AttributedWrapper* wrapper;

			std::vector<GameObject*> pooledGameObjects;
			size_t mPoolSize;
			size_t mCapacity;

			void FillThePool();
			GameObject* AddObjectToPool();
		};
	}
}