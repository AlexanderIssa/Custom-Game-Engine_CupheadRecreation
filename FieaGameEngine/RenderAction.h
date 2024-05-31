#pragma once
#include "Action.h"

namespace Fiea
{
	namespace GameEngine
	{
		struct CTexture
		{
			unsigned int textureRef;
			int width;
			int height;
			glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
			int drawOrder{0};
		};

		class RenderAction : public Action
		{
			RTTI_DECLARATIONS(RenderAction, Action);

		protected:
			// Specialized constructor for child classes to set the type of the object
			RenderAction(IdType type);
			RenderAction(int drawOrder, CTexture&texture, IdType type);

		public:
			RenderAction();
			RenderAction(const RenderAction& other);
			RenderAction(CTexture& texture);
			RenderAction(int drawOrder, CTexture& texture);

			// Cloneable interface
			[[nodiscard]] virtual RenderAction* clone() const override;

			virtual ~RenderAction() override = default;

			virtual void onEnable() override;

			virtual void onDisable() override;

			virtual void update(const GameTime& time) override;

			static std::vector<Signature> prescribedAttributes();

			// Register prescribed attributes
			static bool registerSignatures();

			// RTTI Overrides
			virtual std::string ToString() const override;
			void startFlash();

			// Getters and setters
			virtual CTexture getTexture() { return texture; }
			unsigned int getTextureRef() const { return texture.textureRef; }
			int getWidth() const { return texture.width; }
			int getHeight() const { return texture.height; }
			glm::vec4 getColor() const { return texture.color; }
			int getDrawOrder() const { return texture.drawOrder; }
			bool isVisible() const { return visible; }
			bool isMirrored() const { return mirror; }
			glm::vec2 getSize() const { return glm::vec2(sizeX, sizeY); }
			virtual int getNRows() const { return 1; }
			virtual int getNCols() const { return 1; }
			virtual int getFrameIndex() const { return 0; }
			float getFlashWhite() const { return flashWhite; }

			void setTexture(const CTexture& t) { texture = t; }
			void setTextureRef(const unsigned int ref) { texture.textureRef = ref; }
			void setWidth(const int width) { texture.width = width; }
			void setHeight(const int height) { texture.height = height; }
			void setColor(const glm::vec4 color) { texture.color = color; }
			void setDrawOrder(const int drawOrder);
			void setVisible(const bool visible) { this->visible = visible; }
			void setMirror(const bool newMirror) { mirror = newMirror; }
			void setSize(float x, float y) { sizeX = x; sizeY = y; }
			void setSize(glm::vec2 size) { sizeX = size.x; sizeY = size.y; }
			void setFlashWhite(float flash ) { flashWhite = flash; }

		protected:
			CTexture texture;

			float sizeX;
			float sizeY;
			float flashWhite{ 0.0f };
			float flashTimer = 0.0f;

			// state variables
			bool visible{ true };
			bool mirror{ false };
		};
	}
}
