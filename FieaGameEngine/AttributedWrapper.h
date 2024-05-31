#pragma once
#include <stack>
#include "ParseCoordinator.h"
#include "Attributed.h"

namespace Fiea
{
	namespace GameEngine
	{
		class AttributedWrapper : public ParseCoordinator::Wrapper
		{
			RTTI_DECLARATIONS(AttributedWrapper, ParseCoordinator::Wrapper);

		public:
			AttributedWrapper();
			~AttributedWrapper();

			void setRootTable(Scope* scope);
			Scope* getRootTable() const;
			Scope* getCurrentSubTable() const;
			Scope* setCurrentSubTable(Scope* scope);
			Scope* popSubTable();

			std::size_t maxDepth { 0 };

			// RTTI Overrides
			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;

		private:
			Scope* rootTable{ nullptr };
			std::stack<Scope*> previousSubTables;
			Scope* currentSubTable{ nullptr };
		};
	}
}
