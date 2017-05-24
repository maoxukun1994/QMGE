/*
#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;


in vec3 tuv[];

out vec3 tuvc;

uniform float time;

/*
vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 0.5f;
    vec3 direction = normal * (sin(0)) * magnitude;
    return position + vec4(direction, 0.0f);
}
vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}
*/

/*
void main() 
{
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    tuvc = tuv[0];
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    tuvc = tuv[1];
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    tuvc = tuv[2];
    EmitVertex();
    EndPrimitive();
} 
*/
