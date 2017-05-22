#version 450 core
//ins
in vec2 tuv;
//outs
out vec4 color;
uniform sampler2D ctex;

void main()
{
    color = vec4(texture(ctex,tuv));
} 
 
