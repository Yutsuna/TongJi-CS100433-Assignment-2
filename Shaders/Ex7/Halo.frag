/// @file Halo.frag
/// @brief Halo fragment shader

#version 330 core

in vec2 Corner;

out vec4 FragColor;

uniform vec3 HaloColor;
uniform float HaloIntensity;
uniform float RayCount;
uniform float RayStrength;
uniform float RayRotationSpeed;
uniform float Time;

void main ()
{
    float Radius = length( Corner );
    float Angle  = atan( Corner.y, Corner.x );

    float Core   = exp( -Radius * Radius * 9.0 );
    float Corona = exp( -Radius * 3.2 );
    float Rays   = pow( abs( sin( Angle * RayCount * 0.5 + Time * RayRotationSpeed ) ), 6.0 );
    float Glow   = Core + 0.5 * Corona * mix( 1.0, Rays * 2.0, RayStrength );

    float EdgeFade = 1.0 - smoothstep( 0.75, 1.0, Radius );

    FragColor = vec4( HaloColor * HaloIntensity * Glow * EdgeFade, 1.0 );
}
