#version 330 core

// zmienne wejsciowe
in vec4 inoutPos;
in vec3 inoutColor;
in vec2 inoutUV;

// ---------------------------------
// NOWE: uchwyt tekstury jako zmienna
// jednorodna typu sampler2D
// ---------------------------------
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

// zmienna wyjsciowa
out vec4 outColor;


void main()
{
	// ---------------------------------
	// NOWE: pobranie wartosci tekstury 
	// dla zadanej wspolrzednej UV
	// ---------------------------------
	vec4 tex0 = texture(tex0, inoutUV);
	vec4 tex1 = texture(tex1, inoutUV);
	vec4 tex2 = texture(tex2, inoutUV);

	vec3 texColor = mix(tex0.rgb, tex1.rgb, tex1.a);
	texColor = mix(texColor, tex2.rgb, tex2.a);
	
	outColor = vec4(texColor, 1.0);
}
