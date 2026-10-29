/// @file Planet.vert
/// @brief Planet vertex shader

#version 330 core

layout( location = 0 ) in vec3 Position;
layout( location = 1 ) in vec3 Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 WorldPosition;
out vec3 WorldNormal;
out vec3 LocalPosition;

void main ()
{
    vec4 World = ModelMatrix * vec4( Position, 1.0 );

    WorldPosition = World.xyz;
    WorldNormal   = mat3( ModelMatrix ) * Normal;
    LocalPosition = Position;

    gl_Position = ProjectionMatrix * ViewMatrix * World;
}
