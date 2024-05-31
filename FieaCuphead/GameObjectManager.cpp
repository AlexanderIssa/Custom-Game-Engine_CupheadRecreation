#include "GameObject.h"
#include "GameObjectManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief Constructor for the GameObjectManager
		* @param world The world object
		*/
		void GameObjectManager::SetWorld(GameObject* worldObj)
		{
			world = worldObj;

			// Extract every game object from the world
			// and add it to the pool
			ExtractGameObjects(world);

			// Enable the world
			for (auto& obj : inactiveGameObjects)
			{
				obj->enabled = true;
			}
		}

		/*
		* @brief Add a game object to the manager
		* @param gameObject The game object to add
		*/
		void GameObjectManager::Add(GameObject* gameObject)
		{
			if (world == nullptr)
			{
				throw std::runtime_error("World doesn't exist");
			}

			if (gameObject == nullptr)
			{
				throw std::invalid_argument("GameObject cannot be null");
			}
			
			world->addChild(gameObject, gameObject->name);
			ExtractGameObjects(gameObject);
		}

		/*
		* @brief Update the game objects
		* @param time The time object
		*/
		void GameObjectManager::Update(const GameTime& time)
		{
			RefreshPools();
			world->update(time);
			/*for (auto& obj : activeGameObjects)
			{
				obj->update(time);
			}*/
		}

		/*
		* @brief Helper function to extract game objects from a game object
		* @param gameObject The game object to extract
		*/
		void GameObjectManager::ExtractGameObjects(GameObject* gameObject)
		{
			if (gameObject == nullptr)
			{
				throw std::invalid_argument("GameObject cannot be null");
			}

			// Add the game object to the list
			gameObject->enabled ?
				activeGameObjects.push_back(gameObject) : inactiveGameObjects.push_back(gameObject);

			// Extract children
			Datum& childrenDatum = (*gameObject)["obj_children"];
			Scope& childernContainer = childrenDatum[0];

			// Add every child to the pool
			for (int i = 0; i < childernContainer.getSize(); i++)
			{
				// Get a list of children at this index
				Datum& childrenList = childernContainer[i];
				if (childrenList.getDataType() == Datum::DataType::TABLE)
				{
					for (int j = 0; j < childrenList.getSize(); j++)
					{
						GameObject* child = childrenList[j].As<GameObject>();
						if (child != nullptr)
						{
							// Add the child to the list
							child->enabled ?
								activeGameObjects.push_back(child) : inactiveGameObjects.push_back(child);

							// Recursively extract children
							ExtractGameObjects(child);
						}
					}
				}
			}		
		}

		/*
		* @brief Refresh the pools
		*/
		void GameObjectManager::RefreshPools()
		{
			// If an object is disabled, move it to the inactive pool
			for (auto& obj : activeGameObjects)
			{
				if (!obj->enabled || !obj->alive)
				{
					obj->onDisable();
					obj->enabled = false;
					inactiveGameObjects.push_back(obj);
				}
			}

			if (activeGameObjects.size() > 0)
			{
				activeGameObjects.erase(
					std::remove_if(
						activeGameObjects.begin(),
						activeGameObjects.end(),
						[](GameObject* obj) { return !obj->enabled || !obj->alive; }
					),
					activeGameObjects.end()
				);
			}
			
			// If an object is enabled, move it to the active pool
			for (auto& obj : inactiveGameObjects)
			{
				if (obj->enabled && obj->alive)
				{
					obj->onEnable();
					obj->enabled = true;
					activeGameObjects.push_back(obj);
				}
			}

			if (inactiveGameObjects.size() > 0)
			{
				inactiveGameObjects.erase(
					std::remove_if(
						inactiveGameObjects.begin(),
						inactiveGameObjects.end(),
						[](GameObject* obj) { return obj->enabled && obj->alive; }
					),
					inactiveGameObjects.end()
				);
			}
		}
	}
}