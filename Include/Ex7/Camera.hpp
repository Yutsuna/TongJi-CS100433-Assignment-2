/// @file Camera.hpp
/// @brief 3D Camera rotating around the origin

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>

namespace CS
{

class FCamera
{
public:

    void Rotate ( const Float32 DeltaYaw, const Float32 DeltaPitch ) noexcept;
    void Zoom ( const Float32 ScrollAmount ) noexcept;
    void AutoRotate ( const Float32 DeltaTime ) noexcept;

public:

    [[nodiscard]] glm::fvec3 GetPosition () const noexcept;
    [[nodiscard]] glm::fmat4 GetViewMatrix () const noexcept;
    [[nodiscard]] glm::fmat4 GetProjectionMatrix ( const Float32 AspectRatio ) const noexcept;

private:

    static constexpr Float32 MaxPitch          = 1.5F;
    static constexpr Float32 MinDistance       = 12.F;
    static constexpr Float32 MaxDistance       = 150.F;
    static constexpr Float32 ZoomSensitivity   = 0.1F;
    static constexpr Float32 AutoRotationSpeed = 0.02F;
    static constexpr Float32 FieldOfViewY      = glm::radians( 45.F );
    static constexpr Float32 NearPlane         = 0.1F;
    static constexpr Float32 FarPlane          = 1000.F;

    Float32 Yaw      = 0.6F;
    Float32 Pitch    = 0.5F;
    Float32 Distance = 62.F;
};

} // namespace CS
