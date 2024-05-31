#include "pch.h"
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(AttributedWrapper);

		/*
		* Constructor for the AttributedWrapper.
		*/
		AttributedWrapper::AttributedWrapper() : rootTable(nullptr), currentSubTable(nullptr), maxDepth(0)
		{
			rootTable = new Scope();
			setCurrentSubTable(rootTable);
		}

		/*
		* Destructor for the AttributedWrapper.
		*/
		Fiea::GameEngine::AttributedWrapper::~AttributedWrapper()
		{
			delete rootTable;
		}
		
		/*
		* Set the root table of the AttributedWrapper.
		* @param scope: The scope to set as the root table.
		*/
		void Fiea::GameEngine::AttributedWrapper::setRootTable(Scope* scope)
		{
			rootTable = scope;
			setCurrentSubTable(rootTable);
		}

		/*
		* Returns the scope(the table) of the AttributedWrapper.
		* @return Scope*: The root scope of the AttributedWrapper.
		*/
		Scope* AttributedWrapper::getRootTable() const
		{
			return rootTable;
		}

		/*
		* Returns the current subtable of the AttributedWrapper.
		* @return Scope*: The current subtable of the AttributedWrapper.
		*/
		Scope* AttributedWrapper::getCurrentSubTable() const
		{
			return currentSubTable;
		}

		/*
		* Sets the current subtable of the AttributedWrapper.
		* @param scope: The scope to set as the current subtable.
		* @return Scope*: The current subtable of the AttributedWrapper.
		*/
		Scope* AttributedWrapper::setCurrentSubTable(Scope* scope)
		{
			previousSubTables.push(currentSubTable);
			currentSubTable = scope;
			return currentSubTable;
		}

		/*
		* Pops the current subtable of the AttributedWrapper.
		* @return Scope*: The current subtable of the AttributedWrapper.
		*/
		Scope* AttributedWrapper::popSubTable()
		{
			currentSubTable = previousSubTables.top();
			previousSubTables.pop();
			return currentSubTable;
		}

		// ================== RTTI Overrides ==================
		/**
		 * @brief RTTI override for Equals
		 * @param rhs
		 * @return true if equal, false otherwise
		*/
		bool AttributedWrapper::Equals(const RTTI* rhs) const
		{
			if (rhs == nullptr) return false;

			const AttributedWrapper* other = rhs->As<AttributedWrapper>();
			return rootTable == other->rootTable;
		}

		/**
		 * @brief RTTI override for ToString
		 * @return string representation of this Wrapper
		*/
		std::string AttributedWrapper::ToString() const
		{
			return "AttributedWrapper maxDepth=" + std::to_string(maxDepth);
		}
	}
}
