/// @file SolarSystem.hpp
/// @brief Animated solar-system simulation and rendering

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/fwd.hpp>
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

namespace CS
{

struct FRenderView final
{
    glm::fmat4 View       = {};
    glm::fmat4 Projection = {};
    glm::fvec3 Camera     = {};
    glm::ivec2 Viewport   = { 1, 1 };
    Float32 TotalTime     = 0.F;
};

class FSolarSystem final
{
public:

    FSolarSystem ();
    ~FSolarSystem () noexcept;

    FSolarSystem ( const FSolarSystem & )           = delete;
    FSolarSystem &operator=( const FSolarSystem & ) = delete;

    void Update ( Float32 DeltaTime ) noexcept;
    void Render ( const FRenderView &RenderView ) const noexcept;

private:

    struct FResources;
    std::unique_ptr<FResources> Resources;
};

} // namespace CS
