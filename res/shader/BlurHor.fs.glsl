#version 100
precision mediump float;

varying vec2 texCoord;

uniform sampler2D tex;
uniform float overlayOpacity;

void main(void)
{
    if(overlayOpacity < 0.01){
        gl_FragColor = texture2D(tex, texCoord);
    } else {
        vec4 blurColor = vec4(0.0);
        blurColor += texture2D(tex, texCoord + vec2(-0.028, 0.0)) * 0.0044299121055113265;
        blurColor += texture2D(tex, texCoord + vec2(-0.024, 0.0)) * 0.00895781211794;
        blurColor += texture2D(tex, texCoord + vec2(-0.020, 0.0)) * 0.0215963866053;
        blurColor += texture2D(tex, texCoord + vec2(-0.016, 0.0)) * 0.0443683338718;
        blurColor += texture2D(tex, texCoord + vec2(-0.012, 0.0)) * 0.0776744219933;
        blurColor += texture2D(tex, texCoord + vec2(-0.008, 0.0)) * 0.115876621105;
        blurColor += texture2D(tex, texCoord + vec2(-0.004, 0.0)) * 0.147308056121;
        blurColor += texture2D(tex, texCoord + vec2(0.0, 0.0)) * 0.159576912161;
        blurColor += texture2D(tex, texCoord + vec2( 0.004, 0.0)) * 0.147308056121;
        blurColor += texture2D(tex, texCoord + vec2( 0.008, 0.0)) * 0.115876621105;
        blurColor += texture2D(tex, texCoord + vec2( 0.012, 0.0)) * 0.0776744219933;
        blurColor += texture2D(tex, texCoord + vec2( 0.016, 0.0)) * 0.0443683338718;
        blurColor += texture2D(tex, texCoord + vec2( 0.020, 0.0)) * 0.0215963866053;
        blurColor += texture2D(tex, texCoord + vec2( 0.024, 0.0)) * 0.00895781211794;
        blurColor += texture2D(tex, texCoord + vec2( 0.028, 0.0)) * 0.0044299121055113265;
        gl_FragColor = blurColor * overlayOpacity + texture2D(tex, texCoord) * (1.0 - overlayOpacity);
    }

}
