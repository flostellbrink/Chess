#version 300 es

uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

// get position from vertex array object
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;

out vec2 texCoord;

void main(void)
{
    gl_Position = view_projection_matrix * model_matrix * vec4(pos, 1);

    texCoord = TexCoord;
}
