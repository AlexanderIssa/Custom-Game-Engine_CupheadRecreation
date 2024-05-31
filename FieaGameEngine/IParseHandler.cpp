#include "pch.h"
#include "IParseHandler.h"

namespace Fiea
{
	namespace GameEngine
	{
		void IParseHandler::initialize()
		{
			initialized = true;
		}

		bool IParseHandler::isInitialized() const
		{
			return initialized;
		}

		// Check if the key has a prefix
		bool IParseHandler::hasPrefix(const std::string& key, const std::string& prefix) const
		{
			if (key.length() < prefix.length())
				return false;
			return key.substr(0, prefix.length()) == prefix;
		}

		// Get the prefix of the key before "_", return the prefix without "_"
		std::string IParseHandler::getPrefix(const std::string& key) const
		{
			size_t pos = key.find("_");
			if (pos == std::string::npos)
				return key;
			return key.substr(0, pos);
		}
		
	}
}