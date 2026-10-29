/// @file Orbit.frag
/// @brief Planets Orbit fragment shader

#version 330 core

out vec4 FragColor;

uniform vec3 OrbitColor;
uniform float OrbitOpacity;

void main ()
{
    FragColor = vec4( OrbitColor, OrbitOpacity );
}
