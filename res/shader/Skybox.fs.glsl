#version 100
precision mediump float;

// get color from vertex shader
varying vec3 texCoord;

uniform samplerCube tex;

void main(void)
{
    // set fragment color
    gl_FragColor = textureCube(tex, texCoord);
}
