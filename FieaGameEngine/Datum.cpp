#include "pch.h"
#include "Datum.h"
#include "Scope.h"
#include <stdexcept>

namespace Fiea
{
	namespace GameEngine
	{
        /*
        * Default Constructor for Datum
        */
        Datum::Datum() : 
            mDataType(DataType::UNKNOWN),
            mSize(0), 
            mCapacity(0), 
            isExternalStorage(false),
            mData{} 
        {}

        /*
        * Copy Constructor for Datum that takes in a DataType and size
        * @param other - The Datum to copy from
        */
        Datum::Datum(const Datum& other) : 
            mDataType(other.mDataType), 
            mSize(other.mSize),
            mCapacity(other.mCapacity),
            isExternalStorage(other.isExternalStorage)
        {
            copyData(other);
        }

        /*
        * Move Constructor for Datum that takes in a DataType and size
        * @param other - The Datum to move from
		*/
        Datum::Datum(Datum&& other) noexcept:
            mDataType(std::move(other.mDataType)),
            mSize(std::move(other.mSize)),
            mCapacity(std::move(other.mCapacity)),
            mData(std::move(other.mData)),
            isExternalStorage(std::move(other.isExternalStorage))
        {  
            other.mSize = 0;
            other.mCapacity = 0;
            other.mDataType = DataType::UNKNOWN;
            other.mData = {};
            other.isExternalStorage = false;
        }

        /*
        * Destructor for Datum
        */
        Datum::~Datum() 
        {
            destroyData();
        }


        /*
        * Copy assignment operator
        * @param other - The Datum to copy from
        */
        Datum& Datum::operator=(const Datum& other) 
        {
            if (isExternalStorage != other.isExternalStorage && mDataType != DataType::UNKNOWN)
                throw std::runtime_error("Converting internal storage to extreanl storage is not supported, and vice versa");

            if (mDataType != DataType::UNKNOWN)
            {
                if (mDataType != other.mDataType)
                    throw std::runtime_error("Cannot change the data type of a Datum");
            }

            if (this != &other) 
            {
                destroyData();
                mDataType = other.mDataType;
                mSize = other.mSize;
                mCapacity = other.mCapacity;
                isExternalStorage = other.isExternalStorage;
                copyData(other);
            }
            return *this;
        }

        /*
        * Move assignment operator
        * @param other - The Datum to move from
        */
        Datum& Datum::operator=(Datum&& other)
        {
            if(isExternalStorage != other.isExternalStorage && mDataType != DataType::UNKNOWN)
                throw std::runtime_error("Converting internal storage to extreanl storage is not supported, and vice versa");

            if (mDataType != DataType::UNKNOWN)
            {
                if (mDataType != other.mDataType)
                    throw std::runtime_error("Cannot change the data type of a Datum");
            }

            if (this != &other) 
            {
                destroyData();
                mDataType = std::move(other.mDataType);
                mSize = std::move(other.mSize);
                mCapacity = std::move(other.mCapacity);
                isExternalStorage = std::move(other.isExternalStorage);
                mData = std::move(other.mData);

                // Reset the other Datum
                other.mDataType = DataType::UNKNOWN;
                other.mSize = 0;
                other.mCapacity = 0;
                other.mData = {};
            }
            return *this;
        }

