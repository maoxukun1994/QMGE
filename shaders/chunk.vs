#version 450 core

layout (location = 3) in vec2 position;

out vec3 tuv;

uniform float maxWidth;
uniform float maxHeight;
uniform float heightScale;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform sampler2D tex;

void main()
{
    vec2 v = vec2(position.x/maxWidth,position.y/maxHeight);
    vec4 samplecolor = texture(tex,v);
    float gray = (samplecolor.r*0.299 + samplecolor.g*0.587 + samplecolor.b*0.114);
    tuv = vec3(v,gray);
    vec3 final = vec3(position,gray*heightScale);
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(final,1.0f);
}
