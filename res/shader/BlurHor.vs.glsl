#version 100

attribute vec3 pos;
attribute vec2 TexCoord;

varying vec2 texCoord;
varying vec2 blurCoords[14];

void main(void)
{
    gl_Position = vec4(pos, 1);
    texCoord = TexCoord;
    blurCoords[0] = TexCoord + vec2(-0.028, 0.0);
    blurCoords[1] = TexCoord + vec2(-0.024, 0.0);
    blurCoords[2] = TexCoord + vec2(-0.020, 0.0);
    blurCoords[3] = TexCoord + vec2(-0.016, 0.0);
    blurCoords[4] = TexCoord + vec2(-0.012, 0.0);
    blurCoords[5] = TexCoord + vec2(-0.008, 0.0);
    blurCoords[6] = TexCoord + vec2(-0.004, 0.0);
    blurCoords[7] = TexCoord + vec2( 0.004, 0.0);
    blurCoords[8] = TexCoord + vec2( 0.008, 0.0);
    blurCoords[9] = TexCoord + vec2( 0.012, 0.0);
    blurCoords[10] = TexCoord + vec2( 0.016, 0.0);
    blurCoords[11] = TexCoord + vec2( 0.020, 0.0);
    blurCoords[12] = TexCoord + vec2( 0.024, 0.0);
    blurCoords[13] = TexCoord + vec2( 0.028, 0.0);
}
