#include "pch.h"
#include "Scope.h"

typedef Fiea::GameEngine::Datum::DataType DataType;

namespace Fiea 
{
	namespace GameEngine 
	{
		RTTI_DEFINITIONS(Scope);

		/*
		* Size constructor
		* @param size: size of the Scope
		*/
		Scope::Scope(size_t size) : mTable(), mOrder(), mSize(0), mCapacity(size), mParent(nullptr)
		{
			mTable.reserve(size);
			mOrder.reserve(size);
		}

		/*
		* Copy constructor, deep copy
		* @param other: reference to the Scope to be copied
		*/
		Scope::Scope(const Scope& other)
		{
			deepCopy(other);
		}

		/*
		* Move constructor
		* @param other: r-value reference to the Scope to be moved
		*/
		Scope::Scope(Scope&& other) noexcept :
			mTable(std::move(other.mTable)), 
			mOrder(std::move(other.mOrder)), 
			mSize(other.mSize), 
			mCapacity(other.mCapacity),
			mParent(other.mParent)
		{
			other.mSize = 0;
			other.mCapacity = 0;
			other.mParent = nullptr;
		}

		/*
		* Destructor
		*/
		Scope::~Scope()
		{
			clear();

			// Delete all internal Scopes
			for (auto& scope : mInternalScopes)
			{
				delete scope;
			}
		}

		/*
		* Clone the Scope
		* @return pointer to the cloned Scope
		*/
		Scope* Scope::clone() const
		{
			return new Scope(*this);
		}

		/*
		* Copy assignment operator, deep copy
		* @param rhs: reference to the Scope to be copied
		* @return reference to the copied Scope
		*/
		Scope& Scope::operator=(const Scope& rhs)
		{
			if (this != &rhs)
			{
				clear();
				deepCopy(rhs);
			}

			return *this;
		}

		/*
		* Move assignment operator
		* @param rhs: r-value reference to the Scope to be moved
		* @return reference to the moved Scope
		*/
		Scope& Scope::operator=(Scope&& rhs) noexcept
		{
			if (this != &rhs)
			{
				clear();

				mTable = std::move(rhs.mTable);
				mOrder = std::move(rhs.mOrder);
				mSize = rhs.mSize;
				mCapacity = rhs.mCapacity;
				mParent = rhs.mParent;

				rhs.mSize = 0;
				rhs.mCapacity = 0;
				rhs.mParent = nullptr;
			}

			return *this;
		}

		/*
		* Bracket operator
		* @param index: index of the Datum to be accessed
		* @return Datum at the specified index
		*/
		Datum& Scope::operator[](const size_t index)
		{
			if (index >= mSize)
				throw std::out_of_range("Index out of range");

			Datum& ref = *mOrder[index];
			return ref;
		}

		/*
		* Wrapper to Append for synactic convenience
		* @param key: key of the Datum to be appended
 		*/
		Datum& Scope::operator[](const std::string& key)
		{
			return append(key);
		}

		/*
		* Equality operator
		* @param rhs: reference to the Scope to be compared
		* @return true if the two Scopes are equal, false otherwise
		*/
		bool Scope::operator==(const Scope& rhs) const
		{
			if (mSize != rhs.mSize)
			{
				return false;
			}

			for (size_t i = 0; i < mSize; i++)
			{
				// Check if the keys are the same
				if (mOrder[i]->getDataType() != rhs.mOrder[i]->getDataType())
				{
					return false;
				}

				// Check if the Datum is a table
				if (mOrder[i]->getDataType() == DataType::TABLE)
				{
					// Check if the size of the Datum is the same
					if (mOrder[i]->getSize() != rhs.mOrder[i]->getSize())
					{
						return false;
					}

					// Check if the Scopes are the same
					for (size_t j = 0; j < mOrder[i]->getSize(); j++)
					{
						if (*mOrder[i]->get<Scope*>(j) != *rhs.mOrder[i]->get<Scope*>(j))
						{
							return false;
						}
					}
				}
				else
				{
					// Check if the Datum is the same
					if (*mOrder[i] != *rhs.mOrder[i])
					{
						return false;
					}
				}
			}

			return true;
		}

