#include "pch.h"
#include "PlaySFXAction.h"
#include "AudioManager.h"

using namespace std::string_literals;

namespace Fiea
{
	namespace GameEngine
	{
		// ----------------------------------------------------------------------------------
		// ---------------------------------- PlaySFXAction ---------------------------------
		// ----------------------------------------------------------------------------------
		RTTI_DEFINITIONS(PlaySFXAction);

		PlaySFXAction::PlaySFXAction() : Action(PlaySFXAction::TypeIdClass()), mFilename("DefaultFileName"), mVolume(1)
		{
		}

		std::vector<Signature> PlaySFXAction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "str_filename"s, Datum::DataType::STRING, 1, offsetof(PlaySFXAction, mFilename) },
				{ "float_volume"s, Datum::DataType::FLOAT, 1, offsetof(PlaySFXAction, mVolume) }
			};
		}
		PlaySFXAction* PlaySFXAction::clone() const
		{
			return new PlaySFXAction(*this);
		}
		void PlaySFXAction::onEnable()
		{
		}
		void PlaySFXAction::onDisable()
		{
		}
		void PlaySFXAction::update(const GameTime&)
		{
			AudioManager::PlaySFX2D(mFilename, mVolume);
		}

		// ----------------------------------------------------------------------------------
		// ---------------------------------- PlaySFXRandomAction ---------------------------
		// ----------------------------------------------------------------------------------
		RTTI_DEFINITIONS(PlaySFXRandomAction);

		PlaySFXRandomAction::PlaySFXRandomAction() : Action(PlaySFXRandomAction::TypeIdClass()), mNumberOfSounds(-1), mFilenames{}, mVolume(1)
		{
		}

		std::vector<Signature> PlaySFXRandomAction::prescribedAttributes()
		{
			return std::vector<Signature>
			{
				{ "this"s, Datum::DataType::POINTER, 1, 0 },
				{ "int_numberOfSounds"s, Datum::DataType::INTEGER, 1, offsetof(PlaySFXRandomAction, mNumberOfSounds) },
				{ "str_filenames"s, Datum::DataType::STRING, MAX_NUMBER_OF_RANDOM_SOUNDS, offsetof(PlaySFXRandomAction, mFilenames) },
				{ "float_volume"s, Datum::DataType::FLOAT, 1, offsetof(PlaySFXRandomAction, mVolume) }
			};
		}
		PlaySFXRandomAction* PlaySFXRandomAction::clone() const
		{
			return new PlaySFXRandomAction(*this);
		}
		void PlaySFXRandomAction::onEnable()
		{
		}
		void PlaySFXRandomAction::onDisable()
		{
		}
		void PlaySFXRandomAction::update(const GameTime&)
		{
			// error check: something wrong happened during parsing
			if (mNumberOfSounds == -1)
			{
				assert(false && "Error in PlaySFXRandomAction::update. Number of sounds was not set in JSON.");
				throw std::logic_error("Error in PlaySFXRandomAction::update. Number of sounds was not set in JSON.");
			}
			int randomInt = rand() % mNumberOfSounds;
			AudioManager::PlaySFX2D(mFilenames[randomInt], mVolume);
		}
	}
}