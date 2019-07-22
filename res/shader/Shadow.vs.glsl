#version 100

uniform mat4 view_projection_shadow;
attribute vec3 Pos;

void main(){
    gl_Position =  view_projection_shadow * vec4(Pos,1);
    //gl_Position /= gl_Position.w;
}
