#include "PlayerManager.h"

namespace Fiea::Cuphead
{
	GameObject* PlayerManager::_player = nullptr;

	void PlayerManager::RegisterCharacter(GameObject* player)
	{
		_player = player;
	}

	GameObject* PlayerManager::GetPlayer(int idx)
	{
		return _player;
	}

	glm::vec4 PlayerManager::Position()
	{
		if (_player == nullptr) { return glm::vec4(0.0f); }

		return _player->position;
	}
}
