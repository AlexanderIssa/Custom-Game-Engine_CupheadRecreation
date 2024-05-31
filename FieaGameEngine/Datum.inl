#pragma once

#include "Datum.h"
#include <stdexcept>

namespace Fiea
{
	namespace GameEngine 
	{
        /*
        * Assignment operator for Datum as scalar
        * [Warning] This function will throw a logic_error if input type is not the same as current Datum type
        * @param rhs - The value of type T to assign to the Datum
        * @return The reference to the Datum
        */
        template<typename T>
        Datum& Datum::operator=(const T& rhs)
        {
            if (isExternalStorage)
                throw std::runtime_error("This operation is not supported when Datum has external storage");

            if (mDataType == DataType::UNKNOWN) 
            {
                destroyData();
                push_back(rhs);
            }
            else if constexpr (std::is_same<T, int>::value)
            {
                if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                destroyData();
                push_back(rhs);
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                destroyData();
                push_back(rhs);
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                destroyData();
                push_back(rhs);
            }
            else if constexpr (std::is_same<T, glm::vec4>::value) 
            {
                if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
				destroyData();
				push_back(rhs);
			}
			else if constexpr (std::is_same<T, glm::mat4>::value)
			{
				if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
				destroyData();
				push_back(rhs);
			}
            else if constexpr (std::is_same<T, Scope*>::value)
            {
				if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
				destroyData();
				push_back(rhs);
			}
            else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
            {
                if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
                destroyData();
                push_back(rhs);
            }
            else
            {
                throw std::logic_error("Unsupported type");
            }

            return *this;
        }

