#version 330 core

layout(location = 0) in vec2 inPosition;

uniform float offset_x;
uniform float offset_y;

void main()
{
    gl_Position = vec4(inPosition.y + offset_x, -inPosition.x + offset_y, 0.0, 1.0);

}
