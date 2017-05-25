#version 450 core
//ins
in vec3 tuv;
in float distanceToLight;

//outs
out vec4 color;

uniform sampler2D tex;
uniform sampler2D norm;
uniform int split;

void main()
{
    vec3 normal = texture(norm,tuv.xy).xyz;
    vec3 lightDir = vec3(1,1,1);
    lightDir = normalize(lightDir);
    normal = normalize(normal);
    float diffuse = max(dot(normal,lightDir),0.0f);

    if(gl_FragCoord.x > split)
    {
        color = vec4(texture(norm,tuv.xy)) * min(max(80.0f/distanceToLight,0.3f),1.3f) * diffuse;
    }
    else
    {
        //color = vec4(texture(tex,tuv.xy)) * min(max(80.0f/distanceToLight,0.3f),1.3f) * diffuse;
        color = vec4(tuv.z,1.0f-tuv.z,0.1f,1.0f) * min(max(80.0f/distanceToLight,0.3f),1.3f) * diffuse;
    }
} 
 
