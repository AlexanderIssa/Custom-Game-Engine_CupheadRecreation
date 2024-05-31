#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(Attributed);

		/*
		* Default constructor for the Attributed class
		*/
		Attributed::Attributed(IdType type) :auxiliaryAttributes(), selfPtr(this)
		{
			initializePrescribedAttributes(type);
		}

		/*
		* Copy constructor for the Attributed class
		* @param other - The Attributed object to copy from
		*/
		Attributed::Attributed(const Attributed& other) : Scope(other), selfPtr(this)
		{
			// Copy the auxiliary attributes
			for (auto& sig : other.auxiliaryAttributes)
			{
				auxiliaryAttributes.push_back(sig);
			}

			// Update the this pointer in the scope
			char* baseAddress = reinterpret_cast<char*>(this);
			RTTI** rttiPtr = reinterpret_cast<RTTI**>(baseAddress + offsetof(Attributed, selfPtr));
			find("this"s)->setStorage(rttiPtr, 1);
		}

		/*
		* Move constructor for the Attributed class
		* @param other - The Attributed object to move from
		*/
		Attributed::Attributed(Attributed&& other) noexcept : 
			Scope(std::move(other)), 
			auxiliaryAttributes(std::move(other.auxiliaryAttributes)), 
			selfPtr(this)
		{
			// Update the this pointer in the scope
			char* baseAddress = reinterpret_cast<char*>(this);
			RTTI** rttiPtr = reinterpret_cast<RTTI**>(baseAddress + offsetof(Attributed, selfPtr));
			find("this"s)->setStorage(rttiPtr, 1);
		}

		/*
		* Copy assignment operator for the Attributed class
		* @param other - The Attributed object to copy from
		*/
		Attributed& Attributed::operator=(const Attributed& rhs)
		{
			if (this != &rhs)
			{
				Scope::operator=(rhs);

				selfPtr = this;

				// Clear the auxiliary attributes
				auxiliaryAttributes.clear();

				// Copy the auxiliary attribute
				for (auto& sig : rhs.auxiliaryAttributes)
				{
					auxiliaryAttributes.push_back(sig);
				}

				// Update the this pointer in the scope
				char* baseAddress = reinterpret_cast<char*>(this);
				RTTI** rttiPtr = reinterpret_cast<RTTI**>(baseAddress + offsetof(Attributed, selfPtr));
				find("this"s)->setStorage(rttiPtr, 1);
			}
			return *this;
		}

		/*
		* Move assignment operator for the Attributed class
		* @param other - The Attributed object to move from
		*/
		Attributed& Attributed::operator=(Attributed&& rhs) noexcept
		{
			if (this != &rhs)
			{
				Scope::operator=(std::move(rhs));

				selfPtr = this;

				// Move the auxiliary attributes
				auxiliaryAttributes = std::move(rhs.auxiliaryAttributes);

				// Update the this pointer in the scope
				char* baseAddress = reinterpret_cast<char*>(this);
				RTTI** rttiPtr = reinterpret_cast<RTTI**>(baseAddress + offsetof(Attributed, selfPtr));
				find("this"s)->setStorage(rttiPtr, 1);
			}
			return *this;
		}

		/*
		* Equality operator for the Attributed class
		* @param other - The Attributed object to compare with
		*/
		bool Attributed::operator==(const Attributed& other) const
		{
			if (Scope::operator==(other))
			{
				return auxiliaryAttributes == other.auxiliaryAttributes;
			}
			else
			{
				return false;
			}
		}

		/*
		* Inequality operator for the Attributed class
		*/
		bool Attributed::operator!=(const Attributed& other) const
		{
			return !operator==(other);
		}	

		/*
		* Return true if the key refers to either a prescribed attribute or an attribute
		* @param key - The name of the attribute
		* @return - True if the key refers to either a prescribed attribute or an attribute
		*/
		bool Attributed::isAttribute(const std::string& key) const
		{
			return isPrescribedAttribute(key) || isAuxiliaryAttribute(key);
		}

		/*
		* Return true if the key refers to a prescribed attribute
		* @param key - The name of the attribute
		* @return - True if the key refers to a prescribed attribute
		*/
		bool Attributed::isPrescribedAttribute(const std::string& key) const
		{
			// If it is a auxiliary attribute, return false
			if (isAuxiliaryAttribute(key))
			{
				return false;
			}

			return find(key) != nullptr;
		}

		/*
		* Return true if the key refers to an auxiliary attribute
		*/
		bool Attributed::isAuxiliaryAttribute(const std::string& key) const
		{
			auto it = std::find_if
			(
				auxiliaryAttributes.begin(), auxiliaryAttributes.end(),
				[&key](const Signature& sig)
				{
					return sig.Name == key;
				}
			);

			return it != auxiliaryAttributes.end();
		}

		// Initialize the prescribed attributes of AttributedFoo
		void Attributed::initializePrescribedAttributes(size_t type)
		{
			// Get the base address of the object, size of char is 1 byte.
			// And offsetof macro returns the offset in bytes.
			char* baseAddress = reinterpret_cast<char*>(this);
			void* ptr = nullptr;

			std::vector<Signature> prescribedAttributes = TypeManager::getSignatures(type);

			for (auto& sig : prescribedAttributes)
			{	
				if (sig.Name == "this"s) // this pointer
				{
					ptr = baseAddress + offsetof(Attributed, selfPtr);
					append(sig.Name).setStorage(sig.Type, ptr, sig.Size);
				}
				else if (sig.Offset == 0) // internal
				{
					append(sig.Name);
				}
				else // external
				{
					ptr = baseAddress + sig.Offset;
					append(sig.Name).setStorage(sig.Type, ptr, sig.Size);
				}
			}
		}
	}
}