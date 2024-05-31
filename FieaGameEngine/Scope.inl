#pragma
#include "Scope.h"

typedef Fiea::GameEngine::Datum::DataType DataType;

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* Search for a Scope in this Scope and its ancestors by type
		* @return - A pointer to the first found Scope, nullptr if not found
		*/
		template<typename T>
		T* Scope::searchScope()
		{
			T* result = nullptr;

			if constexpr (!std::is_base_of<Scope, T>::value)
			{
				throw std::invalid_argument("Cannot search for a non-Scope class");
			}			

			// Try to find the Scope in the current Scope
			for (size_t i = 0; i < mSize; i++)
			{
				if (mOrder[i]->getDataType() == DataType::TABLE)
				{
					for (size_t j = 0; j < mOrder[i]->getSize(); j++)
					{
						result = mOrder[i]->get<Scope*>(j)->As<T>();
						if (result != nullptr)
						{
							return result;
						}
					}
				}
			}

			// If not found, try to find the Scope in the parent Scope
			if (result == nullptr)
			{
				if (mParent != nullptr)
				{
					result = mParent->searchScope<T>();
				}
			}

			return result;
		}

		/*
		* Search for a parent Scope of a given type
		* @return - A pointer to the first found Scope, nullptr if not found
		*/
		template<typename T>
		T* Scope::searchParentScope()
		{
			if constexpr (!std::is_base_of<Scope, T>::value)
			{
				throw std::invalid_argument("Cannot search for a non-Scope class");
			}

			// If the parent is of the given type, return it
			if (mParent != nullptr)
			{
				if (mParent->As<T>() != nullptr)
				{
					return mParent->As<T>();
				}
				else
				{
					return mParent->searchParentScope<T>();
				}
			}

			// If we're in the root Scope, search for the Scope in the current Scope
			return nullptr;
		}

		/*
		* Append a new Scope to this Scope by type
		* @param key - The key to append the new Scope with
		* @return - A reference to the newly appended Scope
		*/
		template<typename T>
		Scope& Scope::appendScope(const std::string& key)
		{
			Datum& datum = append(key);

			if (datum.getDataType() == Datum::DataType::TABLE || datum.getDataType() == Datum::DataType::UNKNOWN)
			{
				if constexpr (!std::is_base_of<Scope, T>::value)
				{
					throw std::invalid_argument("Cannot append a non-Scope class to a Scope");
				}

				Scope* scopeChildClass = new T();
				datum.push_back(scopeChildClass);
				scopeChildClass->setParent(this);

				// For memory management in desctructor
				mInternalScopes.push_back(scopeChildClass);

				return *scopeChildClass;
			}
			else
			{
				throw std::invalid_argument("Cannot append a scope to a non-table Datum");
			}
		}
	}
}