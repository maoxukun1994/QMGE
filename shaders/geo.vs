#version 450 core
layout (location = 0) in vec3 position;
layout (location = 3) in vec2 tuv;
out vec2 mapuv;
void main()
{
    mapuv = tuv;
    gl_Position = vec4(position,1.0f);
}
