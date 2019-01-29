#version 300 es

uniform mat4 projection_matrix;
uniform mat4 modelview_matrix;

// get position from vertex array object
layout(location = 0) in vec3 pos;

out vec3 texCoord;

void main(void)
{
    gl_Position = projection_matrix * modelview_matrix * vec4(pos, 1);

    texCoord = pos;
}