        /*
        * Compare two Datum objects for equality
        * @param other - The Datum to compare with
        * @return True if the two Datum objects are equal, false otherwise
        */
        bool Datum::operator==(const Datum& other) const 
        {
            // [03/10/2024] - Added support for comparing external storage with internal storage
            if (mDataType != other.mDataType 
                || mSize != other.mSize )
            {
                return false;
            }

            if (!isExternalStorage) 
            {
                if (isExternalStorage != other.isExternalStorage)
                {
                    switch (mDataType)
                    {
                        case DataType::INTEGER:
                            return areArraysEqual(*mData.intData, mSize, other.mData.externalIntData, other.mSize);
                        case DataType::FLOAT:
                            return areArraysEqual(*mData.floatData, mSize, other.mData.externalFloatData, other.mSize);
                        case DataType::STRING:
                            return areArraysEqual(*mData.stringData, mSize, other.mData.externalStringData, other.mSize);
                        case DataType::VEC4:
                            return areArraysEqual(*mData.vec4Data, mSize, other.mData.externalVec4Data, other.mSize);
                        case DataType::MAT4:
                            return areArraysEqual(*mData.mat4Data, mSize, other.mData.externalMat4Data, other.mSize);
                        case DataType::TABLE:
							return areArraysEqual(*mData.tableData, mSize, other.mData.externalTableData, other.mSize);
                        case DataType::POINTER:
							return areArraysEqual(*mData.pointerData, mSize, other.mData.externalPointerData, other.mSize);
                        default:
                            return true; // UNKNOWN type is always considered equal
                    }
                }
                else
                {
                    switch (mDataType)
                    {
                        case DataType::INTEGER:
                            return *mData.intData == *other.mData.intData;
                        case DataType::FLOAT:
                            return *mData.floatData == *other.mData.floatData;
                        case DataType::STRING:
                            return *mData.stringData == *other.mData.stringData;
                        case DataType::VEC4:
                            return *mData.vec4Data == *other.mData.vec4Data;
                        case DataType::MAT4:
                            return *mData.mat4Data == *other.mData.mat4Data;
                        case DataType::TABLE:
                            return *mData.tableData == *other.mData.tableData;
                        case DataType::POINTER:
                            return *mData.pointerData == *other.mData.pointerData;
                        default:
                            return true; // UNKNOWN type is always considered equal
                    }
                }
            }   
            else 
            {
                if (isExternalStorage != other.isExternalStorage)
                {
                    switch (mDataType)
                    {
                        case DataType::INTEGER:
                            return areArraysEqual(*other.mData.intData, other.mSize, mData.externalIntData, mSize);
                        case DataType::FLOAT:
                            return areArraysEqual(*other.mData.floatData, other.mSize, mData.externalFloatData, mSize);
                        case DataType::STRING:
                            return areArraysEqual(*other.mData.stringData, other.mSize, mData.externalStringData, mSize);
                        case DataType::VEC4:
                            return areArraysEqual(*other.mData.vec4Data, other.mSize, mData.externalVec4Data, mSize);
                        case DataType::MAT4:
                            return areArraysEqual(*other.mData.mat4Data, other.mSize, mData.externalMat4Data, mSize);
                        case DataType::TABLE:
							return areArraysEqual(*other.mData.tableData, other.mSize, mData.externalTableData, mSize);
                        case DataType::POINTER:
                            return areArraysEqual(*other.mData.pointerData, other.mSize, mData.externalPointerData, mSize);
                        default:
                            return true; // UNKNOWN type is always considered equal
                    }
                }
                else
                {
                    switch (mDataType)
                    {
                        case DataType::INTEGER:
                            return areArraysEqual(mData.externalIntData, mSize, other.mData.externalIntData, other.mSize);
                        case DataType::FLOAT:
                            return areArraysEqual(mData.externalFloatData, mSize, other.mData.externalFloatData, other.mSize);
                        case DataType::STRING:
                            return areArraysEqual(mData.externalStringData, mSize, other.mData.externalStringData, other.mSize);
                        case DataType::VEC4:
                            return areArraysEqual(mData.externalVec4Data, mSize, other.mData.externalVec4Data, other.mSize);
                        case DataType::MAT4:
                            return areArraysEqual(mData.externalMat4Data, mSize, other.mData.externalMat4Data, other.mSize);
                        case DataType::TABLE:
                            return areArraysEqual(mData.externalTableData, mSize, other.mData.externalTableData, other.mSize);
                        case DataType::POINTER:
                            return areArraysEqual(mData.externalPointerData, mSize, other.mData.externalPointerData, other.mSize);
                        default:
                            return true; // UNKNOWN type is always considered equal
                    }
                }
            }
        }

