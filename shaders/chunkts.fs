#version 450 core
//ins
in vec3 tuv;
in float distanceToLight;

//outs
out vec4 color;


uniform sampler2D tex;

void main()
{
    color = vec4(texture(tex,tuv.xy)) * min((30.0f/distanceToLight),1.0f);
    //color = vec4(tuv.z,1.0f-tuv.z,0.1f,1.0f) * min((30.0f/distanceToLight),2.0f);
} 
 
