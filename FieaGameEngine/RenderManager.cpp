#include "pch.h"
#include "RenderManager.h"
#include <iostream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

#define ESCAPE_KEY GLFW_KEY_ESCAPE
namespace Fiea::GameEngine
{
	RenderManager* RenderManager::silentInstance = nullptr;

	RenderManager* RenderManager::Instance()
	{
		if (silentInstance == nullptr)
		{
			silentInstance = new RenderManager();
		}
		return silentInstance;
	}
	

	/**
	 * @brief Init
	*/
	void RenderManager::Init(GLFWwindow* window)
	{
		assert(window != nullptr);

		this->window = window;

		// Load and cache VAO, all sprites are quads
		unsigned int VBO;
		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f // top left 
		};

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &(this->quadVAO));
		glBindVertexArray(this->quadVAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &(this->quadEBO));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//// dimensions are equal to screen size (optional) and data is NULL
		//// For this texture, we're only allocating memory and not actually filling it.
		//// Filling the texture will happen as soon as we render to the framebuffer.
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// Load shaders
		shader = new Shader("..\\FieaGameEngine\\vectorShader.vs", "..\\FieaGameEngine\\fragmentShader.fs");
		//postProcessShader = new Shader("..\\FieaGameEngine\\vectorShader.vs", "..\\FieaGameEngine\\PostProcessShader.fs");

		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

		UseShader();
		GLint projLoc = glGetUniformLocation(shader->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(projection));

		// init our post process
		postProcess.Init();
	}

	/**
	 * @brief 
	 * @param width 
	 * @param height 
	 * @param title 
	*/
	void RenderManager::InitWindow(int width, int height, const char* title)
	{
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (window == NULL)
		{
			glfwTerminate();
			throw std::logic_error("Failed to create GLFW window");
		}

		glfwMakeContextCurrent(window);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw std::logic_error("Failed to initialize GLAD");
		}

		glViewport(0, 0, width, height);

		shader = new Shader("..\\FieaGameEngine\\vectorShader.vs", "..\\FieaGameEngine\\fragmentShader.fs");

		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

		UseShader();
		GLint projLoc = glGetUniformLocation(shader->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(projection));
	}

	/**
	 * @brief Loads Image from file (file needs to be set to copy file) and creates a texture of it
	 * @param imageName : imageFile name OR path if not set to copy file
	 * @param includeAlpha : if using RGBA or just RGB
	 * @return texture created
	*/
	CTexture RenderManager::LoadImage(const char* imageName, GLuint& texture, bool loadNearest)
	{


		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering parameters depending on the passed boolean
		if (loadNearest)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}

		// set the texture wrapping parameters
		CTexture ctexture;
		int nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(imageName, &(ctexture.width), &(ctexture.height), & nrChannels, 0);
		if (data) {
			if (nrChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ctexture.width, ctexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ctexture.width, ctexture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else {
			throw std::runtime_error("Unable to read image file");
		}
		stbi_image_free(data);
		shader->use();
		glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);
		
		ctexture.textureRef = texture;
		return ctexture;
	}

	void RenderManager::renderSprite(glm::vec4& position, glm::vec4& rotation, glm::vec4& scale, glm::vec4& color, int frameIndex, int nRows, int nCols, const CTexture& tex)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex.textureRef);
		glBindVertexArray(quadVAO);

		// Create model matrix
		GLint modelLocation = glGetUniformLocation(shader->ID, "model");

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale.z, scale.w, 1.0f));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
			
		glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(model));

		GLint frameLocation = glGetUniformLocation(shader->ID, "frameIndex");
		GLint widthLocation = glGetUniformLocation(shader->ID, "nRows");
		GLint heightLocation = glGetUniformLocation(shader->ID, "nCols");
		glUniform1i(glGetUniformLocation(shader->ID, "frameIndex"), frameIndex);
		glUniform1i(glGetUniformLocation(shader->ID, "nRows"), nRows);
		glUniform1i(glGetUniformLocation(shader->ID, "nCols"), nCols);
		glUniform1f(glGetUniformLocation(shader->ID, "flashAmount"), color.a);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	/**
	 * @brief 
	*/
	void RenderManager::ProcessInput() {
		// Processing Escape input
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	/**
	 * @brief Compiles Vertex Shader
	 * @return true if successfully created and compiled, false otherwise
	*/
	bool RenderManager::CompileShaders()
	{
		const char* vertexShaderSource = "#version 330 core\n"
			"in vec3 aColor;\n"
			"in vec2 aTexCoord;\n"
			"out vec3 ourColor;\n"
			"out vec2 TexCoord;\n"
			"uniform vec3 aPos;\n"
			"void main() {\n"
			"	gl_Position = vec4(aPos, 1.0);\n"
			"	ourColor = aColor;\n"
			"	TexCoord = aTexCoord;\n"
			"}\0";

		// Creating vertex shader object
		GLuint vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Compiling Vertex Shader
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// Checking vertex shader compilation
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"

			"in vec3 ourColor;\n"
			"in vec2 TexCoord;\n"

			"uniform sampler2D Texture;\n"

			"void main()\n"
			"{\n"
			"	FragColor = texture(Texture, TexCoord);\n"
			"}\0";

		// Creating fragment shader object
		GLuint fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Compiling fragment Shader
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// Checking fragment shader compilation
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			throw std::logic_error("Could not link shader program");
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		std::cout << "Shaders compiled and linked successfully" << std::endl;
		return success;
	}

	void RenderManager::DrawQuad(bool background, GLuint& VAO, GLuint& VBO, GLuint& EBO)
	{
		float vertices[] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f // top left 
		};
		unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
		};

		if (VAO == 0) {
			glGenVertexArrays(1, &VAO);
		}
		if (VBO == 0) {
			glGenBuffers(1, &VBO);
		}
		if (EBO == 0) {
			glGenBuffers(1, &EBO);
		}

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// Unbind the VAO
		glBindVertexArray(0);
	}

	void RenderManager::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	void RenderManager::PollEvents()
	{
		glfwPollEvents();
	}

	void RenderManager::UseShader()
	{
		shader->use();
	}

	void RenderManager::Update(const GameTime& time)
	{
		postProcess.update(time);
	}

	/**
	 * @brief
	*/
	void RenderManager::Render()
	{
		postProcess.BeginRender();

		shader->use();
		glm::vec4 position, rotation, scale, color;
		glm::vec2 size;
		color = glm::vec4(1.0f);
		for (auto it = renderActions.begin(); it != renderActions.end(); ++it)
		{
			RenderAction* action = *it;
			if (!action->isVisible()) { continue; }

			position = action->getParentPosition();
			rotation = action->getParentRotation();
			scale = action->getParentScale();
			scale.x *= action->isMirrored() ? -1 : 1;
			color.a = action->getFlashWhite();

			// Pack size into scale since Z and W are unused
			size = action->getSize();
			scale.z = size.x;
			scale.w = size.y;

			const CTexture& tex = action->getTexture();

			renderSprite(position, rotation, scale,  color, action->getFrameIndex(), action->getNRows(), action->getNCols(), tex);
		}

		postProcess.EndRender();
		postProcess.Render();
	}

	/**
	 * @brief
	*/
	void RenderManager::Shutdown()
	{

	}

	/**
	 * @brief RegisterSprite adds a sprite to renderActions sorted based on draw
	 * @param action
	 * @return
	*/
	bool RenderManager::registerRenderAction(RenderAction* action)
	{
		int toInsert = action->getDrawOrder();

		for (auto it = renderActions.begin(); it != renderActions.end(); ++it)
		{
			if (toInsert < (*it)->getDrawOrder())
			{
				renderActions.insert(it, action);
				return true;
			}
		}

		renderActions.push_back(action);
		return true;
	}

	/**
	 * @brief Sets the flag that render actions need to be resorted, this frame
	 * prevents multiple sorts from occuring on a frame
	*/
	void RenderManager::setRenderActionNeedsSort()
	{
		needsSort = true;
		sortRenderActions();
	}


	void RenderManager::sortRenderActions()
	{
		auto texCompare = [&](RenderAction* tex, RenderAction* tex2) {return tex->getDrawOrder() < tex2->getDrawOrder(); };

		std::sort(renderActions.begin(), renderActions.end(), texCompare);
		needsSort = false;
	}

	bool RenderManager::windowClosing()
	{
		ProcessInput();
		return glfwWindowShouldClose(window);
	}
}