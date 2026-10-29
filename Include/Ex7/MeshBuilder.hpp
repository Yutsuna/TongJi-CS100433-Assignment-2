/// @file MeshBuilder.hpp
/// @brief Mesh building functions

#pragma once

#include <Ex7/OpenGL/OpenGLMesh.hpp>

#include <cstddef>
#include <span>

namespace CS
{

class FMeshBuilder final
{
private:

    enum class EVertexLayout : UInt8
    {
        PositionNormal,
        Position,
        Star,
        Quad,
    };

public:

    [[nodiscard]] static GL::FOpenGLMesh Make ( const std::span<const std::byte> VertexData,
                                                const USize VertexSize,
                                                const UInt32 InVertexCount,
                                                const std::span<const UInt32> Indices,
                                                const UInt32 InPrimitiveMode,
                                                const EVertexLayout Layout );

    [[nodiscard]] static GL::FOpenGLMesh MakeSphere ( UInt32 LongitudeSegmentCount = 64, UInt32 LatitudeSegmentCount = 48 );
    [[nodiscard]] static GL::FOpenGLMesh MakeQuad ();
    [[nodiscard]] static GL::FOpenGLMesh MakeOrbit ( UInt32 SegmentCount = 256 );
    [[nodiscard]] static GL::FOpenGLMesh MakeStarField ( UInt32 StarCount, Float32 MinRadius, Float32 MaxRadius, UInt32 Seed );
};

} // namespace CS
