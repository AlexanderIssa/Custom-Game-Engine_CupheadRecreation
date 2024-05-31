#include "pch.h"
#include "TableHandler.h"
#include "TableWrapper.h"

namespace Fiea
{
	namespace GameEngine
	{
		/*
		* @brief virtual constructor for TableHandler
		*/
		IParseHandler* TableHandler::create() const
		{
			return new TableHandler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void TableHandler::initialize()
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
		bool TableHandler::start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			if (tableWrapper == nullptr) return false;

			if (tableWrapper->getDepth() >= tableWrapper->maxDepth)
			{
				tableWrapper->maxDepth = tableWrapper->getDepth();
			}

			if (hasPrefix(key, "obj_"))
			{
				Scope& subTable = tableWrapper->getCurrentSubTable()->appendScope(key);
				tableWrapper->setCurrentSubTable(&subTable);

				++startCount;
				return true;
			}

			return false;
		}

		/**
		 * @brief Override for IParseHandler::end
		 * @param key: The key of the value
		 * @param wrapper: The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool TableHandler::end(const std::string& key, ParseCoordinator::Wrapper* wrapper)
		{
			TableWrapper* tableWrapper = wrapper->As<TableWrapper>();
			if (tableWrapper == nullptr) return false;

			if (hasPrefix(key, "obj_"))
			{
				tableWrapper->popSubTable();

				++endCount;
				return true;
			}

			return false;
		}
	}
}