#version 440 core
//ins
in vec3 tuvc;
//outs
out vec4 color;

uniform sampler2D ctex;

void main()
{
    //color = vec4(texture(ctex,tuv.xy));
    color = vec4(tuvc.z,1.0f-tuvc.z,0.1f,1.0f);
} 
 
