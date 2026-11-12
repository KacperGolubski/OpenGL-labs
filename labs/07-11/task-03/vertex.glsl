#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 vColor;
uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;

void main()
{

	vec4 finalPosition = inPosition;

	vColor = inColor;

	gl_Position = translate * scale * rotate * inPosition;
}
