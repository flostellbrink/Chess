#version 100
precision mediump float;

// get color from vertex shader
varying vec2 texCoord;

uniform sampler2D tex;

void main(void)
{
    // set fragment color
    gl_FragColor = texture2D(tex, texCoord);
}
