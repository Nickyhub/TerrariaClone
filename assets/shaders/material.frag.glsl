#version 450 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D backgroundTexture;

void main()
{
    FragColor = texelFetch(backgroundTexture, ivec2(TexCoord.x, TexCoord.y), 0);
    //FragColor = vec4(0.0f, 1.0f, 1.0f, 0.0f);
}