		/*
		* Inequality operator
		* @param rhs: reference to the Scope to be compared
		* @return true if the two Scopes are not equal, false otherwise
		*/
		bool Scope::operator!=(const Scope& rhs) const
		{
			return !operator==(rhs);
		}

		/*
		* Get the size of the Scope
		* @return size of the Scope
		*/
		size_t Scope::getSize() const
		{
			return mSize;
		}

		/*
		* Get the capacity of the Scope
		* @return capacity of the Scope
		*/
		size_t Scope::getCapacity() const
		{
			return mCapacity;
		}

		/*
		* Get the parent Scope
		* @return pointer to the parent Scope
		*/
		Scope* Scope::getParent() const
		{
			return mParent;
		}

		/*
		* Find the Datum associated with the key and returns the address of the Datum
		* @param key: key of the Datum to be found
		* @return address of the Datum associated with the key
		*/
		Datum* Scope::find(const std::string& key)
		{
			for (auto& pair : mTable) 
			{
				if (pair.first == key)
				{
					return &pair.second;
				}
			}

			return nullptr;
		}

		/*
		* Const version of find
		* @param key: key of the Datum to be found
		* @return address of the Datum associated with the key
		*/
		const Datum* Scope::find(const std::string& key) const
		{
			for (auto& pair : mTable)
			{
				if (pair.first == key)
				{
					return &pair.second;
				}
			}

			return nullptr;
		}

		/*
		* Takes the constant address of a Scope and index at which the Scope was found, and returns the Datum pointer
		* @param child: constant address to the child Scope to be found
		* @param index: index at which the child Scope was found
		* @return address of the Datum associated with the child Scope
		*/
		Datum* Scope::findContainedScope(const Scope* child, size_t& index)
		{
			for (size_t i = 0; i < mSize; i++)
			{
				if (mOrder[i]->getDataType() == DataType::TABLE)
				{
					for (size_t j = 0; j < mOrder[i]->getSize(); j++)
					{
						if (mOrder[i]->get<Scope*>(j) == child)
						{
							index = i;
							return mOrder[i];
						}
					}
				}
			}

			return nullptr;
		}

		/*
		* Takes the constant address of a Scope and index at which the Scope was found, and returns the Datum pointer
		* @param child: constant address to the child Scope to be found
		* @param index: index of the Datum in the mOrder vector
		* @param datumIndex: index of the child Scope in the Datum
		* @return address of the Datum associated with the child Scope
		*/
		Datum* Scope::findContainedScope(const Scope* child, size_t& index, size_t& datumIndex)
		{
			for (size_t i = 0; i < mSize; i++)
			{
				if (mOrder[i]->getDataType() == DataType::TABLE)
				{
					for (size_t j = 0; j < mOrder[i]->getSize(); j++)
					{
						if (mOrder[i]->get<Scope*>(j) == child)
						{
							index = i;
							datumIndex = j;
							return mOrder[i];
						}
					}
				}
			}

			return nullptr;
		}

		/*
		* Search for the Datum in this Scope and its ancestors, 
		* @param key: key of the Datum to be found
		* @param scope: pointer to the Scope where the Datum is found
		* @return address of the Datum associated with the key
		*/
		Datum* Scope::search(const std::string key, Scope** scope)
		{
			Datum* result = find(key);

			if (result == nullptr)
			{
				if (mParent != nullptr)
				{
					result = mParent->search(key, scope);
				}
			}
			else if (scope != nullptr)
			{
				*scope = this;
			}

			return result;
		}

		/*
		* Const version of search
		* @param key: key of the Datum to be found
		* @param scope: pointer to the Scope where the Datum is found
		* @return address of the Datum associated with the key
		*/
		const Datum* Scope::search(const std::string key, const Scope** scope) const
		{
			const Datum* result = find(key);

			if (result == nullptr)
			{
				if (mParent != nullptr)
				{
					result = mParent->search(key, scope);
				}
			}
			else if(scope != nullptr)
			{
				*scope = this;
			}

			return result;
		}