        /*
        * Compare two Datum objects for inequality
        * @param other - The Datum to compare with
        * @return True if the two Datum objects are not equal, false otherwise
        */
        bool Datum::operator!=(const Datum& other) const 
        {
            return !(*this == other);
        }

        /*
        * Clear the Datum without changing its capacity
        */
        void Datum::clear()
        {
            if(isExternalStorage)
                throw std::runtime_error("Cannot clear external storage");

            switch (mDataType)
            {
                case DataType::INTEGER:
                    mData.intData->clear();
                    break;
                case DataType::FLOAT:
                    mData.floatData->clear();
                    break;
                case DataType::STRING:
                    mData.stringData->clear();
                    break;
                case DataType::VEC4:
                    mData.vec4Data->clear();
					break;
                case DataType::MAT4:
                    mData.mat4Data->clear();
					break;
                case DataType::TABLE:
                    mData.tableData->clear();
                    break;
                case DataType::POINTER:
                    mData.pointerData->clear();
					break;
                case DataType::UNKNOWN:
                default:
                    break;
            }

            mSize = 0;
		}

        /*
        * Remove an item from the Datum at the specified index
        * @param index - The index of the item to remove
        */
        void Datum::remove(size_t index) 
        {
            if(isExternalStorage)
                throw std::runtime_error("Cannot remove from external storage");

            if (index >= mSize) 
                throw std::out_of_range("Index out of range");

            switch (mDataType)
            {
                case DataType::INTEGER:
                    mData.intData->erase(mData.intData->begin() + index);
                    break;
                case DataType::FLOAT:
                    mData.floatData->erase(mData.floatData->begin() + index);
                    break;
                case DataType::STRING:
                    mData.stringData->erase(mData.stringData->begin() + index);
                    break;
                case DataType::VEC4:
                    mData.vec4Data->erase(mData.vec4Data->begin() + index);
					break;
                case DataType::MAT4:
                    mData.mat4Data->erase(mData.mat4Data->begin() + index);
					break;
                case DataType::TABLE:
                    mData.tableData->erase(mData.tableData->begin() + index);
                    break;
                case DataType::POINTER:
                    mData.pointerData->erase(mData.pointerData->begin() + index);
                    break;
            }
            mSize--;
        }

        /*
        * Set storage special case for Attributed
        * @param type - The data type of the Datum
        * @param ptr - The pointer to the data
        */
        void Datum::setStorage(DataType type, void* ptr, size_t size)
        {
            int* intPtr = nullptr;
            float* floatPtr = nullptr;
            std::string* stringPtr = nullptr;
            glm::vec4* vec4Ptr = nullptr;
            glm::mat4* mat4Ptr = nullptr;
            RTTI** rttiPtr = nullptr;
            Scope* scopePtr = nullptr;

            switch (type)
            {
            case Datum::DataType::INTEGER:
                intPtr = reinterpret_cast<int*>(ptr);
                setStorage(intPtr, size);
                break;
            case Datum::DataType::FLOAT:
                floatPtr = reinterpret_cast<float*>(ptr);
                setStorage(floatPtr, size);
                break;
            case Datum::DataType::STRING:
                stringPtr = reinterpret_cast<std::string*>(ptr);
                setStorage(stringPtr, size);
                break;
            case Datum::DataType::VEC4:
                vec4Ptr = reinterpret_cast<glm::vec4*>(ptr);
                setStorage(vec4Ptr, size);
                break;
            case Datum::DataType::MAT4:
                mat4Ptr = reinterpret_cast<glm::mat4*>(ptr);
                setStorage(mat4Ptr, size);
                break;
            case Datum::DataType::TABLE:
                scopePtr = reinterpret_cast<Scope*>(ptr);
                push_back(scopePtr);
                break;
            case Datum::DataType::POINTER:
                rttiPtr = reinterpret_cast<RTTI**>(ptr);
                setStorage(rttiPtr, size);
                break;
            default:
                break;
            }
        }

