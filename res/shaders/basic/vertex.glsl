#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoords;

uniform mat4 Transform;

out vec3 Color;
out vec2 TextureCoords;

void main()
{
    gl_Position = Transform * vec4(aPosition, 1.0);
    Color = aColor;
    TextureCoords = aTextureCoords;
}