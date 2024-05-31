#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include "Shader/Shader.h"
#include "RenderAction.h"
#include "PostProcess.h"

namespace Fiea::GameEngine
{

	using textureRef = size_t;

	class RenderManager
	{
	public:
		static RenderManager* Instance();

		void Init(GLFWwindow* window);
		void InitWindow(int width, int height, const char* title);
		CTexture LoadImage(const char* imageName, GLuint& texture, bool loadNearest = false);

		void Update(const GameTime& time);
		void Render();
		void Shutdown();

		bool registerRenderAction(RenderAction* action);
		void setRenderActionNeedsSort();

		bool windowClosing();
		void DrawQuad(bool background, GLuint& VAO, GLuint& VBO, GLuint& EBO);
		void SwapBuffers();
		void PollEvents();
		void UseShader();
		void SetShaderUniform(const char* uniform, GLuint texture);

		void sortRenderActions();
	private:
		RenderManager() = default;
		RenderManager(const RenderManager& other) = delete;

		// TODO: add parameters and implement
		void renderSprite(glm::vec4& position, glm::vec4& rotation, glm::vec4& scale, glm::vec4& color, int frameIndex, int nRows, int nCols, const CTexture& tex);
		void DrawBackground(const GLuint& texId);
		void ProcessInput();
		bool CompileShaders();

		GLFWwindow* window;
		GLuint shaderProgram;
		Shader* shader;
		static RenderManager* silentInstance;

		std::vector<RenderAction*> pendingAdd;
		std::vector<RenderAction*> renderActions;
		std::vector<RenderAction*> graveYard;

		// actions state
		bool needsSort{ false };

		// Cache VAO and EBO for later
		GLuint quadVAO;
		GLuint quadEBO;

		// post process vars
		unsigned int framebuffer;
		unsigned int textureColorbuffer;

		PostProcess postProcess;
	};
}