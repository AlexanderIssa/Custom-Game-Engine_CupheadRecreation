#pragma once
#include "RenderAction.h"
#include "AnimRenderAction.h"

namespace Fiea::GameEngine
{
	class ParticleRenderAction : public RenderAction
	{
		RTTI_DECLARATIONS(ParticleRenderAction, RenderAction);

	protected:
		ParticleRenderAction(IdType type);

	public:
		ParticleRenderAction();
		ParticleRenderAction(const ParticleRenderAction& other);
		virtual ~ParticleRenderAction();

		[[nodiscard]] virtual RenderAction* clone() const override;

		virtual void onEnable() override;

		virtual void onDisable() override;

		virtual void update(const GameTime& time) override;

		static std::vector<Signature> prescribedAttributes();

		// Register prescribed attributes
		static bool registerSignatures();

		virtual std::string ToString() const override;

		virtual int getFrameIndex() const override;
		virtual int getNCols() const override { return nCols; }
		
		void SetTextureAndAnim(CTexture& tex, SpriteAnim& anim);
	protected:
		SpriteAnim _anim;

		float currFrame;
		int nCols;

		float jeffVariable = 0;
	};
}