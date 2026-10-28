/// @file Main.cpp
/// @brief Exercise 3 - Proof of Default NDC

#include <Lib/Check.hpp>
#include <Lib/Format.hpp>
#include <Lib/Maths.hpp>

CS::Int32 main () noexcept
{
    CS::Bool Holds = true;

    /// Near=1, Far=-1
    const glm::dmat4 Correct = CS::Orthographic( -1.0, 1.0, -1.0, 1.0, 1.0, -1.0 );
    /// Near=-1, Far=1
    const glm::dmat4 Wrong = CS::Orthographic( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );

    Holds &= CS::Check( "Orthographic(-1,1, -1,1, 1,-1) == Identity", Correct, glm::dmat4( 1.0 ) );

    constexpr glm::dvec4 P( 0.0, 0.0, -1.0, 1.0 );

    const glm::dvec4 NdcCorrect = Correct * P;
    const glm::dvec4 NdcWrong   = Wrong * P;

    Holds &= CS::Check( "Correct Ortho maps Near (Z=-1) to NDC Z=-1", NdcCorrect.z, -1.0 );
    Holds &= CS::Check( "Wrong Ortho maps Near (Z=-1) to NDC Z=+1", NdcWrong.z, 1.0 );

    return static_cast<CS::Int32>( Holds );
}
