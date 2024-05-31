#include "ParticleManager.h"

namespace Fiea::GameEngine
{
	ObjectPool* ParticleManager::particlePool = nullptr;

	void ParticleManager::Init(const std::string& prefab)
	{
		particlePool = new ObjectPool(prefab, 24);
	}

	GameObject* ParticleManager::getParticle()
	{
		return particlePool->GetPooledObject();
	}

	void ParticleManager::shutdown()
	{
		delete particlePool;
	}

}
