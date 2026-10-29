/// @file Camera.cpp
/// @brief 3D Camera rotating around the origin implementation

#pragma once

#include <Ex7/Camera.hpp>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

void CS::FCamera::Rotate ( const Float32 DeltaYaw, const Float32 DeltaPitch ) noexcept
{
    Yaw += DeltaYaw;
    Pitch = std::clamp( Pitch + DeltaPitch, -MaxPitch, MaxPitch );
}

void CS::FCamera::Zoom ( const Float32 ScrollAmount ) noexcept
{
    Distance = std::clamp( Distance * std::exp( -ScrollAmount * ZoomSensitivity ), MinDistance, MaxDistance );
}

void CS::FCamera::AutoRotate ( const Float32 DeltaTime ) noexcept
{
    Yaw += AutoRotationSpeed * DeltaTime;
}

glm::fvec3 CS::FCamera::GetPosition () const noexcept
{
    return { Distance * std::cos( Pitch ) * std::sin( Yaw ), Distance * std::sin( Pitch ),
             Distance * std::cos( Pitch ) * std::cos( Yaw ) };
}

glm::fmat4 CS::FCamera::GetViewMatrix () const noexcept
{
    return glm::lookAt( GetPosition(), glm::fvec3{}, glm::fvec3{ 0.F, 1.F, 0.F } );
}

glm::fmat4 CS::FCamera::GetProjectionMatrix ( const Float32 AspectRatio ) const noexcept
{
    return glm::perspective( FieldOfViewY, AspectRatio, NearPlane, FarPlane );
}
