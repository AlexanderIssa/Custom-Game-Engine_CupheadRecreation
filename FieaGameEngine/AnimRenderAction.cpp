#include "pch.h"
#include "AnimRenderAction.h"
#include "TypeManager.h"
#include "GameClock.h"
#include <math.h>
#include <Windows.h>
using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		RTTI_DEFINITIONS(AnimRenderAction);

		/**
		 * @brief Default constructor
		*/
		AnimRenderAction::AnimRenderAction() : RenderAction(TypeIdClass())
		{
		}

		AnimRenderAction::AnimRenderAction(const AnimRenderAction& other) : RenderAction(other),
			anim(other.anim), frameRate(other.frameRate), nCols(other.nCols), nRows(other.nRows), cellWidth(other.cellWidth), cellHeight(other.cellHeight), currFrame(other.currFrame)
		{
		}

		/**
		 * @brief AnimRenderAction constructor
		 * @param frameRate target framerate
		 * @param nRows number of rows
		 * @param nCols number of columns
		 * @param tex texture to use
		*/
		AnimRenderAction::AnimRenderAction(int frameRate, int nRows, int nCols, CTexture& tex) : RenderAction(TypeIdClass())
		{
			assert(tex.width != 0);
			assert(tex.width != 0);

			this->frameRate = frameRate;
			this->nRows = nRows;
			this->nCols = nCols;

			cellWidth = tex.width / nCols;
			cellHeight = tex.height / nRows;

			sizeX = (float)cellWidth;
			sizeY = (float)cellHeight;

			texture = tex;
		}
		AnimRenderAction::AnimRenderAction(int frameRate, int nRows, int nCols, CTexture& tex, IdType type) : RenderAction(type)
		{
			assert(tex.width != 0);
			assert(tex.width != 0);

			this->frameRate = frameRate;
			this->nRows = nRows;
			this->nCols = nCols;

			cellWidth = tex.width / nCols;
			cellHeight = tex.height / nRows;

			sizeX = (float)cellWidth;
			sizeY = (float)cellHeight;

			texture = tex;
		}

		AnimRenderAction::AnimRenderAction(int drawOrder, int frameRate, int nRows, int nCols, CTexture& tex) : RenderAction(drawOrder, tex, TypeIdClass())
		{
			assert(tex.width != 0);
			assert(tex.width != 0);

			this->frameRate = frameRate;
			this->nRows = nRows;
			this->nCols = nCols;

			cellWidth = tex.width / nCols;
			cellHeight = tex.height / nRows;

			sizeX = (float)cellWidth;
			sizeY = (float)cellHeight;
		}

		/**
		 * @brief Specialzied constructor for child classes to set the type
		 * @param type
		*/
		AnimRenderAction::AnimRenderAction(IdType type) : RenderAction(type)
		{
		}


		/*
		* @brief Clone function for AnimRenderAction
		* @return - A new AnimRenderAction object
		*/
		AnimRenderAction* AnimRenderAction::clone() const
		{
			return new AnimRenderAction(*this);
		}

		void AnimRenderAction::onEnable()
		{
			RenderAction::onEnable();
		}

		void AnimRenderAction::onDisable()
		{
			RenderAction::onDisable();
		}

		/**
		 * @brief Override of Action update. Calculates the current frame of animation and uv position
		 * @param  
		*/
		void AnimRenderAction::update(const GameTime& time)
		{
			RenderAction::update(time);

            // If there is only one frame there is no need to update
            if (animComplete || anim.nFrames <= 1) { return; }

            // depends on time unit what we do here
            currFrame += frameRate * time.Frame() * 0.001f;

			bool didWhile = false;
            while (currFrame >= anim.nFrames)
            {
				didWhile = true;
                currFrame = 0.0f;
            }

			if (didWhile) { ++loopCount; }
			if (loopLimit != -1 && loopCount >= loopLimit)
			{
				// If we are not looping and we have looped once, we are done
				// so we set the current frame to the last frame
				animComplete = true;
			}

		}

		void AnimRenderAction::TestUpdate()
		{
			// If there is only one frame there is no need to update
			if (anim.nFrames <= 1) { return; }

			// depends on time unit what we do here
			currFrame += frameRate * 0.01f;

			while (currFrame >= anim.nFrames)
			{
				currFrame -= anim.nFrames;
			}

			// Determine the tex coords for this frame
			unsigned int frame = anim.startingCell + (int)currFrame;
		}

		/**
		 * @brief Prescribed attributed for this object
		 * @return 
		*/
		std::vector<Signature> AnimRenderAction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{"int_startingCell"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, anim.startingCell)},
				{"int_frameRate"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, frameRate)},
				{"int_nFrames"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, anim.nFrames)},
				{"int_nCols"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, nCols)},
				{"int_nRows"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, nRows)},
				{"int_cellWidth"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, cellWidth)},
				{"int_cellHeight"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, cellHeight)},
				{"float_currFrame"s, Datum::DataType::FLOAT, 1, offsetof(AnimRenderAction, currFrame)},
				{"int_loopCount"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, loopCount)},
				{"int_loopLimit"s, Datum::DataType::INTEGER, 1, offsetof(AnimRenderAction, loopLimit)}
			};
		}

		/**
		 * @brief Register prescribed attributes with the type manager
		 * @return 
		*/
		bool AnimRenderAction::registerSignatures()
		{
			static bool registered = []()
			{
				TypeManager::registerType(TypeIdClass(), prescribedAttributes());
				return true;
			}();
			return registered;
		}

		std::string AnimRenderAction::ToString() const
		{
			return std::string();
		}

		CTexture AnimRenderAction::getTexture()
		{
			CTexture temp = texture;
			temp.width = cellWidth;
			temp.height = cellHeight;
			return temp;
		}

		int AnimRenderAction::getFrameIndex() const
		{
			return (int)floor(currFrame) + anim.startingCell;
		}

		/**
		 * @brief SetAnim sets the current anim information
		 * @param anim 
		 * @param restart 
		*/
		void AnimRenderAction::setAnim(SpriteAnim& anim, bool restart)
		{
			if (restart)
				currFrame = 0.0f;

			this->anim = anim;
			this->anim.startingCell -= 1;
		}
	}
}
