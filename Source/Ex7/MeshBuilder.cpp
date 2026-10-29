/// @file Mesh Builder
/// @brief Mesh Buider Factory

#include <Ex7/MeshBuilder.hpp>
#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Ex7/OpenGL/OpenGLMesh.hpp>

#include <random>
#include <vector>
#include <span>

///
/// Private
///

namespace {


    struct FStarVertex final
    {
        glm::fvec3 Position = {};
        CS::Float32 Temperature = 0.F;
        CS::Float32 Size        = 1.F;
        CS::Float32 Phase       = 0.F;
    };


}

///
/// Public
///

CS::GL::FOpenGLMesh CS::FMeshBuilder::Make ( const std::span<const std::byte> VertexData,
                                             const USize VertexSize,
                                             const UInt32 InVertexCount,
                                             const std::span<const UInt32> Indices,
                                             const UInt32 InPrimitiveMode,
                                             const EVertexLayout Layout )
{
    UInt32 VertexArray  = 0;
    UInt32 VertexBuffer = 0;
    UInt32 IndexBuffer  = 0;

    GL::Api.GenVertexArrays( 1, &VertexArray );
    GL::Api.GenBuffers( 1, &VertexBuffer );
    GL::Api.BindVertexArray( VertexArray );
    GL::Api.BindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
    GL::Api.BufferData( GL_ARRAY_BUFFER, static_cast<std::ptrdiff_t>( VertexData.size_bytes() ), VertexData.data(), GL_STATIC_DRAW );

    if ( not Indices.empty() )
    {
        GL::Api.GenBuffers( 1, &IndexBuffer );
        GL::Api.BindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer );
        GL::Api.BufferData( GL_ELEMENT_ARRAY_BUFFER, static_cast<std::ptrdiff_t>( Indices.size_bytes() ), Indices.data(),
                        GL_STATIC_DRAW );
    }

    const auto Stride = static_cast<GLsizei>( VertexSize );
    switch ( Layout )
    {
    case EVertexLayout::PositionNormal:
        GL::Api.VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, Stride, nullptr );
        GL::Api.VertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, Stride,
                                 reinterpret_cast<const void *>( offsetof( GL::FVertex, Normal ) ) );
        GL::Api.EnableVertexAttribArray( 0 );
        GL::Api.EnableVertexAttribArray( 1 );
        break;
    case EVertexLayout::Position:
        GL::Api.VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, Stride, nullptr );
        GL::Api.EnableVertexAttribArray( 0 );
        break;
    case EVertexLayout::Star:
        GL::Api.VertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, Stride, nullptr );
        GL::Api.VertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, Stride,
                                 reinterpret_cast<const void *>( offsetof( FStarVertex, Temperature ) ) );
        GL::Api.VertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, Stride,
                                 reinterpret_cast<const void *>( offsetof( FStarVertex, Size ) ) );
        GL::Api.VertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, Stride,
                                 reinterpret_cast<const void *>( offsetof( FStarVertex, Phase ) ) );
        GL::Api.EnableVertexAttribArray( 0 );
        GL::Api.EnableVertexAttribArray( 1 );
        GL::Api.EnableVertexAttribArray( 2 );
        GL::Api.EnableVertexAttribArray( 3 );
        break;
    case EVertexLayout::Quad:
        GL::Api.VertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, Stride, nullptr );
        GL::Api.EnableVertexAttribArray( 0 );
        break;
    }

    GL::Api.BindVertexArray( 0 );
    return GL::FOpenGLMesh( VertexArray, VertexBuffer, IndexBuffer, static_cast<Int32>( InVertexCount ),
                            static_cast<Int32>( Indices.size() ), InPrimitiveMode, not Indices.empty() );
}

