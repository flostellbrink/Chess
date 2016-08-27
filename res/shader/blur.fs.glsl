#version 400

in vec2 texCoord;
in vec2 blurCoords[14];

// send color to screen
layout(location = 0) out vec4 fcolor;

uniform sampler2D tex;
uniform float overlayOpacity;

void main(void)
{
    //fcolor = texture(tex, texCoord);
    if(overlayOpacity < 0.01){
        fcolor = texture(tex, texCoord);
    } else {
        vec4 blurColor = vec4(0.0);
        blurColor += texture(tex, blurCoords[0])   * 0.0044299121055113265;
        blurColor += texture(tex, blurCoords[1])   * 0.00895781211794;
        blurColor += texture(tex, blurCoords[2])   * 0.0215963866053;
        blurColor += texture(tex, blurCoords[3])   * 0.0443683338718;
        blurColor += texture(tex, blurCoords[4])   * 0.0776744219933;
        blurColor += texture(tex, blurCoords[5])   * 0.115876621105;
        blurColor += texture(tex, blurCoords[6])   * 0.147308056121;
        blurColor += texture(tex, texCoord)        * 0.159576912161;
        blurColor += texture(tex, blurCoords[7])   * 0.147308056121;
        blurColor += texture(tex, blurCoords[8])   * 0.115876621105;
        blurColor += texture(tex, blurCoords[9])   * 0.0776744219933;
        blurColor += texture(tex, blurCoords[10])  * 0.0443683338718;
        blurColor += texture(tex, blurCoords[11])  * 0.0215963866053;
        blurColor += texture(tex, blurCoords[12])  * 0.00895781211794;
        blurColor += texture(tex, blurCoords[13])  * 0.0044299121055113265;
        fcolor = blurColor * overlayOpacity + texture(tex, texCoord) * (1 - overlayOpacity);
    }

}
