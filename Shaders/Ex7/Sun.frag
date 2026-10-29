/// @file Sun.frag
/// @brief Sun fragment shader

#version 330 core
#include "Noise.glsl"

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec3 LocalPosition;

out vec4 FragColor;

uniform vec3 CameraPosition;
uniform vec3 EmissionColor;
uniform vec3 HotColor;
uniform float EmissionIntensity;
uniform float GranulationScale;
uniform float GranulationSpeed;
uniform float Time;

///
/// View
///

float ComputeSunFacing ()
{
    vec3 Normal        = normalize( WorldNormal );
    vec3 ViewDirection = normalize( CameraPosition - WorldPosition );
    return max( dot( Normal, ViewDirection ), 0.0 );
}

///
/// Granulation
///

vec3 ComputeGranulationFlow ()
{
    vec3 Advection = vec3( 0.0, Time, Time * 0.6 ) * GranulationSpeed;

    return LocalPosition * GranulationScale + Advection;
}

float ComputeGranulationDetail ( vec3 BaseFlow )
{
    return FractalNoise( BaseFlow * 2.7 - vec3( Time * GranulationSpeed ) );
}

///
/// Surface shading
///

vec3 ComputeEmissionColor ( float Granulation )
{
    /// Hot granulation against a cooler background.
    return mix( EmissionColor, HotColor, smoothstep( 0.30, 0.75, Granulation ) );
}

float ComputeDarkSpots ( float Detail )
{
    /// Dark spots, the peaks of the high-frequency octave are darkened.
    return 1.0 - 0.35 * smoothstep( 0.62, 0.80, Detail );
}

float ComputeLimbDarkening ( float Facing )
{
    /// Center-edge darkening
    return mix( 0.55, 1.0, pow( Facing, 0.6 ) );
}

void main ()
{
    float Facing = ComputeSunFacing();

    vec3 Flow    = ComputeGranulationFlow();
    float Detail = ComputeGranulationDetail( Flow );

    vec3 Color  = ComputeEmissionColor( Detail );
    float Spots = ComputeDarkSpots( Detail );
    float Limb  = ComputeLimbDarkening( Facing );

    FragColor = vec4( Color * EmissionIntensity * Spots * Limb, 1.0 );
}
