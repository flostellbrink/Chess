#version 100

attribute vec3 pos;
attribute vec2 TexCoord;

varying vec2 texCoord;

void main(void)
{
    gl_Position = vec4(pos, 1);
    texCoord = TexCoord;
}
