#include "pch.h"
#include "RenderAction.h"
#include "TypeManager.h"
#include "RenderManager.h"
#include "GameClock.h"
#include "GameObject.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(RenderAction);

		/**
		 * @brief Default constructor
		*/
		RenderAction::RenderAction() : Action(TypeIdClass())
		{
			RenderManager::Instance()->registerRenderAction(this);
		}

		RenderAction::RenderAction(const RenderAction& other) : Action(other), texture(other.texture), sizeX(other.sizeX), sizeY(other.sizeY)
		{
			RenderManager::Instance()->registerRenderAction(this);
		}

		/**
		 * @brief Constructor taking in a texture
		 * @param texture 
		*/
		RenderAction::RenderAction(CTexture& texture) : Action(TypeIdClass())
		{
			assert(texture.width != 0);

			this->texture = texture;

			sizeX = (float)texture.width;
			sizeY = (float)texture.height;
			RenderManager::Instance()->registerRenderAction(this);
		}

		/**
		 * @brief Constructor taking in drawOrder and a texture
		 * @param drawOrder 
		 * @param texture 
		*/
		RenderAction::RenderAction(int drawOrder, CTexture& texture)
		{
			assert(texture.width != 0);

			this->texture = texture;
			this->texture.drawOrder = drawOrder;

			sizeX = (float)texture.width;
			sizeY = (float)texture.height;
			RenderManager::Instance()->registerRenderAction(this);
		}

		/*
		* @brief Clone function for the RenderAction class
		* @return RenderAction* - A pointer to the cloned RenderAction object
		*/
		RenderAction* RenderAction::clone() const
		{
			return new RenderAction(*this);
		}

		/**
		 * @brief Specialized constructor for child classes
		 * @param type
		*/
		RenderAction::RenderAction(IdType type) : Action(type)
		{
			RenderManager::Instance()->registerRenderAction(this);
		}

		RenderAction::RenderAction(int drawOrder, CTexture& texture, IdType type)
		{
			texture.drawOrder = drawOrder;
			this->texture = texture;

			RenderManager::Instance()->registerRenderAction(this);
		}

		/*
		* @brief Override of Action onEnable
		*/
		void RenderAction::onEnable()
		{
			visible = true;
		}

		/*
		* @brief Override of Action onDisable
		*/
		void RenderAction::onDisable()
		{
			visible = false;
		}

		/**
		 * @brief Override of Action Update
		 * @param time 
		*/
		void RenderAction::update(const GameTime& time)
		{
			if (flashTimer > 0.0f)
			{
				flashTimer -= time.Frame() * 0.001f;
				if (flashTimer <= 0.0f)
				{
					flashWhite = 0.0f;
				}
			}
		}

		std::vector<Signature> RenderAction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "int_textureRef"s, Datum::DataType::INTEGER, 1, offsetof(RenderAction, texture.textureRef)},
				{ "int_width"s, Datum::DataType::INTEGER, 1, offsetof(RenderAction, texture.width) },
				{ "int_height"s, Datum::DataType::INTEGER, 1, offsetof(RenderAction, texture.height) },
				{ "vec4_color"s, Datum::DataType::VEC4, 1, offsetof(RenderAction, texture.color) },
				{ "int_drawOrder"s, Datum::DataType::INTEGER, 1, offsetof(RenderAction, texture.drawOrder) },
				{ "float_sizeX"s, Datum::DataType::FLOAT, 1, offsetof(RenderAction, sizeX) },
				{ "float_sizeY"s, Datum::DataType::FLOAT, 1, offsetof(RenderAction, sizeY) }
			};
		}

		bool RenderAction::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		std::string RenderAction::ToString() const
		{
			return std::string("RenderAction");
		}

		void RenderAction::startFlash()
		{
			flashTimer = 0.15f;
			flashWhite = 0.25f;
		}

		void RenderAction::setDrawOrder(const int drawOrder)
		{
			texture.drawOrder = drawOrder;
			RenderManager::Instance()->setRenderActionNeedsSort(); 
		}
	}
}
