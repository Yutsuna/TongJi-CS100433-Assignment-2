/// @file Star.vert
/// @brief Star vertex shader

#version 330 core
#include "Noise.glsl"

layout( location = 0 ) in vec3 Position;
layout( location = 1 ) in float Temperature;
layout( location = 2 ) in float Size;
layout( location = 3 ) in float Phase;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform float Time;
uniform float TwinkleIntensity;
uniform float TwinkleSpeed;
uniform float PointSizeScale;
uniform vec3 StarColorCold;
uniform vec3 StarColorWarm;

out vec3 StarColor;

float ComputeProceduralTwinkling ()
{
    float SpeedFactor = 0.6 + 1.4 * Hash11( Phase );
    float Flicker     = 0.5 * sin( Time * TwinkleSpeed * SpeedFactor + Phase * 6.2831853 ) +
                    0.5 * sin( Time * TwinkleSpeed * SpeedFactor * 2.3 + Phase * 17.0 ); ///<< [-1, 1]

    return 1.0 - TwinkleIntensity * ( 0.5 - 0.5 * Flicker ); ///<< [1 - Intensity, 1]
}

void main ()
{
    float Brightness = ComputeProceduralTwinkling();

    StarColor    = mix( StarColorCold, StarColorWarm, Temperature ) * Brightness;
    gl_PointSize = Size * PointSizeScale * mix( 0.6, 1.0, Brightness );

    mat4 RotationOnlyView = mat4( mat3( ViewMatrix ) );
    gl_Position           = ProjectionMatrix * RotationOnlyView * vec4( Position, 1.0 );
}
