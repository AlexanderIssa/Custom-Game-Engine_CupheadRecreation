#pragma once
#include <stack>
#include "ParseCoordinator.h"
#include "Scope.h"

namespace Fiea
{
	namespace GameEngine
	{
		class TableWrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(TableWrapper, ParseCoordinator::Wrapper);

		public:
			TableWrapper() = delete;
			TableWrapper(Scope* scope);

			Scope* getRootTable() const;
			Scope* getCurrentSubTable() const;
			Scope* setCurrentSubTable(Scope* scope);
			Scope* popSubTable();

			std::size_t maxDepth { 0 };

			// RTTI Overrides
			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

		private: 
			Scope* rootTable { nullptr };
			std::stack<Scope*> previousSubTables;
			Scope* currentSubTable { nullptr };
		};
	}
}