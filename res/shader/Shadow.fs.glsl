#version 400

//layout(location = 0) out float fragmentdepth;
// Used for debugging
layout(location = 0) out vec4 fcolor;

void main(){
    //fragmentdepth = gl_FragCoord.z;
    // Used for debugging
    fcolor = vec4(gl_FragCoord.z);
    //fcolor = gl_FragCoord;
}
