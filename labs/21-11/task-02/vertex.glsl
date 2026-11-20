// Vertex shader
#version 330 core

// Dane pobrane z VAO
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 fragColor;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform vec3 uniColor;

float brightness = 0.5 + 0.5 * ((inColor.y + 1.0) / 2.0);


void main()
{
	fragColor = inColor;
	vec4 newPosition = matProj * matView * matModel * inPosition;
	gl_Position = newPosition;
}