		/*
		* Append a new Datum to the Scope, if it already exists, return the existing Datum
		* @param key: key of the Datum to be appended
		* @return reference to the appended Datum
		*/
		Datum& Scope::append(const std::string& key)
		{
			if (auto search = mTable.find(key); search != mTable.end())
			{
				return search->second;
			}
			else 
			{
				Datum datum;
				mTable[key] = datum;

				mOrder.push_back(&mTable[key]);
				
				mSize++;
				mCapacity = mSize < mCapacity ? mCapacity : mSize;

				return mTable[key];
			}
		}

		/*
		* Append a new Datum whose data type is TABLE(Scope), if the key already exists, reuse the existing Datum
		* Otherwise, create a new Datum and a new Scope and append the Scope to the Datum
		* @param key: key of the Datum to be appended
		* @return reference to the appended Scope
		*/
		Scope& Scope::appendScope(const std::string& key)
		{
			Datum& datum = append(key);

			if (datum.getDataType() == DataType::TABLE || datum.getDataType() == DataType::UNKNOWN)
			{
				Scope* scope = new Scope();
				datum.push_back(scope);
				scope->setParent(this);

				// For memory management in desctructor
				mInternalScopes.push_back(scope);

				return *scope;
			}
			else
			{
				throw std::invalid_argument("Cannot append a scope to a non-table Datum");
			}
		}

		/*
		* Adopt a child Scope using the specified key
		* @param child: reference to the child Scope to be adopted
		* @param key: key of the child Scope to be adopted
		*/
		void Scope::adopt(Scope& child, const std::string& key)
		{
			// If the child is an ancestor of this, throw exception
			if (child.isAncestorOf(*this))
			{
				throw std::invalid_argument("Cannot adopt an ancestor");
			}

			// If the child is this, throw exception
			if (&child == this)
			{
				throw std::invalid_argument("Cannot adopt itself");
			}

			// If the child is a child of this, orphan the child
			if (child.isChildOf(*this) && child.getParent() != this)
			{
				child.orphan();
			}

			Datum& datum = append(key);

			if (datum.getDataType() == DataType::TABLE || datum.getDataType() == DataType::UNKNOWN)
			{
				Scope* scope = &child;
				datum.push_back(scope);
			}
			else
			{
				throw std::invalid_argument("Cannot append a scope to a non-table Datum");
			}
			child.setParent(this);
		}

		/*
		* Adopt a child Scope using the specified key and also transfer ownership
		* @param child: reference to the child Scope to be adopted
		* @param key: key of the child Scope to be adopted
		*/
		void Scope::adopt(Scope*&& child, const std::string& key)
		{
			// If the child is an ancestor of this, throw exception
			if (child->isAncestorOf(*this))
			{
				throw std::invalid_argument("Cannot adopt an ancestor");
			}

			// If the child is this, throw exception
			if (child == this)
			{
				throw std::invalid_argument("Cannot adopt itself");
			}

			// If the child is a child of this, orphan the child
			if (child->isChildOf(*this) && child->getParent() != this)
			{
				child->orphan();
			}

			Datum& datum = append(key);

			if (datum.getDataType() == DataType::TABLE || datum.getDataType() == DataType::UNKNOWN)
			{
				Scope* scope = std::move(child);
				datum.push_back(scope);
				scope->setParent(this);

				// For memory management in desctructor
				mInternalScopes.push_back(scope);
			}
			else
			{
				throw std::invalid_argument("Cannot append a scope to a non-table Datum");
			}
		}

		/*
		* Clear the data stored in the Scope recursively
		*/
		void Scope::clear()
		{
			// Remove child scopes
			for (auto& datum : mOrder)
			{
				if (datum->getDataType() == DataType::TABLE) 
				{
					for (size_t i = 0; i < datum->getSize(); i++)
					{
						datum->get<Scope*>(i)->clear();
					}
				}
			}

			mParent = nullptr;
			//mTable.clear();
			//mOrder.clear();
			mSize = 0;
			mCapacity = 0;
		}

