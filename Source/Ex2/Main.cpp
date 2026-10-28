/// @file Main.cpp
/// @brief Exercise 2

#include <Lib/Check.hpp>
#include <Lib/CoreTypes.hpp>
#include <Lib/Format.hpp>
#include <Lib/Maths.hpp>
#include <Lib/Reflect.hpp>

namespace
{

template <class TFunc>
void ForEach ( std::string_view Name,
               const glm::dmat4 &Matrix,
               const std::array<glm::dvec4, 8> &Corners,
               const std::array<glm::dvec4, 8> &Expected,
               TFunc &&Callback ) noexcept
{
    for ( CS::USize Idx = 0; Idx < Corners.size(); ++Idx )
    {
        glm::dvec4 P = Matrix * Corners[Idx];

        if ( std::abs( P.w ) > CS::EPSILON )
        {
            P /= P.w;
        }
        Callback( std::format( "{} corner {} -> NDC", Name, Idx ), P, Expected[Idx] );
    }
}

struct FProjection final
{
    [[= CS::FReflect{ "Ortho" }]] glm::dmat4 Ortho;
    [[= CS::FReflect{ "Perspective" }]] glm::dmat4 Perspective;

    static constexpr CS::Float64 L = -2.0, R = 3.0, B = -1.5, T = 2.5, Near = 0.5, Far = 10.0;
    static constexpr CS::Float64 FovY = glm::radians( 60.0 ), Aspect = 16.0 / 9.0;

    [[nodiscard]] CS::Bool Verify () const noexcept
    {
        CS::Bool Holds = true;

        Holds &= CS::Check( "Ortho matches glm::ortho", Ortho, glm::ortho( L, R, B, T, Near, Far ) );
        Holds &= CS::Check( "Perspective matches glm::perspective", Perspective, glm::perspective( FovY, Aspect, Near, Far ) );

        const std::array<glm::dvec4, 8> ExpectedNdc = { glm::dvec4( -1, -1, -1, 1 ), glm::dvec4( 1, -1, -1, 1 ),
                                                        glm::dvec4( -1, 1, -1, 1 ), glm::dvec4( 1, 1, -1, 1 ),
                                                        glm::dvec4( -1, -1, 1, 1 ),  glm::dvec4( 1, -1, 1, 1 ),
                                                        glm::dvec4( -1, 1, 1, 1 ),  glm::dvec4( 1, 1, 1, 1 ) };
        const std::array<glm::dvec4, 8> OrthoCorners = { glm::dvec4( L, B, -Near, 1 ), glm::dvec4( R, B, -Near, 1 ),
                                                         glm::dvec4( L, T, -Near, 1 ), glm::dvec4( R, T, -Near, 1 ),
                                                         glm::dvec4( L, B, -Far, 1 ),  glm::dvec4( R, B, -Far, 1 ),
                                                         glm::dvec4( L, T, -Far, 1 ),  glm::dvec4( R, T, -Far, 1 ) };

        ForEach( "Ortho", Ortho, OrthoCorners, ExpectedNdc,
                 [&] ( std::string_view Name, auto P, auto Exp )
                 {
                     Holds &= CS::Check( Name, P, Exp );
                 } );

        const CS::Float64 TanHalf = std::tan( FovY / 2.0 );
        const CS::Float64 Nx = Near * TanHalf * Aspect, Ny = Near * TanHalf;
        const CS::Float64 Fx = Far * TanHalf * Aspect, Fy = Far * TanHalf;

        const std::array<glm::dvec4, 8> PerspCorners = { glm::dvec4( -Nx, -Ny, -Near, 1 ), glm::dvec4( Nx, -Ny, -Near, 1 ),
                                                         glm::dvec4( -Nx, Ny, -Near, 1 ),  glm::dvec4( Nx, Ny, -Near, 1 ),
                                                         glm::dvec4( -Fx, -Fy, -Far, 1 ),  glm::dvec4( Fx, -Fy, -Far, 1 ),
                                                         glm::dvec4( -Fx, Fy, -Far, 1 ),   glm::dvec4( Fx, Fy, -Far, 1 ) };

        ForEach( "Persp", Perspective, PerspCorners, ExpectedNdc,
                 [&] ( std::string_view Name, auto P, auto Exp )
                 {
                     Holds &= CS::Check( Name, P, Exp );
                 } );

        return Holds;
    }

    void Demonstrate () const noexcept
    {
        const glm::dvec4 P( 1.5, -0.5, -3.0, 1.0 );
        std::println( "\nPoint P in View Space: {}", CS::Format( P ) );

        /// Orthographic
        std::println( "\nOrthographic" );
        const glm::dvec4 ClipO = Ortho * P;
        std::println( "Clip space: {}", CS::Format( ClipO ) );
        std::println( "-> w_clip = 1 (no perspective divide needed)" );
        std::println( "NDC space : {}", CS::Format( ClipO / ClipO.w ) );

        /// Perspective
        std::println( "\nPerspective" );
        const glm::dvec4 ClipP = Perspective * P;
        std::println( "Clip space: {}", CS::Format( ClipP ) );
        std::println( "-> w_clip = -z_eye = {:.1f}", ClipP.w );
        std::println( "NDC space : {}", CS::Format( ClipP / ClipP.w ) );

        /// Foreshortening
        std::println( "\nForeshortening Proof" );
        const glm::dvec4 P1( 1.0, 0.0, -2.0, 1.0 ); ///<< Z = -2
        const glm::dvec4 P2( 1.0, 0.0, -4.0, 1.0 ); ///<< Z = -4 (twice as far)

        const CS::Float64 NdcX1 = ( Perspective * P1 ).x / ( Perspective * P1 ).w;
        const CS::Float64 NdcX2 = ( Perspective * P2 ).x / ( Perspective * P2 ).w;

        std::println( "P1 (z=-2) -> x_ndc = {:.4f}", NdcX1 );
        std::println( "P2 (z=-4) -> x_ndc = {:.4f}", NdcX2 );
        std::println( "-> Twice as far = half the size on screen." );
    }
};

} // namespace

CS::Int32 main () noexcept
{
    const FProjection Example = {
        .Ortho       = CS::Orthographic( FProjection::L, FProjection::R, FProjection::B, FProjection::T, FProjection::Near, FProjection::Far ),
        .Perspective = CS::Perspective( FProjection::FovY, FProjection::Aspect, FProjection::Near, FProjection::Far ) };

    CS::Dump( Example );

    const CS::Bool ReturnValue = Example.Verify();

    Example.Demonstrate();
    return static_cast<CS::Int32>( ReturnValue );
}
