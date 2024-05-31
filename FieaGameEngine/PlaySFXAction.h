#pragma once
#include "Action.h"

namespace Fiea
{
	namespace GameEngine
	{
		#define MAX_NUMBER_OF_RANDOM_SOUNDS 25

		class PlaySFXAction final : public Action
		{
			RTTI_DECLARATIONS(PlaySFXAction, Action);
		public:
			PlaySFXAction();
			// inherited from attributed
			static std::vector<Signature> prescribedAttributes();
			// inherited from Action
			[[nodiscard]] virtual PlaySFXAction* clone() const override;
			virtual void onEnable() override;
			virtual void onDisable() override;
			virtual void update(const GameTime&) override;
		private:
			std::string mFilename;
			float mVolume;
		};

		class PlaySFXRandomAction final : public Action
		{
			RTTI_DECLARATIONS(PlaySFXRandomAction, Action);
		public:
			PlaySFXRandomAction();
			// inherited from attributed
			static std::vector<Signature> prescribedAttributes();
			// inherited from Action
			[[nodiscard]] virtual PlaySFXRandomAction* clone() const override;
			virtual void onEnable() override;
			virtual void onDisable() override;
			virtual void update(const GameTime&) override;
		private:
			int mNumberOfSounds;
			std::string mFilenames[MAX_NUMBER_OF_RANDOM_SOUNDS];
			float mVolume;
		};

	}
}