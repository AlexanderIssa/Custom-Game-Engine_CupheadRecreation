#pragma once
#include "FactoryManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* Find a factory in the factory manager
		* @param key: The key(class name) of the factory to be found
		* @return: The factory of the type if found, nullptr otherwise
		*/
		template<class BaseType>
		Factory<BaseType>* FactoryManager<BaseType>::find(const std::string& key)
		{
			Factory<BaseType>& ref= *factories[key];
			return dynamic_cast<Factory<BaseType>*>(&ref);
		}

		/*
		* Add a factory to the factory manager
		* @param factory: The factory of a type to be added
		*/
		template<class BaseType>
		void FactoryManager<BaseType>::add(std::unique_ptr<Factory<BaseType>>&& factory)
		{
			if (find(factory->className()) == nullptr)
			{
				factories[factory->className()] = std::move(factory);
			}
		}

		/*
		* Remove a factory from the factory manager
		* @param factory: The factory of a type to be removed
		*/
		template<class BaseType>
		void FactoryManager<BaseType>::remove(const std::string& key)
		{
			if (find(key) != nullptr)
			{
				factories.erase(key);
			}
		}

		/*
		* Create an object of the type specified by the key
		* @param key: The key(class name) of the object to be created
		* @return: The object of the type if created, nullptr otherwise
		*/
		template<class BaseType>
		inline BaseType* FactoryManager<BaseType>::create(const std::string& key)
		{
			if (find(key) == nullptr)
			{
				throw std::invalid_argument("Factory of the type not found");
			}

			return factories[key]->create();
		}
	}
}