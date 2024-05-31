#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "RTTI.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Scope;

		class Datum 
		{
		public:
			// This enum class is used to define the type of data that the Datum will store.
			enum class DataType
			{
				UNKNOWN,
				INTEGER,
				FLOAT,
				STRING,
				VEC4,
				MAT4,
				POINTER,
				TABLE
			};

		private:
			// This union is used to store the data of the Datum that is defined during runtime. 
			union Data
			{
				std::vector<int>* intData;
				std::vector<float>* floatData;
				std::vector<std::string>* stringData;
				std::vector<glm::vec4>* vec4Data;
				std::vector<glm::mat4>* mat4Data;
				std::vector<Scope*>* tableData;
				std::vector<RTTI*>* pointerData;

				int* externalIntData;
				float* externalFloatData;
				std::string* externalStringData;
				glm::vec4* externalVec4Data;
				glm::mat4* externalMat4Data;
				Scope** externalTableData;
				RTTI** externalPointerData;
			} mData;

			bool isExternalStorage; // Flag to indicate external storage

			size_t mSize;
			size_t mCapacity;
			DataType mDataType;

			void destroyData();
			void copyData(const Datum& other);

			template<typename T>
			void allocateDataForType();

			glm::vec4 stringToVec4(const std::string& value);
			glm::mat4 stringToMat4(const std::string& value);

			template <typename T>
			bool compareScalarWithVector(std::vector<T> vec, const T& scalar) const;

			template<typename T>
			bool areArraysEqual(const T* arr1, size_t size1, const T* arr2, size_t size2) const;

			template<typename T>
			bool areArraysEqual(const std::vector<T> vec, size_t size1, const T* arr2, size_t size2) const;

		public:
			// Default constructor
			Datum(); 

			// Copy constructor
			Datum(const Datum& other); 

			// Move constructor
			Datum(Datum&& other) noexcept;

			// Destructor
			~Datum(); 

			// Copy assignment operator
			Datum& operator=(const Datum& rhs); 

			// Move assignment operator
			Datum& operator=(Datum&& rhs);

			// assignment operator as scalar
			template<typename T>
			Datum& operator=(const T& rhs);

			// Equality operator
			bool operator==(const Datum& other) const;

			// Inequality operator
			bool operator!=(const Datum& other) const;

			template<typename T>
			bool operator==(const T& scalar) const;

			template<typename T>
			bool operator!=(const T& scalar) const;

			// Syntactically simplified access to nested Scopes. 
			Scope& operator[](std::uint32_t index) { return *get<Scope*>(index); }

			// Adds an item to the container based on the type of T.
			template <typename T>
			void push_back(T value);

			// Adds an item to the container based on the type of T.
			template <typename T>
			void push_back_force(T value, size_t index = 0);

			// Removes an item from the container at the specified index.
			void remove(size_t index);

			// Removes the last item from the container.
			void pop();

			// Clears the data stored in the Datum.
			void clear();

			// Sets the value of the item at the specified index.
			template <typename T>
			void set(size_t index, T value);

			// Sets the value of the item at the specified index from a string.
			void setFromString(size_t index, const std::string& value);

			// Return the item with data type T at the specified index
			template<typename T>
			T& get(size_t index); // Non-const version

			// Return the const reference of the item with data type T at the specified index
			template <typename T>
			const T& get(size_t index) const;

			// Get the item as a string at the specified index
			std::string getAsString(size_t index) const;

			// Returns the number of items in the container.
			template<typename T>
			void setStorage(T* array, size_t size);
			void setStorage(DataType type, void* ptr, size_t size);

			DataType getDataType() const;
			size_t getSize() const;
			size_t getCapacity() const;
			void resize(size_t newSize);
		};
	}
}

#include "Datum.inl"