		// ========================  RTTI overrides ======================== 

		std::string Scope::ToString() const
		{
			return "Scope";
		}

		bool Scope::Equals(const RTTI* other) const
		{
			if (this == other)
			{
				return true;
			}

			if (other == nullptr)
			{
				return false;
			}

			const Scope* scope = other->As<Scope>();
			return scope != nullptr ? operator==(*scope) : false;
		}



		// ======================== Helper Functions ========================
		
		// Helper function to perform deep copy
		void Scope::deepCopy(const Scope& other)
		{
			mTable.clear();
			mOrder.clear();

			// Reserve memory if the other Scope has memory allocated
			if (other.mCapacity > 0)
			{
				mTable.reserve(other.mCapacity);
				mOrder.reserve(other.mCapacity);
			}

			// Initialize the mOrder vector if the other Scope has Datum in it
			if (other.mSize > 0)
			{
				mOrder.resize(other.mSize);
			}

			for (auto& pair : other.mTable)
			{
				Datum datum(pair.second); 

				// If the Datum is a table, perform deep copy
				if (datum.getDataType() == DataType::TABLE)
				{
					for (size_t i = 0; i < datum.getSize(); i++)
					{
						Scope* scope = datum.get<Scope*>(i);
						datum.set(i, scope);

						// If we don't set the parent, the parent will be the original scope we copied from
						scope->setParent(this);
					}
				}

				mTable[pair.first] = datum;
				size_t index = other.indexOf(pair.first);
				
				//mOrder.push_back(&mTable[pair.first]);
				mOrder[index] = &mTable[pair.first];
			}

			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mParent = nullptr;		// Parent is not copied
		}

		// Helper function to check ancestor
		bool Scope::isAncestorOf(const Scope& other) const
		{
			// If the other parent pointer is null, return false
			if (other.mParent == nullptr)
			{
				return false;
			}

			// If the other parent pointer is not null, chekck if the parent is the same as this
			if (other.mParent == this)
			{
				return true;
			}

			// If the other parent pointer is not this, check if the parent of the parent is the same as this recursively
			return isAncestorOf(*other.mParent);
		}

		// Helper function to check if the scope is a child of another scope
		bool Scope::isChildOf(const Scope& other) const
		{
			// If the parent pointer is null, return false
			if (mParent == nullptr)
			{
				return false;
			}

			// If the parent pointer is not null, chekck if the parent is the same as the other
			if (mParent == &other)
			{
				return true;
			}

			// If the parent pointer is not the other, check if the parent of the parent is the same as the other recursively
			return mParent->isChildOf(other);
		}

		/*
		* Orphan the Scope
		*/
		Scope* Scope::orphan()
		{
			// Remove the Scope from the parent's Datum
			for (auto& pair : mParent->mTable)
			{
				for (size_t i = 0; i < pair.second.getSize(); i++)
				{
					if (pair.second.getDataType() == DataType::TABLE && pair.second.get<Scope*>(i) == this)
					{
						pair.second.remove(i);
						break;
					}
				}
			}
			mParent = nullptr;
			return this;
		}

		/*
		* Set the parent Scope
		* @param parent: pointer to the parent Scope
		*/
		void Scope::setParent(Scope* parent)
		{
			mParent = parent;
		}

		/*
		* Helper function to return the index of a Datum in the Scope
		* @param key: key of the Datum to be found
		* @return index of the Datum in the mOrder vector
		*/
		size_t Scope::indexOf(const std::string& key) const
		{
			for (auto& pair : mTable)
			{
				// If the key is found, found the index in the mOrder vector
				if (pair.first == key)
				{
					for (size_t i = 0; i < mSize; i++)
					{
						if (mOrder[i] == &pair.second)
						{
							return i;
						}
					}
				}
			}

			throw std::invalid_argument("Index of the datum not found");
		}
	}
}