        /*
        * Set the value of the item at the specified index from a string
        * [Note] Float will be converted to int if the string is an integer, and vice versa
        * @param index - The index of the item to set
        * @param value - The value of type T to set the item to
        */
        void Datum::setFromString(size_t index, const std::string& value)
        {
            if (index >= mSize) throw std::out_of_range("Index out of range");

            try 
            {
                if (!isExternalStorage) 
                {
                    switch (mDataType)
                    {
                    case DataType::INTEGER:
                        if (mData.intData && index < mData.intData->size())
                        {
                            (*mData.intData)[index] = std::stoi(value);
                        }
                        break;
                    case DataType::FLOAT:
                        if (mData.floatData && index < mData.floatData->size())
                        {
                            (*mData.floatData)[index] = std::stof(value);
                        }
                        break;
                    case DataType::STRING:
                        if (mData.stringData && index < mData.stringData->size())
                        {
                            (*mData.stringData)[index] = value;
                        }
                        break;
                    case DataType::VEC4:
                        if (mData.vec4Data && index < mData.vec4Data->size())
                        {
                            (*mData.vec4Data)[index] = stringToVec4(value);
                        }
                        break;
                    case DataType::MAT4:
                        if (mData.mat4Data && index < mData.mat4Data->size())
                        {
                            (*mData.mat4Data)[index] = stringToMat4(value);
                        }
                        break;
                    case DataType::UNKNOWN:
                    default:
                        throw std::invalid_argument("Unsupported data type");
                    }
                }
                else
                {
                    switch (mDataType)
                    {
                    case DataType::INTEGER:
                        if (mData.intData && index < mData.intData->size())
                        {
                            (mData.externalIntData)[index] = std::stoi(value);
                        }
                        break;
                    case DataType::FLOAT:
                        if (mData.floatData && index < mData.floatData->size())
                        {
                            (mData.externalFloatData)[index] = std::stof(value);
                        }
                        break;
                    case DataType::STRING:
                        if (mData.stringData && index < mData.stringData->size())
                        {
                            (mData.externalStringData)[index] = value;
                        }
                        break;
                    case DataType::VEC4:
                        if (mData.vec4Data && index < mData.vec4Data->size())
                        {
                            (mData.externalVec4Data)[index] = stringToVec4(value);
                        }
                        break;
                    case DataType::MAT4:
                        if (mData.mat4Data && index < mData.mat4Data->size())
                        {
                            (mData.externalMat4Data)[index] = stringToMat4(value);
                        }
                        break;
                    case DataType::UNKNOWN:
                    default:
                        throw std::invalid_argument("Unsupported data type");
                    }
                }
                
            }
            catch (const std::invalid_argument& e) 
            {
                // Handle conversion error
                throw e;
            }
            catch (const std::out_of_range& e) 
            {
                // Handle conversion error, possibly due to string representing a number out of range for int or float
                throw e;
            }
        }

        /*
        * Get the item at the specified index
        * @param index - The index of the item to get
        * @return The string representation of the value of the item at the specified index
        */
        std::string Datum::getAsString(size_t index) const
        {
            if (index >= mSize) throw std::out_of_range("Index out of range");

            if (!isExternalStorage)
            {
                switch (mDataType)
                {
                    case DataType::INTEGER:
                        return std::to_string((*mData.intData)[index]);
                    case DataType::FLOAT:
                        return std::to_string((*mData.floatData)[index]);
                    case DataType::STRING:
                        return (*mData.stringData)[index];
                    case DataType::VEC4:
                        return glm::to_string((*mData.vec4Data)[index]);
                    case DataType::MAT4:
                        return glm::to_string((*mData.mat4Data)[index]);
                    case DataType::TABLE:
                        return (*mData.tableData)[index]->ToString();
                    case DataType::POINTER:
                        return (*mData.pointerData)[index]->ToString();
                    default:
                        throw std::logic_error("Unsupported data type OR data type is UNKNOWN");
                }
            }
            else 
            {
                switch (mDataType)
                {
                    case DataType::INTEGER:
                        return std::to_string((mData.externalIntData)[index]);
                    case DataType::FLOAT:
                        return std::to_string((mData.externalFloatData)[index]);
                    case DataType::STRING:
                        return (mData.externalStringData)[index];
                    case DataType::VEC4:
                        return glm::to_string((mData.externalVec4Data)[index]);
                    case DataType::MAT4:
                        return glm::to_string((mData.externalMat4Data)[index]);
                    case DataType::TABLE:
						return (mData.externalTableData)[index]->ToString();
                    case DataType::POINTER:
						return (mData.externalPointerData)[index]->ToString();
                    default:
                        throw std::logic_error("Unsupported data type OR data type is UNKNOWN");
                }
            } 
        }

