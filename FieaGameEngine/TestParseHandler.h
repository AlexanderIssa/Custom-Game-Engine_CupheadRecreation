/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea::GameEngine::test
{
	class TestParseHandler final : public IParseHandler {
	public:
		class Wrapper : public ParseCoordinator::Wrapper 
		{
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, ParseCoordinator::Wrapper);

		public:
			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;
		};

		[[nodiscard]] virtual IParseHandler* create() const override;

		void initialize() override;
		bool start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;
		bool end(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

		size_t startCount{ 0 };
		size_t endCount{ 0 };
	};
}

