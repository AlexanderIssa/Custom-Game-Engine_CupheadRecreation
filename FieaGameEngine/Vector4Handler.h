#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "ParseCoordinator.h"
#include "IParseHandler.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Vector4Handler final : public IParseHandler
		{
		public:
			[[nodiscard]] virtual IParseHandler* create() const override;

			void initialize() override;
			bool start(const std::string& key, const Json::Value& value, ParseCoordinator::Wrapper* wrapper) override;
			bool end(const std::string& key, ParseCoordinator::Wrapper* wrapper) override;

			size_t startCount{ 0 };
			size_t endCount{ 0 };

		private:
			glm::vec4 stringToVec4(const std::string& value);
		};
	}
}
