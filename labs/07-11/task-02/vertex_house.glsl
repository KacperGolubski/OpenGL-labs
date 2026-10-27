#version 330 core

layout(location = 0) in vec2 inPosition;

uniform mat4 Matrix_Rotation;


void main()
{
    gl_Position = Matrix_Rotation * vec4(inPosition, 0.0, 1.0);

}
