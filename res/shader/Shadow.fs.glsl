#version 100
precision mediump float;

void main(){
    //fragmentdepth = gl_FragCoord.z;
    // Used for debugging
    gl_FragColor = vec4(gl_FragCoord.z);
    //gl_FragColor = gl_FragCoord;
}
