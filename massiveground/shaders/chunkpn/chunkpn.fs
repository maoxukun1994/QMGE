#version 440 core

in float height;
in float distanceToLight;
out vec4 color;

void main()
{
    //vec3 lightDir = normalize(vec3(1.0f,1.0f,1.0f));

    //float diffuse = max(dot(normal,lightDir),0.0f);

    //color = vec4(1.0f,0.0f,0.0f,1.0f);
    color = vec4(height,1.0f-height,0.2f,1.0f) * min(max(80.0f/distanceToLight,0.2f),1.3f);
} 
 
