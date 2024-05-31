#include "pch.h"
#include "IntegerHandler.h"
#include "TableWrapper.h"
#include "AttributedWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for IntegerHandler
		*/
		IParseHandler* IntegerHandler::create() const
		{
			return new IntegerHandler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void IntegerHandler::initialize()
		{
			initialized = true;
		}

		/**
		 * @brief Override for IParseHandler::start
		 * @param key The key of the value
		 * @param value The value to parse
		 * @param wrapper The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool IntegerHandler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();

			if (tableWrapper != nullptr)
			{
				if (tableWrapper->getDepth() >= tableWrapper->maxDepth)
				{
					tableWrapper->maxDepth = tableWrapper->getDepth();
				}

				if (hasPrefix(key, "int_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(value[i].asInt());
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = tableWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(value.asInt());
						++startCount;
						return true;
					}
				}
			}

			if (attributedWrapper != nullptr)
			{
				if (hasPrefix(key, "int_"))
				{
					if (value.isArray())
					{
						for (unsigned int i = 0; i < value.size(); ++i)
						{
							Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
							datum.push_back_force(value[i].asInt());
							++startCount;
						}
						return true;
					}
					else
					{
						Datum& datum = attributedWrapper->getCurrentSubTable()->append(key);
						datum.push_back_force(value.asInt());
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
		 * @param wrapper The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool IntegerHandler::end(const std::string&, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			AttributedWrapper* attributedWrapper = wrapper->As<AttributedWrapper>();
			if (tableWrapper == nullptr && attributedWrapper == nullptr) return false;

			++endCount;
			return true;
		}
	}
}