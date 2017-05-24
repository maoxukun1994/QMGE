#version 450 core
//ins
in vec3 tuv;
in float distanceToLight;

//outs
out vec4 color;


uniform sampler2D tex;

void main()
{
    if(gl_FragCoord.x > 640)
    {
        color = vec4(texture(tex,tuv.xy)) * min((80.0f/distanceToLight),1.0f);
    }
    else
    {
        color = vec4(tuv.z,1.0f-tuv.z,0.1f,1.0f) * min((30.0f/distanceToLight),2.0f);
    }
} 
 
