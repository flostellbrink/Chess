#version 100
precision mediump float;

varying vec3 vcolor;
varying vec2 texCoord;
varying vec3 normal;
varying vec3 lightDir;
varying vec4 model_Position;
varying vec4 shadow_Position;
varying vec3 camDir;
varying vec2 shadow_Samples[5];

uniform sampler2D tex;
uniform sampler2D texShadow;

uniform vec3 Ld;
uniform vec3 kd;
uniform float shininess;
uniform float reflectivity;

void main(void)
{
    vec3 Id = max(dot(normalize(lightDir), normal), 0.0) * kd * Ld;

    vec3 halfDir = normalize(lightDir + camDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    float Is = pow(specAngle, shininess);

    // Shadow
    float visibility = 1.0;
    for (int i=0;i<5;i++){
        if(shadow_Position.z > texture2D(texShadow, shadow_Samples[i]).z + .01){
            visibility -= 0.2;
        }
    }

    gl_FragColor = vec4(vcolor + visibility * Id, 1)
            //Map area from -8.5 to 8.5 on x and z axis to 0 to 1 for texture
            * texture2D(tex, (model_Position.xz + vec2(8.5, 8.5)) / 17.0)
            + vec4(vec3(visibility * reflectivity * Is), 0);
}
