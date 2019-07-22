#version 100
precision mediump float;

varying vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D texOverlay;
uniform float overlayOpacity;
uniform vec2 overlayFac;
uniform int texOverlayEnabled;

void main(void)
{
    vec2 overlayBias = vec2((1.0 - overlayFac.x) / 2.0, (1.0 - overlayFac.y) / 2.0),
            texOverlayBiased = vec2(texCoord.x, 1.0 - texCoord.y) * overlayFac + overlayBias;
    vec4 color = texture2D(tex, texCoord),
         colorOverlay = texture2D(texOverlay, texOverlayBiased);
    float opacity = colorOverlay.a * overlayOpacity;
    if(texOverlayEnabled == 0 || opacity < 0.01){
        gl_FragColor = color;
    } else {
        gl_FragColor = colorOverlay * opacity + color * (1.0 - opacity);
    }
}
