#include "pch.h"
#include "StringHandler.h"
#include "TableWrapper.h"
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for StringHandler
		*/
		IParseHandler* StringHandler::create() const
		{
			return new StringHandler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void StringHandler::initialize()
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
		bool StringHandler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attrubutedWrapper = wrapper->As<AttributedWrapper>();

			if (tableWrapper != nullptr)
			{
				if (tableWrapper->getDepth() >= tableWrapper->maxDepth)
				{
					tableWrapper->maxDepth = tableWrapper->getDepth();
				}

				if (hasPrefix(key, "str_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(value[i].asString(), i);
							++startCount;
						}
						return true;
					}
					else if (value.isString())
					{
						Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(value.asString());
						++startCount;
						return true;
					}
				}
			}

			if (attrubutedWrapper != nullptr)
			{
				if (hasPrefix(key, "str_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = attrubutedWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(value[i].asString(), i);
							++startCount;
						}
						return true;
					}
					else if (value.isString())
					{
						Datum& datum = attrubutedWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(value.asString());
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
		bool StringHandler::end(const std::string&, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			if (tableWrapper == nullptr && attributedWrapper == nullptr) return false;

			++endCount;
			return true;
		}
	}
}