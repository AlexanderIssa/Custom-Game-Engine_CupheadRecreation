#pragma once
#include <json/json.h>
#include <string>
#include <map>
#include "RTTI.h"

using HandlerRef = size_t;

namespace Fiea
{
	namespace GameEngine
	{
		class IParseHandler;

		class ParseCoordinator
		{
		public:
			// Embedded Wrapper class, a base class for warappers in different handlers to override based on JSON format they handle
			class Wrapper : public RTTI
			{
				RTTI_DECLARATIONS(Wrapper, RTTI);

			public:
				void incrementDepth();
				void decrementDepth();
				size_t getDepth() const;
				virtual ~Wrapper() = default;

			private:
				size_t depth = 0;
			};


			// Constructor that takes a reference to a Wrapper object to initialize the object
			ParseCoordinator(Wrapper& wrapper);

			// TEMP: copy / move constructors and assignment operators are deleted for now
			ParseCoordinator(const ParseCoordinator& other) = delete;
			ParseCoordinator(ParseCoordinator&& other) noexcept = delete;
			ParseCoordinator& operator=(const ParseCoordinator& other) = delete;
			ParseCoordinator& operator=(ParseCoordinator&& other) noexcept = delete;

			// Destructor
			~ParseCoordinator() = default;

			// AddHandler method that takes a unique_ptr to an IParseHandler object and returns a HandlerRef
			// unique_ptr can not be copied, so we use move semantics to transfer ownership
			HandlerRef addHandler(std::unique_ptr<IParseHandler>&& handler);
			
			// RemoveHandler method that takes a HandlerRef and removes the handler from the list
			void removeHandler(HandlerRef ref);

			// Deserialize JSON object from a string
			bool deserializeObject(const std::string& json);

			// Deserialize JSON object from std::istream
			bool deserializeObject(std::istream& stream);

			// Deserialize JSON object from a file
			bool deserializeObjectFromFile(const std::string& filename);

			// Return the address of the current Wrapper object
			Wrapper* getWrapper() const;

			// Return the handler with the given reference
			template<typename T>
			T* getHandler(HandlerRef ref) const;

		private:
			HandlerRef nextRef = 1;
			std::map<HandlerRef, std::unique_ptr<IParseHandler>> handlers;
			Wrapper* currentWrapper = nullptr;

			// Helper methods to parse JSON object
			bool parseMembers(const Json::Value& value);
			bool parse(const std::string& key, const Json::Value& value);
			bool findSuitableHandler(const std::string& key, const Json::Value& value);
		};
	}
}

#include "ParseCoordinator.inl"