#version 100

uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

// get position from vertex array object
attribute vec3 Pos;
attribute vec2 TexCoord;

varying vec2 texCoord;

void main(void)
{
    gl_Position = view_projection_matrix * model_matrix * vec4(Pos, 1);

    texCoord = TexCoord;
}
