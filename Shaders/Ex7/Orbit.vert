/// @file Orbit.vert
/// @brief Planet Orbit vertex shader

#version 330 core

layout( location = 0 ) in vec3 Position;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main ()
{
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4( Position, 1.0 );
}
