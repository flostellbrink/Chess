#version 300 es
precision mediump float;

in vec2 texCoord;

// send color to screen
layout(location = 0) out vec4 fcolor;

uniform sampler2D tex;
uniform sampler2D texOverlay;
uniform float overlayOpacity;
uniform vec2 overlayFac;
uniform int texOverlayEnabled;

void main(void)
{
    vec2 overlayBias = vec2((1.0 - overlayFac.x) / 2.0, (1.0 - overlayFac.y) / 2.0),
            texOverlayBiased = vec2(texCoord.x, 1.0 - texCoord.y) * overlayFac + overlayBias;
    vec4 color = texture(tex, texCoord),
         colorOverlay = texture(texOverlay, texOverlayBiased);
    float opacity = colorOverlay.a * overlayOpacity;
    if(texOverlayEnabled == 0 || opacity < 0.01){
        fcolor = color;
    } else {
        fcolor = colorOverlay * opacity + color * (1.0 - opacity);
    }
}
