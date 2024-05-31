#pragma once
#include <string>
#include <json/json.h>
#include "ParseCoordinator.h"

namespace Fiea 
{
	namespace GameEngine
	{
		class IParseHandler
		{
		public:
			// Virtual constructor pattern
			[[nodiscard]] virtual IParseHandler* create() const = 0;

			// Virtual Initialize method
			virtual void initialize();

			// Pure virtual Start method
			virtual bool start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) = 0;

			// Pure virtual End method
			virtual bool end(const std::string& key, ParseCoordinator::Wrapper* wrapper) = 0;

			// Virtual destructor
			virtual ~IParseHandler() = default;

			bool isInitialized() const;

		protected:
			bool hasPrefix(const std::string& key, const std::string& prefix) const;
			std::string getPrefix(const std::string& key) const;
			bool initialized = false;
		};
	}
}