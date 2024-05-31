#include "UIElement.h"
#include <iostream>
#include <GLFW/glfw3.h>

namespace Fiea::GameEngine
{
    UIElement::UIElement(const char* imagePath, glm::vec2 position, glm::vec2 size)
        : position(position), size(size) {
        loadTexture(imagePath);
        setupMesh();

        shader = new Shader("..\\FieaGameEngine\\UIVertexShader.vs", "..\\FieaGameEngine\\UIFragmentShader.fs");
    }

    UIElement::~UIElement() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &texture);
    }

    void UIElement::loadTexture(const char* imagePath) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void UIElement::setupMesh() {
        float vertices[] = {
            // position          // texture coordinate
            position.x + size.x, position.y + size.y, 0.0f, 1.0f, 1.0f,
            position.x + size.x, position.y, 0.0f, 1.0f, 0.0f,
            position.x, position.y, 0.0f, 0.0f, 0.0f,
            position.x, position.y + size.y, 0.0f, 0.0f, 1.0f
        };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void UIElement::draw() {
        //// Make sure UI appears at the top
        GLboolean depthTestEnabled; 
        glGetBooleanv(GL_DEPTH_TEST, &depthTestEnabled); 
        if (depthTestEnabled) { 
            glDisable(GL_DEPTH_TEST); 
        }
        
        this->setupMesh();
        shader->use();

        glBindTexture(GL_TEXTURE_2D, texture); 
        glBindVertexArray(VAO); 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

        glBindVertexArray(0);  // Unbind VAO
        glBindTexture(GL_TEXTURE_2D, 0);  // Unbind texture


        // Restore Depth Test
        if (depthTestEnabled) {
            glEnable(GL_DEPTH_TEST); 
        }
    }

}