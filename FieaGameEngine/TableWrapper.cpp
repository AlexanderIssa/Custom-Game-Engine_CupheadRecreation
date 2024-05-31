#include "pch.h"
#include "TableWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(TableWrapper);
		
		/*
		* Constructor for TableWrapper that sets the root scope address.
		* @param scope: The root scope of the TableWrapper.
		*/
		TableWrapper::TableWrapper(Scope* scope) : rootTable(scope), currentSubTable(scope) {}

		/*
		* Returns the scope(the table) of the TableWrapper.
		* @return Scope*: The root scope of the TableWrapper.
		*/
		Scope* TableWrapper::getRootTable() const
		{
			return rootTable;
		}

		/*
		* Returns the current subtable of the TableWrapper.
		* @return Scope*: The current subtable of the TableWrapper.
		*/
		Scope* TableWrapper::getCurrentSubTable() const
		{
			return currentSubTable;
		}

		/*
		* Sets the current subtable of the TableWrapper.
		* @param scope: The scope to set as the current subtable.
		* @return Scope*: The current subtable of the TableWrapper.
		*/
		Scope* TableWrapper::setCurrentSubTable(Scope* scope)
		{
			previousSubTables.push(currentSubTable);
			currentSubTable = scope;
			return currentSubTable;
		}

		/*
		* Pops the current subtable of the TableWrapper.
		* @return Scope*: The current subtable of the TableWrapper.
		*/
		Scope* TableWrapper::popSubTable()
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
		bool TableWrapper::Equals(const RTTI* rhs) const
		{
			if (rhs == nullptr) return false;

			const TableWrapper* other = rhs->As<TableWrapper>();
			return rootTable == other->rootTable;
		}

		/**
		 * @brief RTTI override for ToString
		 * @return string representation of this Wrapper
		*/
		std::string TableWrapper::ToString() const
		{
			return "TableWrapper maxDepth=" + std::to_string(maxDepth);
		}
	}
}
