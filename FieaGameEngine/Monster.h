#pragma once
#include <vector>
#include "GameObject.h"

namespace Fiea
{
	namespace GameEngine
	{
		class Monster final : public GameObject
		{
			RTTI_DECLARATIONS(Monster, GameObject);

		public:

			// Default constructor
			Monster();

			// Copy constructor
			Monster(const Monster& other) = default;

			// Move constructor
			Monster(Monster&& other) noexcept = default;

			// Copy assignment operator
			Monster& operator=(const Monster& rhs) = default;

			// Move assignment operator
			Monster& operator=(Monster&& rhs) noexcept = default;

			// Destructor
			virtual ~Monster() override = default;

			// Get the prescribed attributes of the Monster class
			static std::vector<Signature> prescribedAttributes();

			//RTTI Overrides
			virtual std::string ToString() const override;

			std::string tag { "Monster" };
		};
	}
}
