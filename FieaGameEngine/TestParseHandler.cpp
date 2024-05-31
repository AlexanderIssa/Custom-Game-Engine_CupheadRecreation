#include "pch.h"
#include "TestParseHandler.h"

namespace Fiea::GameEngine::test
{
#pragma region TestParseHandler::Wrapper
	RTTI_DEFINITIONS(TestParseHandler::Wrapper);

	/**
	 * @brief RTTI override for Equals
	 * @param rhs 
	 * @return true if equal, false otherwise
	*/
	bool TestParseHandler::Wrapper::Equals(const RTTI* rhs) const 
	{
		if (rhs == nullptr) return false;

		const TestParseHandler::Wrapper* other = rhs->As<TestParseHandler::Wrapper>();
		return maxDepth == other->maxDepth;
	}

	/**
	 * @brief RTTI override for ToString
	 * @return string representation of this Wrapper
	*/
	std::string TestParseHandler::Wrapper::ToString() const 
	{
		return "TestParseHandler: maxDepth=" + std::to_string(maxDepth);
	}
#pragma endregion TestParseHandler::Wrapper

#pragma region TestParseHandler
	/*
	* @brief virtual constructor for TestParseHandler
	*/
	IParseHandler* TestParseHandler::create() const
	{
		return new TestParseHandler();
	}

	/**
	 * @brief Override for IParseHandler::Initialize
	*/
	void TestParseHandler::initialize() 
	{
		initialized = true;
	}

	/**
	 * @brief Override for IParseHandler::Start
	 * @param unused
	 * @param unused
	 * @param wrapper The wrapper to populate 
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::start(const std::string&, const Json::Value&, ParseCoordinator::Wrapper* wrapper) 
	{
		TestParseHandler::Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) return false;

		if (testWrapper->getDepth() >= testWrapper->maxDepth) 
		{
			testWrapper->maxDepth = testWrapper->getDepth();
		}
		++startCount;
		return true;
	}

	/**
	 * @brief Override for IParseHandler::End
	 * @param unused
	 * @param wrapper The wrapper to populate
	 * @return True, if handled, false otherwise
	*/
	bool TestParseHandler::end(const std::string&, ParseCoordinator::Wrapper* wrapper) 
	{
		Wrapper* testWrapper = wrapper->As<TestParseHandler::Wrapper>();
		if (testWrapper == nullptr) return false;

		++endCount;
		return true;
	}
#pragma endregion TestParseHandler
}
