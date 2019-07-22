#version 100
precision mediump float;

varying vec3 vcolor;
varying vec2 texCoord;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 camDir;
varying vec4 model_Position;
varying vec4 shadow_Position;
varying vec2 shadow_Samples[5];

uniform sampler2D tex;
uniform sampler2D texOverlay;
uniform sampler2D texReflection;
uniform sampler2D texShadow;

uniform vec2 texSize;

uniform vec3 Ld;
uniform vec3 kd;
uniform float shininess;
uniform float reflectivity;

uniform float overlayOpacity;

void main(void)
{
    // reflectivity of the mirror
    float mirrority = .2;
    vec3 Id = max(dot(normalize(lightDir), normal), 0.0) * kd * Ld;

    // Specular light
    vec3 halfDir    = normalize(lightDir + camDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    float Is        = pow(specAngle, shininess);

    // Shadow
    float visibility = 1.0;
    for (int i=0;i<5;i++){
        if(shadow_Position.z > texture2D(texShadow, shadow_Samples[i]).z + .01){
            visibility -= 0.2;
        }
    }

    // Texture colors and overlayOpacity
    vec4 color = texture2D(tex, (model_Position.xz + vec2(8.5, 8.5)) / 17.0),
         colorOverlay = texture2D(texOverlay, texCoord);
    float opacity = overlayOpacity * colorOverlay.a;

    // Combine overlay and texture color
    if(opacity < 0.01){
        color = color;
    } else {
        color = (1.0 - opacity) * color
              + opacity       * colorOverlay;
    }

    // Apply light
    color *= vec4(visibility * vcolor + Id, 1);
    color += vec4(vec3(visibility * reflectivity * Is), 0);

    // Combine color and reflection
    gl_FragColor = (1.0 - mirrority) * color
           + mirrority * texture2D(texReflection, gl_FragCoord.xy / texSize);
}
