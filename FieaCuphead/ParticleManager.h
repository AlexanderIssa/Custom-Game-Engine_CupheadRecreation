#pragma once
#include "ParticleRenderAction.h"
#include "ObjectPool.h"

namespace Fiea::GameEngine
{
	class GameObject;

	class ParticleManager
	{
	public:
		static void Init(const std::string& prefab);
		static GameObject* getParticle();
		static void shutdown();


	private:
		static ObjectPool* particlePool;

	};
}