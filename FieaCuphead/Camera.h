#pragma once
#include "Action.h"

namespace Fiea::GameEngine
{
	class Camera : public Action 
	{
		RTTI_DECLARATIONS(Camera, Action);
	protected:
		Camera(IdType idType);
	public:
		Camera();

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		virtual std::string ToString() const override;

	private:

		float moveSpeed;
		glm::vec4 leftPos;
		glm::vec4 rightPos;
	};

}