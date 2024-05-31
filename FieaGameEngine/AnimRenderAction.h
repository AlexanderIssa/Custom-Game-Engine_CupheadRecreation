#pragma once
#include "RenderAction.h"

namespace Fiea
{
	// Assuming a sprite sheet is a single row of many animations every cell is the same width and height
	// Y will range from 0/1
	// X varies

	namespace GameEngine
	{
		struct SpriteAnim
		{
			unsigned int startingCell{ 0 };
			unsigned int nFrames{ 1 };
		};

		class AnimRenderAction : public RenderAction
		{
			RTTI_DECLARATIONS(AnimRenderAction, RenderAction);
		protected:
			// Specialized constructor for child classes to set the type of the object
			AnimRenderAction(IdType type);
			AnimRenderAction(int frameRate, int nRows, int nCols, CTexture& tex, IdType type);

		public:

			// Default constructor
			AnimRenderAction();
			AnimRenderAction(const AnimRenderAction& other);
			AnimRenderAction(int frameRate, int nRows, int nCols, CTexture& tex);
			AnimRenderAction(int drawOrder, int frameRate, int nRows, int nCols, CTexture& tex);
			virtual ~AnimRenderAction() = default;

			// Cloneable interface
			[[nodiscard]] virtual AnimRenderAction* clone() const override;

			void onEnable() override;
			void onDisable() override;
			virtual void update(const GameTime& time) override;
			void TestUpdate();

			static std::vector<Signature> prescribedAttributes();

			static bool registerSignatures();

			virtual std::string ToString() const override;

			// getters and setters
			CTexture getTexture() override;
			SpriteAnim getAnim() const { return anim; }
			int getFrameRate() const { return frameRate; }
			int getNRows() const override { return nRows; }
			int getNCols() const override { return nCols; }
			int getFrameIndex() const override;

			int getLoopCount() const { return loopCount; }

			bool isAnimComplete() const { return animComplete; }

			float getCurrentFrame() const { return currFrame; }


			void setAnim(SpriteAnim& anim, bool restart = false);
			void setFrameRate(int newRate) { frameRate = newRate; }
			void setNFrames(int frames) { anim.nFrames = frames; }
			void setCols(int cols) { nCols = cols; }
			void setRows(int rows) { nRows = rows; }

			void setCurrentFrame(float frame) { currFrame = frame; }

			void setLoopLimit(int limit) { loopLimit = limit; }

			void resetLoopCount() { loopCount = 0; }

			void resetAnimCompletion() { animComplete = false; }



		protected:
			SpriteAnim anim;
			int frameRate;
			int nCols;
			int nRows;
			int cellWidth;
			int cellHeight;
			float currFrame;

			int loopCount = 0;
			int loopLimit = -1;
			bool animComplete = false;
		};
	}
}