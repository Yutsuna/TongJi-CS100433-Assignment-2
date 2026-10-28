/// @file Main.cpp
/// @brief Exercise 6

#include "Lib/CoreTypes.hpp"
#include <Lib/Check.hpp>
#include <Lib/Format.hpp>
#include <Lib/Maths.hpp>

namespace
{

constexpr CS::Float64 Far      = 1000.0;
constexpr CS::Float64 NearBad  = 0.1;
constexpr CS::Float64 NearGood = 10.0;

constexpr CS::Float64 Z1 = 100.0;
constexpr CS::Float64 Z2 = 100.001;

constexpr CS::UInt32 DepthBits = 24;
constexpr CS::UInt32 MaxDepth  = ( 1U << DepthBits ) - 1U;

/// Converts a positive view-space distance into the OpenGL depth-buffer range [0, 1].
CS::Float64 Depth ( CS::Float64 Near, CS::Float64 Far, CS::Float64 Distance ) noexcept
{
    const glm::dmat4 Projection = glm::perspectiveRH_NO( glm::radians( 60.0 ), 1.0, Near, Far );
    const glm::dvec4 Clip       = Projection * glm::dvec4( 0.0, 0.0, -Distance, 1.0 );
    const CS::Float64 NdcZ      = Clip.z / Clip.w;

    return ( NdcZ + 1.0 ) * 0.5;
}

/// Emulates quantization in a 24-bit depth buffer.
CS::UInt32 Quantize ( CS::Float64 Value ) noexcept
{
    return static_cast<CS::UInt32>( std::llround( Value * MaxDepth ) );
}

} // namespace

CS::Int32 main () noexcept
{
    CS::Bool Holds = true;

    const CS::Float64 D1Bad = Depth( NearBad, Far, Z1 );
    const CS::Float64 D2Bad = Depth( NearBad, Far, Z2 );

    const CS::UInt32 Q1Bad = Quantize( D1Bad );
    const CS::UInt32 Q2Bad = Quantize( D2Bad );

    const CS::Float64 D1Good = Depth( NearGood, Far, Z1 );
    const CS::Float64 D2Good = Depth( NearGood, Far, Z2 );

    const CS::UInt32 Q1Good = Quantize( D1Good );
    const CS::UInt32 Q2Good = Quantize( D2Good );


    std::println("Near = {}, Far = {}", NearBad, Far);
    std::println("Depth difference: {:.12f}", std::abs(D2Bad - D1Bad));
    std::println("Quantized depth values: {}, {}\n", Q1Bad, Q2Bad);
    std::println("Near = {}, Far = {}", NearGood, Far);
    std::println("Depth difference: {:.12f}", std::abs(D2Good - D1Good));
    std::println("Quantized depth values: {}, {}\n", Q1Good, Q2Good);

    Holds &= CS::Check( "Large near/far ratio loses depth precision", Q1Bad == Q2Bad, true );
    Holds &= CS::Check( "Moving the near plane improves depth precision", Q1Good != Q2Good, true );

    return static_cast<CS::Int32>( Holds );
}
