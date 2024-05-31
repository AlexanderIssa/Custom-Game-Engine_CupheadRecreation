#include "pch.h"
#include "TypeManager.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* Registers a new type with the type manager
		* @param type The type to register
		* @param sigs The signatures of the type
		*/
		void TypeManager::registerType(const size_t type, const std::vector<Signature>& sigs)
		{
			// Lazy initialization
			if (typeRegistry == nullptr) 
			{
				typeRegistry = new std::map<size_t, std::vector<Signature>>();
			}

			auto [it, success] = typeRegistry->insert({ type , sigs });
			if (!success)
			{
				throw::std::runtime_error("Type already registered");
			}
		}

		/*
		* Registers a new type as a child of another type manually
		* @param type The type to register
		* @param parent The parent type
		* @param childSigs The signatures of the type
		*/
		void TypeManager::registerTypeAsChild(const size_t type, const size_t parentType, const std::vector<Signature>& childSigs)
		{
			// Lazy initialization
			if (typeRegistry == nullptr)
			{
				throw::std::runtime_error("TypeManager is not initialized! Please call registerType() at least once!");
			}

			auto [it, success] = typeRegistry->insert({ type , childSigs});
			if (!success)
			{
				throw::std::runtime_error("Type already registered");
			}

			// Add child signatures to the parent signatures
			std::vector<Signature>& parentSigs = typeRegistry->at(parentType);
			std::vector<Signature>& sigs = typeRegistry->at(type);

			for (auto sig : parentSigs)
			{
				sigs.push_back(sig);
			}
		}

		/*
		* Get the signatures of a attributed type
		* @param type The type to get the signatures of
		* @return The signatures of the type
		*/
		const std::vector<Signature>& TypeManager::getSignatures(const size_t type)
		{
			if (typeRegistry == nullptr)
			{
				throw::std::runtime_error("TypeManager is not initialized! Please call registerType() at least once!");
			}
			return typeRegistry->at(type);
		}

		/*
		* Clear the type registry
		*/
		void TypeManager::clear()
		{
			if (typeRegistry != nullptr)
			{
				typeRegistry = nullptr;
			}
		}
		
		// Static initialization
		std::map<size_t, std::vector<Signature>>* TypeManager::typeRegistry;
	}
}