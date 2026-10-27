#version 330 core

out vec4 outColor;

void main()
{
    float gradient_x = gl_FragCoord.x / 600;
    float gradient_y = gl_FragCoord.y / 600;

    if (gl_PrimitiveID == 0) {
        outColor = vec4(1.0, gradient_y, gradient_x, 1.0);
    }
    else if (gl_PrimitiveID == 1){
        outColor = vec4(gradient_x, gradient_y, 0.5, 1.0);
    }
    else if (gl_PrimitiveID == 2) {
        outColor = vec4(gradient_y, 0.0, gradient_x, 1.0);
    }
    else  if (gl_PrimitiveID == 3){
        outColor = vec4(gradient_x, 0.9, gradient_y, 1.0);
    }
    else if (gl_PrimitiveID == 4) {
        outColor = vec4(0.5, gradient_y, gradient_x, 1.0);
    }
    else if (gl_PrimitiveID == 5) {
        outColor = vec4(gradient_x, 0.7, gradient_x, 1.0);
    }
    else if (gl_PrimitiveID == 6) {
        outColor = vec4(gradient_y, 0.0, gradient_x, 1.0);
    }
    else if (gl_PrimitiveID == 7){
        outColor = vec4(gradient_y, gradient_y, 0.5, 1.0);
    }
    else if (gl_PrimitiveID == 8){
        outColor = vec4(0.9, gradient_x, gradient_x, 1.0);
    }
}
