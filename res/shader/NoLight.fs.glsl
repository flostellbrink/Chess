#version 400

// get color from vertex shader
in vec2 texCoord;

// send color to screen
layout(location = 0) out vec4 fcolor;

uniform sampler2D tex;

void main(void)
{
    // set fragment color
    fcolor = texture(tex, texCoord);
}
