#version 400

// get color from vertex shader
in vec3 texCoord;

// send color to screen
layout(location = 0) out vec4 fcolor;

uniform samplerCube tex;

void main(void)
{
    // set fragment color
    fcolor = texture(tex, texCoord);
}
