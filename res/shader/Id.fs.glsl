#version 100
precision mediump float;

uniform int object_id;

void main(void)
{
    gl_FragColor = vec4(float(object_id) / 255.0, 0, 0, 0);
}