CS::GL::FOpenGLMesh CS::FMeshBuilder::MakeSphere ( UInt32 LongitudeSegmentCount, UInt32 LatitudeSegmentCount )
{
    LongitudeSegmentCount = std::max( LongitudeSegmentCount, 3U );
    LatitudeSegmentCount  = std::max( LatitudeSegmentCount, 2U );

    std::vector<GL::FVertex> Vertices;
    std::vector<UInt32> Indices;
    Vertices.reserve( static_cast<USize>( LatitudeSegmentCount + 1 ) * ( LongitudeSegmentCount + 1 ) );
    Indices.reserve( static_cast<USize>( LatitudeSegmentCount ) * LongitudeSegmentCount * 6 );

    constexpr Float32 Pi = std::numbers::pi_v<Float32>;
    for ( UInt32 Latitude = 0; Latitude <= LatitudeSegmentCount; ++Latitude )
    {
        const Float32 Theta    = Pi * static_cast<Float32>( Latitude ) / static_cast<Float32>( LatitudeSegmentCount );
        const Float32 SinTheta = std::sin( Theta );
        const Float32 CosTheta = std::cos( Theta );

        for ( UInt32 Longitude = 0; Longitude <= LongitudeSegmentCount; ++Longitude )
        {
            const Float32 Phi = 2.F * Pi * static_cast<Float32>( Longitude ) / static_cast<Float32>( LongitudeSegmentCount );
            const glm::fvec3 Normal{ SinTheta * std::cos( Phi ), CosTheta, SinTheta * std::sin( Phi ) };
            Vertices.push_back( { .Position = Normal, .Normal = Normal } );
        }
    }

    const UInt32 RowSize = LongitudeSegmentCount + 1;
    for ( UInt32 Latitude = 0; Latitude < LatitudeSegmentCount; ++Latitude )
    {
        for ( UInt32 Longitude = 0; Longitude < LongitudeSegmentCount; ++Longitude )
        {
            const UInt32 First  = Latitude * RowSize + Longitude;
            const UInt32 Second = First + RowSize;
            Indices.insert( Indices.end(), { First, First + 1, Second, Second, First + 1, Second + 1 } );
        }
    }

    return Make( std::as_bytes( std::span<const GL::FVertex>( Vertices ) ), sizeof( GL::FVertex ),
                                  static_cast<UInt32>( Vertices.size() ), Indices, GL_TRIANGLES, EVertexLayout::PositionNormal );
}

CS::GL::FOpenGLMesh CS::FMeshBuilder::MakeQuad ()
{
    constexpr std::array Corners{ glm::fvec2{ -1.F, -1.F }, glm::fvec2{ 1.F, -1.F }, glm::fvec2{ -1.F, 1.F },
                                  glm::fvec2{ 1.F, 1.F } };
    return Make( std::as_bytes( std::span<const glm::fvec2>( Corners ) ), sizeof( glm::fvec2 ),
                                  static_cast<UInt32>( Corners.size() ), {}, GL_TRIANGLE_STRIP, EVertexLayout::Quad );
}

CS::GL::FOpenGLMesh CS::FMeshBuilder::MakeOrbit ( const UInt32 SegmentCount )
{
    const UInt32 ClampedSegmentCount = std::max( SegmentCount, 16U );
    std::vector<glm::fvec3> Positions;
    Positions.reserve( ClampedSegmentCount );

    constexpr Float32 TwoPi = 2.F * std::numbers::pi_v<Float32>;
    for ( UInt32 Segment = 0; Segment < ClampedSegmentCount; ++Segment )
    {
        const Float32 Angle = TwoPi * static_cast<Float32>( Segment ) / static_cast<Float32>( ClampedSegmentCount );
        Positions.emplace_back( std::cos( Angle ), 0.F, std::sin( Angle ) );
    }

    return Make( std::as_bytes( std::span<const glm::fvec3>( Positions ) ), sizeof( glm::fvec3 ),
                                  static_cast<UInt32>( Positions.size() ), {}, GL_LINE_LOOP, EVertexLayout::Position );
}

CS::GL::FOpenGLMesh
CS::FMeshBuilder::MakeStarField ( const UInt32 StarCount, const Float32 MinRadius, const Float32 MaxRadius, const UInt32 Seed )
{
    std::mt19937 Random( Seed );
    std::uniform_real_distribution<Float32> UnitDistribution( 0.F, 1.F );
    std::uniform_real_distribution<Float32> RadiusDistribution( MinRadius, MaxRadius );
    constexpr Float32 TwoPi = 2.F * std::numbers::pi_v<Float32>;

    std::vector<FStarVertex> Stars;
    Stars.reserve( StarCount );
    for ( UInt32 Index = 0; Index < StarCount; ++Index )
    {
        const Float32 Z         = UnitDistribution( Random ) * 2.F - 1.F;
        const Float32 Longitude = UnitDistribution( Random ) * TwoPi;
        const Float32 Ring      = std::sqrt( std::max( 0.F, 1.F - Z * Z ) );
        const Float32 Radius    = RadiusDistribution( Random );

        Stars.push_back(
            { .Position    = { Radius * Ring * std::cos( Longitude ), Radius * Z, Radius * Ring * std::sin( Longitude ) },
              .Temperature = UnitDistribution( Random ),
              .Size        = 0.8F + 2.2F * std::pow( UnitDistribution( Random ), 3.F ),
              .Phase       = UnitDistribution( Random ) } );
    }

    return Make( std::as_bytes( std::span<const FStarVertex>( Stars ) ), sizeof( FStarVertex ),
                                  static_cast<UInt32>( Stars.size() ), {}, GL_POINTS, EVertexLayout::Star );
}
