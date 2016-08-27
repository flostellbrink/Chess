#version 400

in vec3 vcolor;
in vec2 texCoord;
in vec3 normal;
in vec3 lightDir;
in vec3 camDir;
in vec4 shadow_Position;
in vec2 shadow_Samples[5];

// send color to screen
layout(location = 0) out vec4 fcolor;

uniform sampler2D tex;
uniform sampler2D texShadow;

uniform vec3 lightPos;
uniform vec3 Ld;
uniform vec3 kd;
uniform float shininess;
uniform float reflectivity;

void main(void)
{
    vec3 Id = max(dot(lightDir, normal),0) * kd * Ld;

    vec3 halfDir = normalize(lightDir + camDir);
    float specAngle = max(dot(halfDir, normal), 0.0);
    float Is = pow(specAngle, shininess);

    // Shadow
    float visibility = 1.0;
    for (int i=0;i<5;i++){
        if(shadow_Position.z > texture(texShadow, shadow_Samples[i]).z + .01){
            visibility -= 0.2;
        }
    }

    fcolor = vec4(vcolor + visibility * Id, 1) * texture(tex, texCoord)
           + vec4(vec3(visibility * reflectivity * Is),0);
}
