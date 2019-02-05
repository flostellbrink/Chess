#version 400

uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

void main(void)
{
    vec4 model_Position = model_matrix * vec4(Pos,1);
    gl_Position = view_projection_matrix * model_Position;
}
