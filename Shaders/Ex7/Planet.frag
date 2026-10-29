/// @file Planet.frag
/// @brief Planet fragment shader

#version 330 core

#include "Noise.glsl"

in vec3 WorldPosition;
in vec3 WorldNormal;
in vec3 LocalPosition;

out vec4 FragColor;

uniform vec3 CameraPosition;
uniform vec3 LightPosition;
uniform vec3 LightColor;
uniform float LightIntensity;
uniform float LightFalloff;
uniform float AmbientStrength;

uniform vec3 BaseColor;
uniform vec3 AccentColor;
uniform float PatternScale;
uniform float PatternContrast;
uniform float BandStrength;
uniform float BandFrequency;
uniform float SpecularStrength;
uniform float Shininess;
uniform float AtmosphereStrength;

struct FSceneLighting
{
    vec3 Direction;    ///<< Surface -> Light
    vec3 Color;        ///<< LightColor * LightIntensity
    float Attenuation; ///<< 1/(1 + k*d^2)
};

struct FSurfaceShading
{
    vec3 Normal;
    vec3 ViewDirection;
    vec3 Albedo;
    float Pattern;
};

///
/// Lighting
///

FSceneLighting ComputeSceneLighting ( vec3 WorldPos )
{
    FSceneLighting Lighting;
    vec3 ToLight   = LightPosition - WorldPos;
    float Distance = length( ToLight );

    Lighting.Direction   = ToLight / max( Distance, 1e-4 );
    Lighting.Color       = LightColor * LightIntensity;
    Lighting.Attenuation = 1.0 / ( 1.0 + LightFalloff * Distance * Distance );

    return Lighting;
}

vec3 ComputeDiffuseLighting ( const FSurfaceShading Surface, const FSceneLighting Lighting )
{
    float NdotL = max( dot( Surface.Normal, Lighting.Direction ), 0.0 );
    return Surface.Albedo * Lighting.Color * NdotL * Lighting.Attenuation;
}

float ComputeBlinnPhongSpecular ( const FSurfaceShading Surface, const FSceneLighting Lighting, float DiffuseTerm )
{
    vec3 HalfVector = normalize( Lighting.Direction + Surface.ViewDirection );
    float NdotH     = max( dot( Surface.Normal, HalfVector ), 0.0 );
    return pow( NdotH, Shininess ) * smoothstep( 0.0, 0.15, DiffuseTerm );
}

vec3 ComputeSpecularLighting ( const FSurfaceShading Surface, const FSceneLighting Lighting )
{
    float DiffuseTerm = max( dot( Surface.Normal, Lighting.Direction ), 0.0 );
    float Specular    = ComputeBlinnPhongSpecular( Surface, Lighting, DiffuseTerm );
    return Lighting.Color * SpecularStrength * Specular * Lighting.Attenuation;
}

///
/// Surface
///

float EvaluatePlanetPattern ( vec3 LocalPos )
{
    float Blotches = FractalNoise( LocalPos * PatternScale );
    float Bands    = 0.5 + 0.5 * sin( LocalPos.y * BandFrequency + 4.0 * Blotches );
    float Pattern  = mix( Blotches, Bands, BandStrength );
    return clamp( ( Pattern - 0.5 ) * PatternContrast + 0.5, 0.0, 1.0 );
}

FSurfaceShading ComputeSurfaceShading ( vec3 WorldPos, vec3 WorldNrm, vec3 LocalPos )
{
    FSurfaceShading Surface;
    Surface.Normal        = normalize( WorldNrm );
    Surface.ViewDirection = normalize( CameraPosition - WorldPos );
    Surface.Pattern       = EvaluatePlanetPattern( LocalPos );
    Surface.Albedo        = mix( BaseColor, AccentColor, Surface.Pattern );
    return Surface;
}

///
/// Atmosphere rim
//

vec3 ComputeAtmosphericRim ( const FSurfaceShading Surface, const FSceneLighting Lighting )
{
    float Rim       = pow( 1.0 - max( dot( Surface.Normal, Surface.ViewDirection ), 0.0 ), 3.0 );
    float LitFactor = smoothstep( -0.1, 0.5, dot( Surface.Normal, Lighting.Direction ) );
    vec3 RimColor   = mix( BaseColor, LightColor, 0.5 );
    return RimColor * Rim * LitFactor * AtmosphereStrength * Lighting.Attenuation;
}

void main ()
{
    FSceneLighting Lighting = ComputeSceneLighting( WorldPosition );
    FSurfaceShading Surface = ComputeSurfaceShading( WorldPosition, WorldNormal, LocalPosition );

    vec3 Ambient  = Surface.Albedo * AmbientStrength;
    vec3 Diffuse  = ComputeDiffuseLighting( Surface, Lighting );
    vec3 Specular = ComputeSpecularLighting( Surface, Lighting );
    vec3 Rim      = ComputeAtmosphericRim( Surface, Lighting );

    FragColor = vec4( Ambient + Diffuse + Specular + Rim, 1.0 );
}
