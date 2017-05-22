#version 450 core

layout (location = 3) in vec2 position;

out vec2 tuv;

uniform float maxWidth;
uniform float maxHeight;
uniform float heightScale;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform sampler2D tex;

void main()
{
    vec2 v = position;
    tuv = vec2(position.x/maxWidth,position.y/maxHeight);
    vec4 samplecolor = texture(tex,tuv);
    float gray = (samplecolor.r*0.299 + samplecolor.g*0.587 + samplecolor.b*0.114)*heightScale;
    vec3 final = vec3(v,gray);
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(final,1.0f);
}
