/// @file Star.frag
/// @brief Star fragment shader

#version 330 core

in vec3 StarColor;
out vec4 FragColor;

void main ()
{
    vec2 Offset           = gl_PointCoord * 2.0 - 1.0;
    float DistanceSquared = dot( Offset, Offset );

    if ( DistanceSquared > 1.0 )
    {
        discard;
    }
    FragColor = vec4( StarColor * exp( -DistanceSquared * 4.0 ), 1.0 );
}
