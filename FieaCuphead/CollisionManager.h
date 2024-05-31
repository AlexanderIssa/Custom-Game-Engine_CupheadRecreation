#pragma once
#include "Collision.h"
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader/Shader.h"

namespace Fiea::GameEngine
{
	class CollisionManager final
	{
	public:
		
		static void Init();
		static void registerCollider(AbstractCollider* collider);
		static void removeCollider(AbstractCollider* collider);
		static void processCollisions(const GameTime& time);

		static void drawDebugBoxes();

	private:
		static std::vector<AbstractCollider*> colliders;
		static unsigned int VBO, VAO;
		static Shader* lineShader;
	};
}