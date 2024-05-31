#pragma once
#include "Factory.h"
#include <memory>

namespace Fiea
{
	namespace GameEngine
	{
		template <class BaseType>
		class FactoryManager
		{
		public:
			static Factory<BaseType>* find(const std::string& key);
			static void add(std::unique_ptr<Factory<BaseType>>&& factory);
			static void remove(const std::string& key);
			static BaseType* create(const std::string& key);
			static void clear() { factories.clear(); }
		private:
			inline static std::unordered_map<std::string, std::unique_ptr<Factory<BaseType>>> factories;
		};
	}
}

#include "FactoryManager.inl"