        /*
        * Remove the last item from the Datum
        */
        void Datum::pop() 
        {
            if (mSize > 0) 
            {
                remove(mSize - 1);
            }
            else
            {
				throw std::out_of_range("Cannot pop from an empty Datum");
			}
        }

        /*
        * Get current data type of the Datum
        * @return The data type of the Datum
        */
        Datum::DataType Datum::getDataType() const 
        {
            return mDataType;
        }

        /*
        * Get the size of the Datum
        * @return The size of the Datum
        */
        size_t Datum::getSize() const
        {
		    return mSize;
        }

        /*
        * Get the capacity of the Datum
        * @return The capacity of the Datum
        */
        size_t Datum::getCapacity() const
		{
            return mCapacity;
		}

        void Datum::resize(size_t newSize)
        {
            if(isExternalStorage) 
                throw std::runtime_error("Cannot resize external storage");

            switch (mDataType) 
            {
                case DataType::INTEGER:
                    mData.intData->resize(newSize);
                    mSize = newSize;

                    if(mCapacity > mSize)
					{
						mData.intData->shrink_to_fit();
					}

                    mCapacity = mData.intData->capacity();
                    break;
                case DataType::FLOAT:
                    mData.floatData->resize(newSize);
                    mSize = newSize;

                    if(mCapacity > mSize)
					{
                        mData.floatData->shrink_to_fit();
					}

                    mCapacity = mData.floatData->capacity();
                    break;
                case DataType::STRING:
                    mData.stringData->resize(newSize);
                    mSize = newSize;

                    if (mCapacity > mSize)
                    {
                        mData.stringData->shrink_to_fit();
                    }

                    mCapacity = mData.stringData->capacity();
                    break;
                case DataType::VEC4:
                    mData.vec4Data->resize(newSize);
                    mSize = newSize;

                    if (mCapacity > mSize)
					{
						mData.vec4Data->shrink_to_fit();
					}

                    mCapacity = mData.vec4Data->capacity();
                    break;
                case DataType::MAT4:
					mData.mat4Data->resize(newSize);
                    mSize = newSize;

                    if (mCapacity > mSize) 
                    {
                        mData.mat4Data->shrink_to_fit();
                    }

                    mCapacity = mData.mat4Data->capacity();
					break;
                case DataType::TABLE:
                    mData.tableData->resize(newSize);
                    mSize = newSize;

                    if (mCapacity > mSize)
                    {
                        mData.tableData->shrink_to_fit();
                    }

                    mCapacity = mData.tableData->capacity();
                    break;
                case DataType::POINTER:
                    mData.pointerData->resize(newSize);
					mSize = newSize;

                    if (mCapacity > mSize)
                    {
						mData.pointerData->shrink_to_fit();
					}

					mCapacity = mData.pointerData->capacity();
					break;
                case DataType::UNKNOWN:
                default:
                    throw std::logic_error("Unsupported data type OR data type is UNKNOWN");
            }
        }


