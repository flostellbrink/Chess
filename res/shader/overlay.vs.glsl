#version 400
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;

out vec2 texCoord;

void main(void)
{
    gl_Position = vec4(pos, 1);
    texCoord = TexCoord;
}
