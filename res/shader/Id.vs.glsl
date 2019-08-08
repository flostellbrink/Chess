#version 100

uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

attribute vec3 Pos;

void main(void)
{
    vec4 model_Position = model_matrix * vec4(Pos,1);
    gl_Position = view_projection_matrix * model_Position;
}