        /*
        * Comparison operator for Datum as scalar
        * @param scalar - The value of type T to compare with the Datum
        * @return True if the Datum is equal to the scalar, false otherwise
        */
        template <typename T>
        bool Datum::operator==(const T& scalar) const
        {
            if (mSize != 1) 
			{
				return false;
			}

            if (!isExternalStorage) 
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    if (mDataType != DataType::INTEGER) return false;
                    return compareScalarWithVector(*mData.intData, scalar);  
                }
                else if constexpr (std::is_same<T, float>::value)
				{
					if (mDataType != DataType::FLOAT) return false;
					return compareScalarWithVector(*mData.floatData, scalar);
				}
				else if constexpr (std::is_same<T, std::string>::value)
				{
					if (mDataType != DataType::STRING) return false;
					return compareScalarWithVector(*mData.stringData, scalar);
				}
				else if constexpr (std::is_same<T, glm::vec4>::value)
				{
					if (mDataType != DataType::VEC4) return false;
					return compareScalarWithVector(*mData.vec4Data, scalar);
				}
				else if constexpr (std::is_same<T, glm::mat4>::value)
				{
					if (mDataType != DataType::MAT4) return false;
					return compareScalarWithVector(*mData.mat4Data, scalar);
				}
                else if constexpr (std::is_same<T, Scope*>::value)
                {
                    if (mDataType != DataType::TABLE) return false;
                    return compareScalarWithVector(*mData.tableData, scalar);
                }
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
                    if (mDataType != DataType::POINTER) return false;
                    T t = reinterpret_cast<T>((*mData.pointerData)[0]);
                    std::vector<T> vec = { t };
                    return compareScalarWithVector(vec, scalar);
				}
				else
				{
					return false;
				}
            }
            else
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    // missing test coverage for the mismatched types vs. external storage
                    if (mDataType != DataType::INTEGER) return false;
                    return mData.externalIntData[0] == scalar;
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) return false;
                    return mData.externalFloatData[0] == scalar;
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) return false;
                    return mData.externalStringData[0] == scalar;
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) return false;
                    return mData.externalVec4Data[0] == scalar;
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) return false;
                    return mData.externalMat4Data[0] == scalar;
                }
                else if constexpr (std::is_same<T, Scope*>::value)
                {
					if (mDataType != DataType::TABLE) return false;
					return mData.externalTableData[0] == scalar;
				}
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
					if (mDataType != DataType::POINTER) return false;
					T t = reinterpret_cast<T>(mData.externalPointerData[0]);
					return t == scalar;
				}
                else
                {
                    return false;
                }
            }
        }

        /*
        * Comparison operator for Datum as scalar
        * @param scalar - The value of type T to compare with the Datum
        * @return True if the Datum is not equal to the scalar, false otherwise
        */
        template <typename T>
        bool Datum::operator!=(const T& scalar) const 
        {
            return !(*this == scalar);
        }

        /*
        * Add an item to the Datum
        * @param value - The value of type T to add to the Datum
        */
        template <typename T>
        void Datum::push_back(T value)
        {
            if (isExternalStorage)
                throw std::runtime_error("Datum has external storage so this operation is not allowed");

            // Instantiate the correct vector if it doesn't exist
            if (mDataType == DataType::UNKNOWN)
            {
                // Determine type and allocate
                allocateDataForType<T>();
            }

            // Add the item
            // constexpr because we want to check the type at compile time
            if constexpr (std::is_same<T, int>::value)
            {
                if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                mData.intData->push_back(value);
                mCapacity = mData.intData->capacity();
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                mData.floatData->push_back(value);
                mCapacity = mData.floatData->capacity();
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                mData.stringData->push_back(value);
                mCapacity = mData.stringData->capacity();
            }
            else if constexpr(std::is_same<T, glm::vec4>::value)
			{
				if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
				mData.vec4Data->push_back(value);
				mCapacity = mData.vec4Data->capacity();
			}
			else if constexpr (std::is_same<T, glm::mat4>::value)
			{
				if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
				mData.mat4Data->push_back(value);
				mCapacity = mData.mat4Data->capacity();
			}
            else if constexpr (std::is_same<T, Scope*>::value)
            {  
                if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
				mData.tableData->push_back(value);
				mCapacity = mData.tableData->capacity();
			}
            else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
            {
                if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
                mData.pointerData->push_back(value);
                mCapacity = mData.pointerData->capacity();
			}
            else
            {
                throw std::logic_error("Unsupported type");
            }


            mSize++;
        }

        /*
        * Try to add and item to the Datum
        * If the Datum is external storage, this function will call set() instead
        * @param value - The value of type T to add to the Datum
        */
        template <typename T>
        void Datum::push_back_force(T value, size_t index)
        {
            if (isExternalStorage)
            {
				set(index, value);
			}
            else
            {
				push_back(value);
			}
		}


        /*
        * Set the value of the item at the specified index
        * @param index - The index of the item to set
        * @param value - The value of type T to set the item to
        */
        template <typename T>
        void Datum::set(size_t index, T value)
        {
            if (index >= mSize) 
                throw std::out_of_range("Index out of range");

            // Set the value
            // constexpr because we want to check the type at compile time
            if (!isExternalStorage) 
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                    (*mData.intData)[index] = value;
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                    (*mData.floatData)[index] = value;
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                    (*mData.stringData)[index] = value;
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
                    (*mData.vec4Data)[index] = value;
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
                    (*mData.mat4Data)[index] = value;
                }
                else if constexpr (std::is_same<T, Scope*>::value)
                {
					if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
					(*mData.tableData)[index] = value;
				}
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
                    if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
                    (*mData.pointerData)[index] = value;
                }
                else
                {
                    throw std::invalid_argument("Unsupported type");
                }
            }
            else 
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    // missing test coverage for the mismatched types vs. external storage
                    if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                    (mData.externalIntData)[index] = value;
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                    (mData.externalFloatData)[index] = value;
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                    (mData.externalStringData)[index] = value;
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
                    (mData.externalVec4Data)[index] = value;
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
                    (mData.externalMat4Data)[index] = value;
                }
                else if constexpr (std::is_same<T, Scope*>::value)
                {
					if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
					(mData.externalTableData)[index] = value;
				}
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
					if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
					(mData.externalPointerData)[index] = value;
				}
                else
                {
                    throw std::invalid_argument("Unsupported type");
                }
            }
        }


        /*
        * Get the value of the item at the specified index
        * @param index - The index of the item to get
        * @return The const reference of the item at the specified index
        */
        template <typename T>
        const T& Datum::get(size_t index) const
        {
            // Guard against invalid access
            if (index >= mSize) throw std::out_of_range("Index out of range");

            // Get the item of Type T
            // constexpr because we want to check the type at compile time
            if(!isExternalStorage)
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                    return (*mData.intData)[index];
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                    return (*mData.floatData)[index];
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                    return (*mData.stringData)[index];
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
                    return (*mData.vec4Data)[index];
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
                    return (*mData.mat4Data)[index];
                }
                else if constexpr (std::is_same<T, Scope*>::value)
                {
					if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
					return (*mData.tableData)[index];
				}
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
                    if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
                    T& ref = reinterpret_cast<T&>((*mData.pointerData)[index]);
                    return ref;
                }
                else
                {
                    throw std::invalid_argument("Unsupported type");
                }
            }
            else 
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                    return (mData.externalIntData)[index];
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                    return (mData.externalFloatData)[index];
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                    return (mData.externalStringData)[index];
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
                    return (mData.externalVec4Data)[index];
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
                    return (mData.externalMat4Data)[index];
                }
                else if constexpr (std::is_same<T, Scope*>::value)
                {
                    if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
                    return (mData.externalTableData)[index];
                }
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
					if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
					T& ref = reinterpret_cast<T&>(mData.externalPointerData[index]);
					return ref;
				}
                else
                {
                    throw std::invalid_argument("Unsupported type");
                }
            }
           
        }

        /*
        * Get the item at the specified index
        * @param index - The index of the item to get
        * @return The reference of the item at the specified index
        */
        template <typename T>
        T& Datum::get(size_t index)
        {
            // Guard against invalid access
            if (index >= mSize) throw std::out_of_range("Index out of range");

            // Get the item of Type T
            // constexpr because we want to check the type at compile time
            if (!isExternalStorage)
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                    return (*mData.intData)[index];
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                    return (*mData.floatData)[index];
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                    return (*mData.stringData)[index];
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
                    return (*mData.vec4Data)[index];
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
                    return (*mData.mat4Data)[index];
                }
                else if constexpr (std::is_same<T, Scope*>::value) 
                {
                    if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
                    return (*mData.tableData)[index];
                }
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
                    if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
                    T& ref = reinterpret_cast<T&>((*mData.pointerData)[index]);
                    return ref;
				}
                else
				{
					throw std::invalid_argument("Unsupported type");
				}
            }
            else
            {
                if constexpr (std::is_same<T, int>::value)
                {
                    if (mDataType != DataType::INTEGER) throw std::logic_error("Type mismatch");
                    return (mData.externalIntData)[index];
                }
                else if constexpr (std::is_same<T, float>::value)
                {
                    if (mDataType != DataType::FLOAT) throw std::logic_error("Type mismatch");
                    return (mData.externalFloatData)[index];
                }
                else if constexpr (std::is_same<T, std::string>::value)
                {
                    if (mDataType != DataType::STRING) throw std::logic_error("Type mismatch");
                    return (mData.externalStringData)[index];
                }
                else if constexpr (std::is_same<T, glm::vec4>::value)
                {
                    if (mDataType != DataType::VEC4) throw std::logic_error("Type mismatch");
                    return (mData.externalVec4Data)[index];
                }
                else if constexpr (std::is_same<T, glm::mat4>::value)
                {
                    if (mDataType != DataType::MAT4) throw std::logic_error("Type mismatch");
                    return (mData.externalMat4Data)[index];
                }
                else if constexpr (std::is_same<T, Scope*>::value)
                {
					if (mDataType != DataType::TABLE) throw std::logic_error("Type mismatch");
					return (mData.externalTableData)[index];
				}
                else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                {
					if (mDataType != DataType::POINTER) throw std::logic_error("Type mismatch");
					T& ref = reinterpret_cast<T&>(mData.externalPointerData[index]);
					return ref;
				}
                else
				{
					throw std::invalid_argument("Unsupported type");
				}
            }
        }

        template<typename T>
        void Datum::setStorage(T* array, size_t size) 
        {
            if (mDataType != DataType::UNKNOWN)
            {
                if (isExternalStorage)
                {
                    if constexpr (std::is_same<T, int>::value)
                    {
                        if (mDataType != DataType::INTEGER) 
                            throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                    }
                    else if constexpr (std::is_same<T, float>::value)
                    {
                        if (mDataType != DataType::FLOAT)
                            throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                    }
                    else if constexpr (std::is_same<T, std::string>::value)
                    {
                        if (mDataType != DataType::STRING)
                            throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                    }
                    else if constexpr (std::is_same<T, glm::vec4>::value)
                    {
                        if (mDataType != DataType::VEC4) 
                            throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                    }
                    else if constexpr (std::is_same<T, glm::mat4>::value)
                    {
                        if (mDataType != DataType::MAT4) 
                            throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                    }
                    else if constexpr (std::is_same<T, Scope*>::value)
                    {
						if (mDataType != DataType::TABLE) 
							throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
					}
                    else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
                    {
						if (mDataType != DataType::POINTER) 
							throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
					}
                    else
                    {
                        throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                    }
                }
                else
                {
                    throw std::logic_error("Datum has already been initialized so setStorage is not allowed");
                }
            }

            // Set the size and capacity
            mSize = mCapacity = size;

            // Mark storage as external
            isExternalStorage = true; 

            // Determine the type and set the data pointer accordingly
            if constexpr (std::is_same<T, int>::value)
            {
                mDataType = DataType::INTEGER;
                mData.externalIntData = array;
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                mDataType = DataType::FLOAT;
                mData.externalFloatData = array;
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                mDataType = DataType::STRING;
                mData.externalStringData = array;
            }
            else if constexpr (std::is_same<T, glm::vec4>::value)
            {
                mDataType = DataType::VEC4;
                mData.externalVec4Data = array;
            }
            else if constexpr (std::is_same<T, glm::mat4>::value)
            {
                mDataType = DataType::MAT4;
                mData.externalMat4Data = array;
            }
            else if constexpr (std::is_same<T, Scope*>::value)
            {
                mDataType = DataType::TABLE;
                mData.externalTableData = array;
            }
            else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
            {
                mDataType = DataType::POINTER;
				mData.externalPointerData = reinterpret_cast<RTTI**>(array);
            }
            else 
            {
                throw std::invalid_argument("Unsupported type for SetStorage");
            }
        }

        // ================== Helper Functions ==================

        template<typename T>
        void Datum::allocateDataForType()
        {
            // constexpr because we want to check the type at compile time
            if constexpr (std::is_same<T, int>::value)
            {
                mData.intData = new std::vector<int>();
                mDataType = DataType::INTEGER;
            }
            else if constexpr (std::is_same<T, float>::value)
            {
                mData.floatData = new std::vector<float>();
                mDataType = DataType::FLOAT;
            }
            else if constexpr (std::is_same<T, std::string>::value)
            {
                mData.stringData = new std::vector<std::string>();
                mDataType = DataType::STRING;
            }
		    else if constexpr (std::is_same<T, glm::vec4>::value)
            {
                mData.vec4Data = new std::vector<glm::vec4>();
                mDataType = DataType::VEC4;
            }
            else if constexpr (std::is_same<T, glm::mat4>::value)
			{
				mData.mat4Data = new std::vector<glm::mat4>();
				mDataType = DataType::MAT4;
			}
            else if constexpr (std::is_same<T, Scope*>::value)
            {
				mData.tableData = new std::vector<Scope*>();
				mDataType = DataType::TABLE;
			}
            else if constexpr (std::is_pointer<T>::value && std::is_base_of<RTTI, typename std::remove_pointer<T>::type>::value)
            {
                mData.pointerData = new std::vector<RTTI*>();
                mDataType = DataType::POINTER;
            }
            else
            {
                throw std::logic_error("Unsupported type");
            }
        }

        // Helper function to compare two arrays
        template<typename T>
        bool Datum::areArraysEqual(const T* arr1, size_t size1, const T* arr2, size_t size2) const
        {
            if (size1 != size2) 
            {
                return false; // Arrays of different sizes are automatically not equal
            }

            for (size_t i = 0; i < size1; i++) 
            {
                if (arr1[i] != arr2[i]) 
                {
                    return false; // Found elements that are not equal
                }
            }
            return true; // All elements are equal
        }

        // Helper function to compare array and vector
        template<typename T>
        bool Datum::areArraysEqual(const std::vector<T> vec, size_t size1, const T* arr2, size_t size2) const
        {
            if (size1 != size2)
            {
                return false; // Arrays of different sizes are automatically not equal
            }

            for (size_t i = 0; i < size1; i++)
            {
                if (vec[i] != arr2[i])
                {
					return false; // Found elements that are not equal
				}
			}

            return true; // All elements are equal
        }

        // Helper function to compare scalar with a vector
        template <typename T>
        bool Datum::compareScalarWithVector(std::vector<T> vec, const T& scalar) const
        {
            if (vec.empty() || vec.size() > 1)
            {
                return false;
            }

            for (const auto& val : vec) 
            {
                if (val != scalar) 
                {
                    return false;
                }
            }

            return true;
        }
	}
}