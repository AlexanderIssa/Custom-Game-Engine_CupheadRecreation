#include "pch.h"
#include "FloatHandler.h"
#include "TableWrapper.h"
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for FloatHandler
		*/
		IParseHandler* FloatHandler::create() const
		{
			return new FloatHandler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void FloatHandler::initialize()
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
		bool FloatHandler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();

			if (tableWrapper != nullptr)
			{
				if (tableWrapper->getDepth() >= tableWrapper->maxDepth)
				{
					tableWrapper->maxDepth = tableWrapper->getDepth();
				}

				if (hasPrefix(key, "float_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(value[i].asFloat());
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(value.asFloat());
						++startCount;
						return true;
					}
				}
			}

			if (attributedWrapper != nullptr)
			{
				if (hasPrefix(key, "float_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(value[i].asFloat());
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(value.asFloat());
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
		bool FloatHandler::end(const std::string&, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			if (tableWrapper == nullptr && attributedWrapper == nullptr) return false;

			++endCount;
			return true;
		}
	}
}