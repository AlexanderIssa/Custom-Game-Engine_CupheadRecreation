#pragma once
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea
{
	namespace GameEngine
	{
		class TestParseIntHandler final : public IParseHandler {
		public:
			class Wrapper : public ParseCoordinator::Wrapper
			{
				RTTI_DECLARATIONS(TestParseIntHandler::Wrapper, ParseCoordinator::Wrapper);

			public:
				Wrapper() = delete;
				Wrapper(int* i) : mInt(i) {}

				void setInt(int i);

				bool Equals(const RTTI* rhs) const;
				std::string ToString() const;
			private:
				int* mInt;
			};

			[[nodiscard]] virtual IParseHandler* create() const override;

			void initialize() override;
			bool start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;
			bool end(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

			size_t count {0};
		};
	}	
}
