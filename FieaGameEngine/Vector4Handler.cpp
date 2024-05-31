#include "pch.h"
#include "Vector4Handler.h"
#include "TableWrapper.h"
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for Vector4Handler
		*/
		IParseHandler* Vector4Handler::create() const
		{
			return new Vector4Handler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void Vector4Handler::initialize()
		{
			initialized = true;
		}

		/**
		 * @brief Override for IParseHandler::start
		 * @param key: The key of the value
		 * @param value: The value to parse
		 * @param wrapper: The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool Vector4Handler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();

			if (tableWrapper != nullptr)
			{
				if (tableWrapper->getDepth() >= tableWrapper->maxDepth)
				{
					tableWrapper->maxDepth = tableWrapper->getDepth();
				}

				if (hasPrefix(key, "vec4_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(stringToVec4(value[i].asString()));
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(stringToVec4(value.asString()));
						++startCount;
						return true;
					}
				}
			}

			if (attributedWrapper != nullptr)
			{
				if (hasPrefix(key, "vec4_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(stringToVec4(value[i].asString()));
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(stringToVec4(value.asString()));
						++startCount;
						return true;
					}
				}
			}

			return false;
		}

		/**
		 * @brief Override for IParseHandler::end
		 * @param unused
		 * @param wrapper: The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool Vector4Handler::end(const std::string&, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			
			if (tableWrapper == nullptr && attributedWrapper == nullptr) return false;

			++endCount;
			return true;
		}

		/*
		* @brief Converts a string to a vec4
		* @param value: The string to convert
		* @return The vec4
		*/
		glm::vec4 Vector4Handler::stringToVec4(const std::string& value)
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
	}
}