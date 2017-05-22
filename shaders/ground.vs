#version 450 core

layout (location = 0) in vec3 position;

out vec2 tuv;

uniform int maxWidth;
uniform int maxHeight;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform float heightScale;
uniform sampler2D tex;

void main()
{
    vec2 v = vec2(position.x,position.y);
    tuv = vec2(position.x/maxWidth,position.y/maxHeight);
    vec4 samplecolor = texture(tex,tuv);
    float gray = (samplecolor.r*0.299 + samplecolor.g*0.587 + samplecolor.b*0.114)*heightScale;
    vec3 final = vec3(v,gray);
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(final,1.0f);
}
