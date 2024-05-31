#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include "Shader/Shader.h"
#include "RenderAction.h"

namespace Fiea::GameEngine
{
	class PostProcess
	{
	public:
		/// @brief Init stuff
		void Init();

		/// @brief Init VAO
		void InitVAOVBO();

		// Render processes Begin->End->Render
		void BeginRender();
		void Render();
		void EndRender();

		void update(const GameTime& time);

	private:
		unsigned int VAO, VBO, FBO, TCB, RBO, RFBO;
		Shader* PostProcessShader;
		GLuint ppText;
		CTexture ppImg;

		float currFrame = 0.0f;
	};
}

