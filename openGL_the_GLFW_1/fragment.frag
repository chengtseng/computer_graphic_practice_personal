#version 330 core

out vec4 color;

uniform float R;
uniform float G;
uniform float B;

void main()
{
    color = vec4(R, G, B, 1.0f);
} 