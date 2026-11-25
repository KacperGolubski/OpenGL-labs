// Fragment shader
#version 330 core

in vec2 fragUV;

uniform sampler2D tex;
out vec4 outColor;

void main()
{
	vec4 color = texture(tex, fragUV);
	if (color.r > color.g + 0.1 && color.r > color.b + 0.1)
	{
		discard;
	}

	outColor = color;
}
