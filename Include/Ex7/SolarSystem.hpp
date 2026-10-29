/// @file SolarSystem.hpp
/// @brief Solarsystem Container

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace CS
{

struct FRenderView
{
    glm::fmat4 View       = {};
    glm::fmat4 Projection = {};
    glm::fvec3 Camera     = {};
    glm::ivec2 ViewPort   = { 1, 1 };
    Float32 TotalTime     = 0.F;
};

class FSolarSystem final
{

public:

    void Update ( const Float32 DeltaTime ) noexcept;
    void Render ( const FRenderView &RenderView ) const noexcept;
};

} // namespace CS
