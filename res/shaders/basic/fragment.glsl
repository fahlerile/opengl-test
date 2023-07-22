#version 330 core

in vec2 TextureCoords;

out vec4 FragColor;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main()
{
    FragColor = mix(texture(Texture1, TextureCoords), texture(Texture2, TextureCoords), 0.8);
}