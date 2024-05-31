#include "pch.h"
#include <fstream>
#include "ParseCoordinator.h"
#include "IParseHandler.h"

using HandlerRef = size_t;

namespace Fiea
{
	namespace GameEngine
	{
#pragma region ParseCoordinator::Wrapper
		RTTI_DEFINITIONS(ParseCoordinator::Wrapper);

		/*
		* Increments the depth of the wrapper.
		*/
		void ParseCoordinator::Wrapper::incrementDepth()
		{
			++depth;
		}

		/*
		* Decrements the depth of the wrapper.
		*/
		void ParseCoordinator::Wrapper::decrementDepth()
		{
			--depth;
		}

		/*
		*  Returns the depth of the wrapper.
		*/
		size_t ParseCoordinator::Wrapper::getDepth() const
		{
			return depth;
		}
#pragma endregion ParseCoordinator::Wrapper

#pragma region ParseCoordinator
		/*
		* Constructor for ParseCoordinator
		* @param wrapper: The wrapper to initialize the object with.
		*/
		ParseCoordinator::ParseCoordinator(Wrapper& wrapper) : currentWrapper(&wrapper), handlers() {}

		/*
		* Add a hander to the list of handlers and transfer ownership to the ParseCoordinator.
		* @param handler: The handler to add.
		* @return HandlerRef: The reference to the handler.
		*/
		HandlerRef ParseCoordinator::addHandler(std::unique_ptr<IParseHandler>&& handler)
		{
			HandlerRef currentRef = nextRef;
			handlers[nextRef++] = std::move(handler);
			return currentRef;
		}

		/*
		* Remove a handler from the list of handlers.
		* @param ref: The reference to the handler to remove.
		*/
		void ParseCoordinator::removeHandler(HandlerRef ref)
		{
			handlers.erase(ref);
		}

		/*
		* Deserialize a JSON object from a string.
		* @param json: The JSON object to deserialize.
		* @return bool: True if the object was deserialized, false otherwise.
		*/
		bool ParseCoordinator::deserializeObject(const std::string& json)
		{
			Json::Value root;
			Json::CharReaderBuilder builder;
			JSONCPP_STRING errs;

			const int rawJsonLength = static_cast<int>(json.length());
			const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

			if (!reader->parse(json.c_str(), json.c_str() + rawJsonLength, &root, &errs)) 
			{
				throw std::runtime_error(errs.c_str());
			}

			return parseMembers(root);
		}

		/*
		* Deserialize a JSON object from a stream.
		* @param stream: The stream to deserialize the object from.
		* @return bool: True if the object was deserialized, false otherwise.
		*/
		bool ParseCoordinator::deserializeObject(std::istream& stream)
		{
			Json::Value root;
			Json::CharReaderBuilder builder;
			builder["collectComments"] = true;

			JSONCPP_STRING errs;
			if (!parseFromStream(builder, stream, &root, &errs)) 
			{
				throw std::runtime_error(errs.c_str());
			}

			return parseMembers(root);
		}

		/*
		* Deserialize a JSON object from a file.
		* @param filename: The name of the file to deserialize the object from.
		* @return bool: True if the object was deserialized, false otherwise.
		*/
		bool ParseCoordinator::deserializeObjectFromFile(const std::string& filename)
		{
			std::ifstream file(filename);
			if (!file.is_open())
			{
				throw std::runtime_error("File not found.");
			}

			return deserializeObject(file);
		}

		/*
		* Get the current wrapper.
		* @return Wrapper*: The current wrapper.
		*/
		ParseCoordinator::Wrapper* ParseCoordinator::getWrapper() const
		{
			return currentWrapper;
		}
		// ====================== Helper Methods ======================

		/*
		* Parse the JSON object.
		* @param value: The JSON object to parse.
		* @return bool: True if the object was parsed, false otherwise.
		*/
		bool ParseCoordinator::parseMembers(const Json::Value& value)
		{
			bool success = true;
			for (auto it = value.begin(); it != value.end(); ++it)
			{
				std::string key = it.key().asString();
				Json::Value value = *it;
				success = parse(key, value);

				if (!success)
					break;
			}

			return success;
		}

		/*
		* Parse certain key-value pairs in the JSON object.
		* @param key: The key of the key-value pair.
		* @param value: The value of the key-value pair.
		* @param isArray: True if the value is an array, false otherwise.
		* @return bool: True if the key-value pair was parsed, false otherwise.
		*/
		bool ParseCoordinator::parse(const std::string& key, const Json::Value& value)
		{
			bool success = false;

			// Check if the value is an array of objects
			// Assuming all objects in the array are of the same type
			if (value.isArray() && value[0].isObject())
			{
				// Loop through every object in the array with the same key
				for (const Json::Value& subValue : value)
				{
					success = findSuitableHandler(key, subValue);
				}
			}
			else
			{
				success = findSuitableHandler(key, value);
			}

			return success;
		}

		/*
		* Find suitable handler to parse the JSON object. Chain of Responsibility pattern.
		* @param key: The key of the key-value pair.
		* @param value: The value of the key-value pair.
		* @return bool: True if a suitable handler did the job, false otherwise.
		*/
		bool ParseCoordinator::findSuitableHandler(const std::string& key, const Json::Value& value)
		{
			for (auto it = handlers.begin(); it != handlers.end(); ++it)
			{
				it->second->initialize();

				if (it->second->start(key, value, currentWrapper))
				{
					if (value.isObject())
					{
						currentWrapper->incrementDepth();
						// Parse the members of the object recursively
						if (!parseMembers(value))
						{
							return false;
						}
						currentWrapper->decrementDepth();
					}

					return it->second->end(key, currentWrapper);
				}
			}

			return false;
		}
#pragma endregion ParseCoordinator
	}
}