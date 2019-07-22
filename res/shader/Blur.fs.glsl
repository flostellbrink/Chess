#version 100
precision mediump float;

varying vec2 texCoord;
varying vec2 blurCoords[14];

uniform sampler2D tex;
uniform float overlayOpacity;

void main(void)
{
    if(overlayOpacity < 0.01){
        gl_FragColor = texture2D(tex, texCoord);
    } else {
        vec4 blurColor = vec4(0.0);
        blurColor += texture2D(tex, blurCoords[0])   * 0.0044299121055113265;
        blurColor += texture2D(tex, blurCoords[1])   * 0.00895781211794;
        blurColor += texture2D(tex, blurCoords[2])   * 0.0215963866053;
        blurColor += texture2D(tex, blurCoords[3])   * 0.0443683338718;
        blurColor += texture2D(tex, blurCoords[4])   * 0.0776744219933;
        blurColor += texture2D(tex, blurCoords[5])   * 0.115876621105;
        blurColor += texture2D(tex, blurCoords[6])   * 0.147308056121;
        blurColor += texture2D(tex, texCoord)        * 0.159576912161;
        blurColor += texture2D(tex, blurCoords[7])   * 0.147308056121;
        blurColor += texture2D(tex, blurCoords[8])   * 0.115876621105;
        blurColor += texture2D(tex, blurCoords[9])   * 0.0776744219933;
        blurColor += texture2D(tex, blurCoords[10])  * 0.0443683338718;
        blurColor += texture2D(tex, blurCoords[11])  * 0.0215963866053;
        blurColor += texture2D(tex, blurCoords[12])  * 0.00895781211794;
        blurColor += texture2D(tex, blurCoords[13])  * 0.0044299121055113265;
        gl_FragColor = blurColor * overlayOpacity + texture2D(tex, texCoord) * (1.0 - overlayOpacity);
    }

}
