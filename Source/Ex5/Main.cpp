/// @file Main.cpp
/// @brief Exercise 5

#include <Lib/Check.hpp>
#include <Lib/Format.hpp>
#include <Lib/Maths.hpp>

CS::Int32 main () noexcept
{
    CS::Bool Holds = true;

    const glm::dmat4 Proj = CS::Perspective( glm::radians( 90.0 ), 1.0, 1.0, 10.0 );

    /// x_eye = -z_eye = 2
    /// -> x_clip = w_clip
    const glm::dvec4 EyeEdge( 2.0, 0.0, -2.0, 1.0 );
    const glm::dvec4 P = Proj * EyeEdge;

    /// -w <= x <= w -> linear
    Holds &= CS::Check( "Clip test is linear: x == w", P.x, P.w );

    /// x_NDC = 1.0
    const glm::dvec4 Ndc = P / P.w;
    Holds &= CS::Check( "NDC x is normalized to 1.0", Ndc.x, 1.0 );

    /// Vertice A  x=0, z=-2 (w=2)
    const glm::dvec4 A( 0.0, 0.0, -2.0, 1.0 );
    /// Vertice B  x=10, z=-10 (w=10)
    const glm::dvec4 B( 10.0, 0.0, -10.0, 1.0 );

    /// Clip space linear
    const glm::dvec4 ClipA = Proj * A;
    const glm::dvec4 ClipB = Proj * B;
    const glm::dvec4 ClipMid = ClipA * 0.5 + ClipB * 0.5;

    /// NDC division first
    const glm::dvec4 NdcA = ClipA / ClipA.w;
    const glm::dvec4 NdcB = ClipB / ClipB.w;
    const glm::dvec4 NdcMid = NdcA * 0.5 + NdcB * 0.5;

    const glm::dvec4 NdcMidCorrect = ClipMid / ClipMid.w;

    std::println( "Interpolated after divide: x = {:.4f}", NdcMid.x );
    std::println( "Divided after interpolate: x = {:.4f}", NdcMidCorrect.x );

    Holds &= CS::Check( "Linear interpolation in NDC is wrong", std::abs( NdcMid.x - NdcMidCorrect.x ) > CS::EPSILON, true );

    return static_cast<CS::Int32>( Holds );
}
