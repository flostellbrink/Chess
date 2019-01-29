#version 300 es

uniform mat4 view_projection_matrix;
uniform mat4 view_projection_shadow;
uniform mat4 model_matrix;
uniform mat4 tra_inv_model_matrix;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 La;
uniform vec3 ka;

uniform int textureAnimated;
uniform mat2 texTransform;

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

out vec2 texCoord;
out vec3 normal;
out vec3 vcolor;
out vec3 lightDir;
out vec3 camDir;
out vec4 model_Position;
out vec4 shadow_Position;
out vec2 shadow_Samples[5];

void main(void)
{
    // calculate position in model view projection space
    model_Position = model_matrix * vec4(Pos,1);
    gl_Position = view_projection_matrix * model_Position;
    normal = normalize((tra_inv_model_matrix * vec4(Normal, 1)).xyz);

    shadow_Position = view_projection_shadow * vec4(Pos,1);
    shadow_Position /= vec4(2 ,2 ,2 ,1);
    shadow_Position += vec4(.5,.5,.5,0);
    shadow_Samples[0] = shadow_Position.xy;
    shadow_Samples[1] = shadow_Position.xy + vec2(-.0001,-.0001);
    shadow_Samples[2] = shadow_Position.xy + vec2(-.0001,+.0001);
    shadow_Samples[3] = shadow_Position.xy + vec2(+.0001,-.0001);
    shadow_Samples[4] = shadow_Position.xy + vec2(+.0001,+.0001);
    /*shadow_Samples[5] = shadow_Position.xy + vec2(0,-.0001);
    shadow_Samples[6] = shadow_Position.xy + vec2(0,+.0001);
    shadow_Samples[7] = shadow_Position.xy + vec2(-.0001,0);
    shadow_Samples[8] = shadow_Position.xy + vec2(+.0001,0);*/

    if(textureAnimated == 1)
        // rotate around textures center
        texCoord = texTransform * (TexCoord - vec2(.5,.5)) + vec2(.5,.5);
    else
        texCoord = TexCoord;

    vcolor = ka * La;
    lightDir = lightPos - model_Position.xyz;
    camDir = normalize(camPos - model_Position.xyz);
}
