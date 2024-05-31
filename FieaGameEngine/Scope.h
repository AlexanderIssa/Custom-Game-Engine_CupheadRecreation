#pragma once
#include "RTTI.h"
#include "Datum.h"
#include <unordered_map>

namespace Fiea 
{
	namespace GameEngine 
	{
		class Scope : public RTTI 
		{
			RTTI_DECLARATIONS(Scope, RTTI);

		public:
			// Size constructor
			Scope(size_t size = 0);

			// Copy constructor
			Scope(const Scope& other);

			// Move constructor
			Scope(Scope&& other) noexcept;

			// Destructor override
			virtual ~Scope();

			// Cloneable interface
			[[nodiscard]] virtual Scope* clone() const;

			// Copy assignment operator
			Scope& operator=(const Scope& rhs);

			// Move assignment operator
			Scope& operator=(Scope&& rhs) noexcept;

			// Bracket operator
			Datum& operator[](const size_t index);

			// Wraps to Append
			Datum& operator[](const std::string& key);

			// Equality operator
			bool operator==(const Scope& rhs) const;

			// Inequality operator
			bool operator!=(const Scope& rhs) const;

			// Get the size of the scope
			size_t getSize() const;

			// Get the capacity of the scope
			size_t getCapacity() const;

			// Get the parent scope
			Scope* getParent() const;

			// Find the Datum associated with the key and returns the address of the Datum
			Datum* find(const std::string& key);

			// Const version of find
			const Datum* find(const std::string& key) const;

			// Takes the constant address of a Scope and returns the Datum pointer and index at which the Scope was found.
			Datum* findContainedScope(const Scope* child, size_t& index);

			// In case we also need the index of the scope in the Datum
			Datum* findContainedScope(const Scope* child, size_t& index, size_t& datumIndex);

			// Search for the datum in this scope and its ancestors
			Datum* search(const std::string key, Scope** scope = nullptr);

			// Const verion of search
			const Datum* search(const std::string key, const Scope** scope = nullptr) const;

			// Search for the target scope in this scope and its ancestors
			template<typename T>
			T* searchScope();

			// Serach for the target scope in the root parent
			template<typename T>
			T* searchParentScope();

			// Append a new datum to the scope, if it already exists, return the existing datum
			Datum& append(const std::string& key);

			// Apped a new datum whose data type is TABLE(Scope), if it already exists, return the existing Scope in the Datum
			Scope& appendScope(const std::string& key);

			// Append a new datum to the scope as its child, if it already exists, return the existing datum
			template<typename T>
			Scope& appendScope(const std::string& key);

			// Adopt a child scope using specified key
			void adopt(Scope& child, const std::string& key);

			// Adopt a child scope and transfer ownership to the parent scope
			void adopt(Scope*&& child, const std::string& key);

			// Clear all alocated memory recursively
			void clear();

			// RTTI Overrides
			std::string ToString() const override;
			bool Equals(const RTTI* rhs) const override;

		private:
			// Map of string key and Datum value
			std::unordered_map<std::string, Datum> mTable;

			// Vector of pointers to the datum in the map
			std::vector<Datum*> mOrder;

			// Pointer to the parent scope
			Scope* mParent;

			// size of the scope
			size_t mSize;

			// Capacity of the scope
			size_t mCapacity;

			// Helper vector to store internal scope pointers
			std::vector<Scope*> mInternalScopes;

			// Helper function to perform deep copy
			void deepCopy(const Scope& other);

			// Helper function to return the index of a datum in the scope
			size_t indexOf(const std::string& key) const;

		protected:
			// Set the parent scope
			void setParent(Scope* parent);

			// Orphan the scope
			Scope* orphan();

			// Helper function to check ancestor
			bool isAncestorOf(const Scope& other) const;

			// Helper function to check if the scope is a child of another scope
			bool isChildOf(const Scope& other) const;
		};
	}
}

#include "Scope.inl"