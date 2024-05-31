#include "CollisionManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>


namespace Fiea::GameEngine
{
	std::vector<AbstractCollider*> CollisionManager::colliders;

	unsigned int CollisionManager::VAO;
	unsigned int CollisionManager::VBO;

	Shader* CollisionManager::lineShader;

	void CollisionManager::Init()
	{			
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, NULL, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		lineShader = new Shader("..\\FieaGameEngine\\lineVertexShader.vs", "..\\FieaGameEngine\\lineFragmentShader.fs");
	}

	/**
	 * @brief Adds a collider to the list of colliders
	 * @param collider 
	*/
	void CollisionManager::registerCollider(AbstractCollider* collider)
	{
		if (collider == nullptr) { throw std::runtime_error("Collider can't be nullptr!"); }

		for (auto c : colliders)
		{
			if (c == collider) { return; }
		}

		colliders.push_back(collider);
	}

	/**
	 * @brief Removes a collider from the list of colliders
	 * @param collider 
	*/
	void CollisionManager::removeCollider(AbstractCollider* collider)
	{
		colliders.erase(std::remove_if(
			colliders.begin(), colliders.end(),
			[&](const AbstractCollider* c) {return collider == c; }),
			colliders.end());
	}

	/**
	 * @brief Iterates over all colliders and checks for a collision
	 * -- THIS IS NOT OPTIMIZED
	*/
	void CollisionManager::processCollisions(const GameTime& time)
	{
		size_t nColliders = colliders.size();
		for (size_t i = 0; i < nColliders - 1; ++i)
		{	
			// Check if their active and do something
			for (size_t j = i + 1; j < nColliders; ++j)
			{
				if (!colliders[i]->processCollision(colliders[j], time)) { continue; }
			}
		}
	}

	void CollisionManager::drawDebugBoxes()
	{
		lineShader->use();
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
		GLint projLocation = glGetUniformLocation(lineShader->ID, "projection");
		glUniformMatrix4fv(projLocation, 1, false, glm::value_ptr(projection));

		for (size_t i = 0; i < colliders.size(); ++i)
		{
			AABB temp = colliders[i]->getAABB();
			float vertices[16] = { temp.minx, temp.miny,
				temp.minx, temp.maxy, 
				temp.minx, temp.maxy, 
				temp.maxx, temp.maxy, 
				temp.maxx, temp.maxy, 
				temp.maxx, temp.miny, 
				temp.maxx, temp.miny,
				temp.minx, temp.miny
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, vertices, GL_STATIC_DRAW);
			glDrawArrays(GL_LINES, 0, 8);
		}
	}
}
