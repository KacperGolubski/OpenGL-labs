#version 330 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 vColor;
uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;
uniform int mode;

mat4 identity = mat4(1.0);

void ModeSelect(int mode){
	switch(mode){
		case 0:
			identity = mat4(
					1.0, 0.0, 0.0, 0.0,  // pierwsza kolumna!!!
					0.0, 1.0, 0.0, 0.0,  // druga kolumna
					0.0, 0.0, 1.0, 0.0,  // trzecia kolumna
					0.0, 0.0, 0.0, 1.0   // czwarta kolumna
			);
		break;
		case 1:
			identity = mat4(
					0.5, 0.0, 0.0, 0.0,  // pierwsza kolumna
					0.0, 0.5, 0.0, 0.0,  // druga kolumna
					0.0, 0.0, 0.5, 0.0,  // trzecia kolumna
					0.0, 0.0, 0.0, 1.0   // czwarta kolumna
			);
		break;
		case 2:
			identity = mat4(
					1.0, 0.0, 0.0, 0.0,  // pierwsza kolumna
					0.0, 1.0, 0.0, 0.0,  // druga kolumna
					0.0, 0.0, 1.0, 0.0,  // trzecia kolumna
					0.5, 0.5, 0.0, 1.0   // czwarta kolumna
			);
		break;
		case 3:
			identity = mat4(
					cos(3.14/4), sin(3.14/4), 0.0, 0.0,  // pierwsza kolumna
					-sin(3.14/4), cos(3.14/4), 0.0, 0.0,  // druga kolumna
					0.0, 0.0, 1.0, 0.0,  // trzecia kolumna
					0.0, 0.0, 0.0, 1.0   // czwarta kolumna
			);
		break;
	}
}

void main()
{
	ModeSelect(mode);

	vec4 finalPosition = inPosition;

	vColor = inColor;

	gl_Position = identity * inPosition;
}
