#pragma once
#include "Signature.h"
#include <vector>
#include <map>

namespace Fiea
{
	namespace GameEngine
	{
		class TypeManager
		{
		public:
			// Default constructor
			TypeManager() = default;

			// Diesable copy constructor and assignment operator
			TypeManager(const TypeManager& other) = delete;
			TypeManager& operator=(const TypeManager& other) = delete;

			// Register the prescribed attributes of a attributed type
			static void registerType(const size_t type, const std::vector<Signature>& sigs);

			// Manually register a type as a child of another type
			static void registerTypeAsChild(const size_t type, const size_t parentType, const std::vector<Signature>& childSigs);

			// Get the prescribed attributes of a attributed type
			static const std::vector<Signature>& getSignatures(const size_t type);

			// Clear the type registry
			static void clear();

		private:
			static std::map<size_t, std::vector<Signature>>* typeRegistry;
		};
	}
}