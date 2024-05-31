#include "HealthRenderAction.h"
#include "CupheadEvents.h"
#include "TypeManager.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(HealthRenderAction);

		/*
		* @brief Default constructor
		*/
		HealthRenderAction::HealthRenderAction() : AnimRenderAction(TypeIdClass())
		{
			auto bindedFooFunction = BIND_FUNC(this, HealthRenderAction::OnDamageTakenEvent);
			myDamageTakenCallback = new EventCallback(bindedFooFunction);
			EventDispatcher::addListener<OnCupheadTakeDamageEvent>(myDamageTakenCallback);
			UpdateHealth();
		}

		/*
		* @brief Parameterized constructor
		* @param frameRate: The frame rate of the animation
		* @param nRows: The number of rows in the texture
		* @param nCols: The number of columns in the texture
		* @param tex: The texture to use
		*/
		HealthRenderAction::HealthRenderAction(int frameRate, int nRows, int nCols, CTexture& tex) :
			AnimRenderAction(frameRate, nRows, nCols, tex, TypeIdClass())
		{
			auto bindedFooFunction = BIND_FUNC(this, HealthRenderAction::OnDamageTakenEvent);
			myDamageTakenCallback = new EventCallback(bindedFooFunction);
			EventDispatcher::addListener<OnCupheadTakeDamageEvent>(myDamageTakenCallback);
			UpdateHealth();
		}

		/*
		* @brief Copy constructor
		* @param other: The HealthRenderAction to copy
		*/
		HealthRenderAction::HealthRenderAction(const HealthRenderAction& other) : currentHealth(other.currentHealth), AnimRenderAction(other)
		{
			auto bindedFooFunction = BIND_FUNC(this, HealthRenderAction::OnDamageTakenEvent);
			myDamageTakenCallback = new EventCallback(bindedFooFunction);
			EventDispatcher::addListener<OnCupheadTakeDamageEvent>(myDamageTakenCallback);
			UpdateHealth();
		}

		/*
		* @brief Destructor
		*/
		HealthRenderAction::~HealthRenderAction()
		{
			delete myDamageTakenCallback;
		}

		/*
		* @brief Update the render action
		* @param time: The GameTime object
		* @param gameObject: The GameObject to update
		*/
		void HealthRenderAction::update(const GameTime& time)
		{
			AnimRenderAction::update(time);
		}

		/*
		* @brief Clone the render action
		* @return HealthRenderAction*: The cloned render action
		*/
		HealthRenderAction* HealthRenderAction::clone() const
		{
			return new HealthRenderAction(*this);
		}

		/*
		* @brief Get the prescribed attributes
		* @return std::vector<Signature>: The prescribed attributes
		*/
		std::vector<Signature> HealthRenderAction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 }
			};
		}

		/*
		* @brief Get the prescribed attributes
		* @return std::vector<Signature>: The prescribed attributes
		*/
		bool HealthRenderAction::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		/*
		* @brief Set the current health
		*/
		void HealthRenderAction::UpdateHealth()
		{
			switch (currentHealth)
			{
				case 0:
					anim.nFrames = 1;
					anim.startingCell = 0;
					break;
				case 1:
					anim.nFrames = 3;
					anim.startingCell = 1;
					break;
				case 2:
					anim.nFrames = 1;
					anim.startingCell = 4;
					break;
				case 3:
					anim.nFrames = 1;
					anim.startingCell = 5;
					break;
				default:
					anim.nFrames = 1;
					anim.startingCell = 0;
					break;
			}
		}

		/*
		* @brief Event handler for when damage is taken
		* @param event: The event that was triggered
		*/
		void HealthRenderAction::OnDamageTakenEvent(IEvent* eventData)
		{
			if (eventData == nullptr)
			{
				throw std::runtime_error("eventData cannot be null.");
			}

			OnCupheadTakeDamageEvent* data = eventData->As<OnCupheadTakeDamageEvent>();

			// Check if the data is of the correct type, this should always be true though
			if (data == nullptr)
			{
				throw std::runtime_error("Invalid event data type.");
			}

			// Update the health
			currentHealth = data->health;
			UpdateHealth();
		}
	}
}