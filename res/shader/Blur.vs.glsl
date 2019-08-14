#version 100

attribute vec3 Pos;
attribute vec2 TexCoord;

varying vec2 texCoord;

void main(void)
{
    gl_Position = vec4(Pos, 1);
    texCoord = TexCoord;
}
