#version 100

uniform mat4 view_projection_matrix;
uniform mat4 view_projection_shadow;
uniform mat4 model_matrix;
uniform mat4 tra_inv_model_matrix;

uniform vec3 lightPos;
uniform vec3 camPos;
uniform vec3 La;
uniform vec3 ka;

attribute vec3 Pos;
attribute vec3 Normal;

varying vec3 normal;
varying vec3 vcolor;
varying vec3 lightDir;
varying vec3 camDir;
varying vec4 model_Position;
varying vec4 shadow_Position;
varying vec2 shadow_Samples[5];

void main(void)
{
    // calculate position in model view projection space
    model_Position = model_matrix * vec4(Pos, 1);
    gl_Position = view_projection_matrix * model_Position;

    shadow_Position = view_projection_shadow * vec4(Pos, 1);
    shadow_Position /= 2.0;
    shadow_Position += vec4(.5);
    shadow_Samples[0] = shadow_Position.xy;
    shadow_Samples[1] = shadow_Position.xy + vec2(-.0001, -.0001);
    shadow_Samples[2] = shadow_Position.xy + vec2(-.0001, +.0001);
    shadow_Samples[3] = shadow_Position.xy + vec2(+.0001, -.0001);
    shadow_Samples[4] = shadow_Position.xy + vec2(+.0001, +.0001);
    /*
    shadow_Samples[5] = shadow_Position.xy + vec2(0, -.0001);
    shadow_Samples[6] = shadow_Position.xy + vec2(0, +.0001);
    shadow_Samples[7] = shadow_Position.xy + vec2(-.0001, 0);
    shadow_Samples[8] = shadow_Position.xy + vec2(+.0001, 0);
    */

    // needs to be normalized because scale model matrix scews the length
    normal = normalize((tra_inv_model_matrix * vec4(Normal, 1)).xyz);

    vcolor = ka * La;
    lightDir = lightPos - model_Position.xyz;
}
