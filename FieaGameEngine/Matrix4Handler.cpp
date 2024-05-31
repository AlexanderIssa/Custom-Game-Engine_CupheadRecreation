#include "pch.h"
#include "Matrix4Handler.h"
#include "TableWrapper.h"
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for Matrix4Handler
		*/
		IParseHandler* Matrix4Handler::create() const
		{
			return new Matrix4Handler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void Matrix4Handler::initialize()
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
		bool Matrix4Handler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();

			if (tableWrapper != nullptr)
			{
				if (tableWrapper->getDepth() >= tableWrapper->maxDepth)
				{
					tableWrapper->maxDepth = tableWrapper->getDepth();
				}

				if (hasPrefix(key, "mat4_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(stringToMat4(value[i].asString()));
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(stringToMat4(value.asString()));
						++startCount;
						return true;
					}
				}
			}

			if (attributedWrapper != nullptr)
			{
				if (hasPrefix(key, "mat4_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(stringToMat4(value[i].asString()));
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(stringToMat4(value.asString()));
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
		bool Matrix4Handler::end(const std::string&, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			if (tableWrapper == nullptr && attributedWrapper == nullptr) return false;

			++endCount;
			return true;
		}

		/*
		* @brief Converts a string to a mat4
		* @param value: The string to convert
		* @return The mat4
		*/
		glm::mat4 Matrix4Handler::stringToMat4(const std::string& value)
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