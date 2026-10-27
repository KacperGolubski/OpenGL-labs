#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 vColor;

void main()
{

	vec4 finalPosition = inPosition;

	vColor = inColor;

	gl_Position = finalPosition;
}
