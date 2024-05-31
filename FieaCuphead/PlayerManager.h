#pragma once
#include "GameObject.h"

using namespace Fiea::GameEngine;

namespace Fiea::Cuphead
{
	class PlayerManager
	{
	public:
		/**
		 * @brief Registers one character with the player manager
		 * @param player 
		*/
		static void RegisterCharacter(GameObject* player);

		/**
		 * @brief GetPlayer returns the player
		 * @param idx for potential coop use unsed for now
		 * @return 
		*/
		static GameObject* GetPlayer(int idx = 0);

		/**
		 * @brief position of the player
		 * @return 
		*/
		static glm::vec4 Position();

	private:
		PlayerManager();

		static GameObject* _player;
	};
}