        // ================== Helper functions ==================
        void Datum::destroyData()
        {
            // If the data is not stored externally, delete it
            if (!isExternalStorage) 
            {
                switch (mDataType)
                {
                case DataType::INTEGER:
                    delete mData.intData;
                    break;
                case DataType::FLOAT:
                    delete mData.floatData;
                    break;
                case DataType::STRING:
                    delete mData.stringData;
                    break;
                case DataType::VEC4:
                    delete mData.vec4Data;
                    break;
                case DataType::MAT4:
                    delete mData.mat4Data;
                    break;
                case DataType::TABLE:
                    delete mData.tableData;
                    break;
                case DataType::POINTER:
                    delete mData.pointerData;
					break;
                case DataType::UNKNOWN:
                default:
                    break; // Handle UNKNOWN or any other types appropriately
                }
            }   

            mData = {};
            mSize = 0;
            mCapacity = 0;
            mDataType = DataType::UNKNOWN;
            isExternalStorage = false;
        }

        void Datum::copyData(const Datum& other)
        {
            if (!isExternalStorage) 
            {
                switch (other.mDataType)
                {
                    case DataType::INTEGER:
                        mData.intData = new std::vector<int>(*other.mData.intData);
                        break;
                    case DataType::FLOAT:
                        mData.floatData = new std::vector<float>(*other.mData.floatData);
                        break;
                    case DataType::STRING:
                        mData.stringData = new std::vector<std::string>(*other.mData.stringData);
                        break;
                    case DataType::VEC4:
                        mData.vec4Data = new std::vector<glm::vec4>(*other.mData.vec4Data);
                        break;
                    case DataType::MAT4:
                        mData.mat4Data = new std::vector<glm::mat4>(*other.mData.mat4Data);
                        break;
                    case DataType::TABLE:
                        mData.tableData = new std::vector<Scope*>(*other.mData.tableData);
                        break;
                    case DataType::POINTER:
                        mData.pointerData = new std::vector<RTTI*>(*other.mData.pointerData);
						break;
                    case DataType::UNKNOWN:
                        break; // Handle UNKNOWN or any other types appropriately
                }
            }
            else
            {
                switch (other.mDataType)
                {
                    case DataType::INTEGER:
                        mData.externalIntData = other.mData.externalIntData;
                        break;
                    case DataType::FLOAT:
                    	mData.externalFloatData = other.mData.externalFloatData;
                        break;
                    case DataType::STRING:
                    	mData.externalStringData = other.mData.externalStringData;
                        break;
                    case DataType::VEC4:
                    	mData.externalVec4Data = other.mData.externalVec4Data;
                        break;
                    case DataType::MAT4:
                    	mData.externalMat4Data = other.mData.externalMat4Data;
                        break;
                    case DataType::TABLE:
						mData.externalTableData = other.mData.externalTableData;
						break;
                    case DataType::POINTER:
                        mData.externalPointerData = other.mData.externalPointerData;
                        break;
                }
            }
        }

        glm::vec4 Datum::stringToVec4(const std::string& value)
		{
			glm::vec4 vec;
            float v0, v1, v2, v3;
            int count = sscanf_s(value.c_str(), "vec4(%f, %f, %f, %f)", &v0, &v1, &v2, &v3);

            if (count == 4)
			{
				vec = glm::vec4(v0, v1, v2, v3);
				return vec;
			}
			else
			{
				throw std::invalid_argument("Invalid string format for vec4");
			}
		}

        glm::mat4 Datum::stringToMat4(const std::string& value) 
        {
            glm::mat4 mat;
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;

            int count = sscanf_s
            (
                value.c_str(), 
                "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&m00, &m01, &m02, &m03,
				&m10, &m11, &m12, &m13,
				&m20, &m21, &m22, &m23,
				&m30, &m31, &m32, &m33
            );
			
            if (count == 16)
			{
				mat = glm::mat4
                (
                    m00, m01, m02, m03,
					m10, m11, m12, m13,
					m20, m21, m22, m23,
					m30, m31, m32, m33
                );
				return mat;
			}
			else
			{
				throw std::invalid_argument("Invalid string format for mat4");
			}
        }
	}
}