#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <STB/stb_image.h>
#include "GameObject.h"
#include "Shader/Shader.h"

namespace Fiea::GameEngine
{
    class UIElement : GameObject {
    public:
        UIElement(const char* imagePath, glm::vec2 position, glm::vec2 size);
        ~UIElement();

        void draw();
        void setupMesh();
        void drawTest();

        glm::vec2 position;
        glm::vec2 size;
        
    private:
        Shader* shader;
        unsigned int texture;
        unsigned int VBO, VAO, EBO;
        void loadTexture(const char* imagePath); 
    };
}
