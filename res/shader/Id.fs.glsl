#version 400

uniform int object_id;

layout(location = 0) out vec4 fcolor;

void main(void)
{
    fcolor = vec4(float(object_id) / 255.0, 0, 0, 0);
}
