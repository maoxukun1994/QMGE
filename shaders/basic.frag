#version 450 core
//ins
in vec2 tuv;
//outs
out vec4 color;
//uniforms
uniform sampler2D tex;

void main()
{
    color = texture(tex,tuv);
    //color = vec4(1.0f,0.0f,0.0f,1.0f);
} 
 
