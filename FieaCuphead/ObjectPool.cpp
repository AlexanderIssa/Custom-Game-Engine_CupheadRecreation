#include "ObjectPool.h"
#include "IntegerHandler.h"
#include "FloatHandler.h"
#include "StringHandler.h"
#include "Vector4Handler.h"
#include "Matrix4Handler.h"
#include "AttributedHandler.h"
#include "GameObjectManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief Constructor for ObjectPool
		* @param fileName: The name of the file to parse
		* @param poolSize: The size of the object pool [default = 5]
		*/
		ObjectPool::ObjectPool(const std::string& fileName, size_t capacity) : 
			mFileName(fileName), wrapper(nullptr), tableParser(nullptr), source(nullptr), mPoolSize(0), mCapacity(capacity)
		{
			wrapper = new AttributedWrapper();
			tableParser = new ParseCoordinator(*wrapper);

			// Add handlers for parsing different types
			tableParser->addHandler(std::make_unique<IntegerHandler>());
			tableParser->addHandler(std::make_unique<FloatHandler>());
			tableParser->addHandler(std::make_unique<StringHandler>());
			tableParser->addHandler(std::make_unique<Vector4Handler>());
			tableParser->addHandler(std::make_unique<Matrix4Handler>());
			tableParser->addHandler(std::make_unique<AttributedHandler>());

			// Parse the file
			bool deserialized = tableParser->deserializeObjectFromFile(mFileName);
			if(!deserialized)
			{
				throw std::invalid_argument("Failed to deserialize object from file");
			}

			// Get the source object
			Scope* root = wrapper->getRootTable();
			source = (*root)[0].get<Scope*>(0)->As<GameObject>(); // Try to get the first JSON object as a GameObject
			if(source == nullptr)
			{
				throw std::invalid_argument("Failed to get source object");
			}

			pooledGameObjects.reserve(mPoolSize);
			FillThePool();
		}

		/*
		* @brief Constructor for ObjectPool
		* @param gameObject: The GameObject to use as the source
		* @param poolSize: The size of the object pool [default = 5]
		*/
		ObjectPool::ObjectPool(GameObject* gameObject, size_t capacity) :
			mFileName(""), wrapper(nullptr), tableParser(nullptr), source(nullptr), mPoolSize(0), mCapacity(capacity)
		{
			if(gameObject == nullptr)
			{
				throw std::invalid_argument("GameObject cannot be null");
			}

			source = gameObject;

			pooledGameObjects.reserve(mPoolSize);
			FillThePool();
		}

		/*
		* @brief Destructor for ObjectPool
		*/
		ObjectPool::~ObjectPool()
		{
			delete tableParser;
			delete wrapper;
		}

		/*
		* @brief Function to get a pooled object
		* @return GameObject*: The pooled object
		*/
		GameObject* ObjectPool::GetPooledObject()
		{
			for (auto& obj : pooledGameObjects)
			{
				if (!obj->enabled)
				{
					obj->alive = true;
					obj->enabled = true;
					return obj;
				}
			}

			GameObject* obj = AddObjectToPool();
			obj->alive = true;
			obj->enabled = true;
			return obj;
		}

		//========================== Helper Functions ==========================//

		/*
		* @brief Helper function to add an object to the pool
		*/
		GameObject* ObjectPool::AddObjectToPool()
		{
			if (source == nullptr)
			{
				throw std::runtime_error("Source object is null");
			}

			GameObject* clone = source->clone();
			clone->name = std::string("(Clone)") + source->name + std::string("_") + std::to_string(mPoolSize);
			clone->enabled = false;
			clone->alive = true;

			// Add the clone to the world
			GameObjectManager::Add(clone);

			pooledGameObjects.push_back(clone);
			
			mPoolSize++;
			mCapacity = mCapacity < mPoolSize ? mPoolSize : mCapacity;

			return clone;	
		}

		/*
		* @brief Helper function to fill the object pool
		*/
		void ObjectPool::FillThePool()
		{
			for (int i = 0; i < mCapacity; i++)
			{
				AddObjectToPool();
			}
		}
	}
}