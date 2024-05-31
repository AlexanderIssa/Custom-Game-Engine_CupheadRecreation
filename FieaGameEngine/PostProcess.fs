#version 330 core

// Outputs the color for the Fragment Shader
out vec4 FragColor;

// Inputs the color and texture from the vector shader
in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D sceneTexture;

// Animation information
uniform int frameIndex;
uniform int nRows;
uniform int nCols;

void main()
{
    float frameWidth = 1.0 / nCols;
    float frameHeight = 1.0 / nRows;

    int xPos = frameIndex % nCols;
    int yPos = frameIndex / nCols;

    float xOffset = frameWidth * xPos;
    float yOffset = frameHeight * yPos;

    float x = xOffset + (TexCoord.x * frameWidth);
    float y = (1 - yOffset) - ((1.0 - TexCoord.y) * frameHeight);

    vec2 uv = vec2(x, y);

    // Multiply over scene texture
    // Create a vec4 out of the scene texture
    vec4 sceneColor = texture(sceneTexture, TexCoord);
    
    // Create a vec4 out of the post process texture
    vec4 bwColor = texture(texture1, uv);

    // the post process animation is color inverted from the sprite sheet
    vec4 inversebwColor = 1 - bwColor;

    // the resulting color is the result of the scene color + the post process sprite sheet texture
    vec4 color = (sceneColor * 1.0) + inversebwColor;
    FragColor = color;
}