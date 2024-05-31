#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform int frameIndex;
uniform int nRows;
uniform int nCols;
uniform float flashAmount;

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

    vec4 color = texture2D(texture1, uv);

    //float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    vec4 whiteColor = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 desaturatedColor = mix(color, whiteColor, flashAmount);

    //FragColor = texture2D(texture1, uv);
    FragColor = vec4(desaturatedColor.rgb, color.a);
}

