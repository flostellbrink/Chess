#version 400

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 TexCoord;

out vec2 texCoord;
out vec2 blurCoords[14];

void main(void)
{
    gl_Position = vec4(pos, 1);
    texCoord = TexCoord;
    blurCoords[0] = TexCoord + vec2(0.0, -0.028);
    blurCoords[1] = TexCoord + vec2(0.0, -0.024);
    blurCoords[2] = TexCoord + vec2(0.0, -0.020);
    blurCoords[3] = TexCoord + vec2(0.0, -0.016);
    blurCoords[4] = TexCoord + vec2(0.0, -0.012);
    blurCoords[5] = TexCoord + vec2(0.0, -0.008);
    blurCoords[6] = TexCoord + vec2(0.0, -0.004);
    blurCoords[7] = TexCoord + vec2(0.0,  0.004);
    blurCoords[8] = TexCoord + vec2(0.0,  0.008);
    blurCoords[9] = TexCoord + vec2(0.0,  0.012);
    blurCoords[10] = TexCoord + vec2(0.0, 0.016);
    blurCoords[11] = TexCoord + vec2(0.0, 0.020);
    blurCoords[12] = TexCoord + vec2(0.0, 0.024);
    blurCoords[13] = TexCoord + vec2(0.0, 0.028);
}
