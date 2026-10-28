/// @file Main.cpp
/// @brief Exercise 4

#include <Lib/Check.hpp>
#include <Lib/Format.hpp>
#include <Lib/Maths.hpp>

CS::Int32 main () noexcept
{
    CS::Bool Holds = true;

    const glm::dmat4 Proj = CS::Perspective( glm::radians( 60.0 ), 1.0, 1.0, 10.0 );

    /// Point in front of the camera (w = 2)
    const glm::dvec4 ClipFront = Proj * glm::dvec4( 1.0, 0.0, -2.0, 1.0 );
    /// Point behind the camera (w = -2)
    const glm::dvec4 ClipBack  = Proj * glm::dvec4( 1.0, 0.0,  2.0, 1.0 );

    /// Clip Space linear test -w <= x <= w
    Holds &= CS::Check( "Front visible in Clip", ClipFront.x <=  ClipFront.w, true );
    Holds &= CS::Check( "Back NOT visible in Clip", ClipBack.x  <=  ClipBack.w,  false );

    /// NDC division by w
    const glm::dvec4 NdcFront = ClipFront / ClipFront.w;
    const glm::dvec4 NdcBack  = ClipBack  / ClipBack.w;

    /// Inversion proof
    Holds &= CS::Check( "Front x positive", NdcFront.x > 0.0, true );
    Holds &= CS::Check( "Back x negative", NdcBack.x < 0.0, true );
    Holds &= CS::Check( "Back falsely in NDC [-1,1]", std::abs( NdcBack.x ) <= 1.0, true );

    return static_cast<CS::Int32>( Holds );
}
