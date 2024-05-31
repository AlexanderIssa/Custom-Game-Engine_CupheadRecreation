#pragma once
#include "AnimRenderAction.h"
#include "EventDispatcher.h"

namespace Fiea
{
	namespace GameEngine
	{
		class HealthRenderAction : public AnimRenderAction
		{
			RTTI_DECLARATIONS(HealthRenderAction, AnimRenderAction);

		public:

			// Default constructor
			HealthRenderAction();

			// Parameterized constructor
			HealthRenderAction(int frameRate, int nRows, int nCols, CTexture& tex);

			// Copy constructor
			HealthRenderAction(const HealthRenderAction& other);
			
			// Destructor
			~HealthRenderAction();
			
			// Update the render action
			void update(const GameTime& time) override;

			// Clone the render action
			[[nodiscard]] HealthRenderAction* clone() const override;

			static std::vector<Signature> prescribedAttributes();

			static bool registerSignatures();

		private:
			int currentHealth = 3;
			EventCallback* myDamageTakenCallback;

			void UpdateHealth();
			void OnDamageTakenEvent(IEvent* event);
		};
	}
}