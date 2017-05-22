#version 450 core
layout( triangles ) in;
layout( triangle_strip, max_vertices=256 ) out;

in vec2 mapuv[3];

uniform int uLevel;
uniform float heightScale;
uniform sampler2D tex;
uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;

out vec4 gColor;

vec2 V0, V1, V2;
vec2 U0, U1, U2;
void ProduceVertex( float s, float t )
{
    vec2 v = V0 + s*V1 + t*V2;
    vec2 u = U0 + s*U1 + t*U2;
    vec4 samplecolor = texture(tex,u);
    float gray = (samplecolor.r*0.299 + samplecolor.g*0.587 + samplecolor.b*0.114)*heightScale;
    vec3 final = vec3(v,gray);
    gColor = vec4(0.1f,0.1f,0.4f,1.0f) * heightScale / gray;
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(final,1.0f);
    EmitVertex();
}

void main( )
{
    V0 = gl_in[0].gl_Position.xy;
    V1 = (gl_in[1].gl_Position - gl_in[0].gl_Position).xy;
    V2 = (gl_in[2].gl_Position - gl_in[0].gl_Position).xy;
    
    U0 = mapuv[0].xy;
    U1 = (mapuv[1] - mapuv[0]).xy;
    U2 = (mapuv[2] - mapuv[0]).xy;

    int numLayers = 1 << uLevel;
    float dt = 1.0f / float( numLayers );
    float t_top = 1.0f;
    for( int it = 0; it < numLayers; it++ )
    {
        float t_bot = t_top - dt;
        float smax_top = 1. - t_top;
        float smax_bot = 1. - t_bot;
        int nums = it + 1;
        float ds_top = smax_top / float( nums - 1 );
        float ds_bot = smax_bot / float( nums );
        float s_top = 0.;
        float s_bot = 0.;
        for( int is = 0; is < nums; is++ )
        {
            ProduceVertex( s_bot, t_bot );
            ProduceVertex( s_top, t_top );
            s_top += ds_top;
            s_bot += ds_bot;
        }
        ProduceVertex( s_bot, t_bot );
        EndPrimitive();
        t_top = t_bot;
        t_bot -= dt;
    }
}
