/// @file Hallo.vert
/// @brief Hallo vertex shader

#version 330 core

layout( location = 0 ) in vec2 CornerPosition;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 HaloCenter;
uniform float HaloSize;

out vec2 Corner;

void main ()
{
    vec3 CameraRight = vec3( ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0] );
    vec3 CameraUp    = vec3( ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1] );
    vec3 World       = HaloCenter + ( CameraRight * CornerPosition.x + CameraUp * CornerPosition.y ) * HaloSize;

    Corner      = CornerPosition;
    gl_Position = ProjectionMatrix * ViewMatrix * vec4( World, 1.0 );
}
