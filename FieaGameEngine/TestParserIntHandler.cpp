#include "pch.h"
#include "TestParseIntHandler.h"

namespace Fiea 
{
	namespace GameEngine
	{
#pragma region TestParseIntHandler::Wrapper
		RTTI_DEFINITIONS(TestParseIntHandler::Wrapper);

		/**
		 * @brief RTTI override for Equals
		 * @param rhs
		 * @return true if equal, false otherwise
		*/
		bool TestParseIntHandler::Wrapper::Equals(const RTTI* rhs) const
		{
			if (rhs == nullptr) return false;

			const TestParseIntHandler::Wrapper* other = rhs->As<TestParseIntHandler::Wrapper>();
			return mInt == other->mInt;
		}

		/**
		 * @brief RTTI override for ToString
		 * @return string representation of this Wrapper
		*/
		std::string TestParseIntHandler::Wrapper::ToString() const
		{
			return "TestParseIntHandler: *mInt =" + std::to_string(*mInt);
		}

		/*
		* @brief Set the int value
		* @param i The int value to set
		*/
		void TestParseIntHandler::Wrapper::setInt(int i)
		{
			*mInt = i;
		}

#pragma endregion TestParseIntHandler::Wrapper

#pragma region TestParseIntHandler
		/*
		* @brief virtual constructor for TestParseHandler
		*/
		IParseHandler* TestParseIntHandler::create() const
		{
			return new TestParseIntHandler();
		}

		/**
		 * @brief Override for IParseHandler::Initialize
		*/
		void TestParseIntHandler::initialize()
		{
			initialized = true;
		}

		/**
		 * @brief Override for IParseHandler::Start
		 * @param unused
		 * @param value The value to parse
		 * @param wrapper The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool TestParseIntHandler::start(const std::string&, const Json::Value& value, ParseCoordinator::Wrapper* wrapper)
		{
			TestParseIntHandler::Wrapper* testWrapper = wrapper->As<TestParseIntHandler::Wrapper>();
			if (testWrapper == nullptr || !value.isInt() || count > 0) return false;

			++count;
			testWrapper->setInt(value.asInt());
			return true;
		}

		/**
		 * @brief Override for IParseHandler::End
		 * @param unused
		 * @param wrapper The wrapper to populate
		 * @return True, if handled, false otherwise
		*/
		bool TestParseIntHandler::end(const std::string&, ParseCoordinator::Wrapper* wrapper)
		{
			Wrapper* testWrapper = wrapper->As<TestParseIntHandler::Wrapper>();
			if (testWrapper == nullptr) return false;
			return true;
		}

#pragma endregion TestParseHandler
	}
}