/// @file OpenGLMesh.hpp
/// @brief Procedural meshes and OpenGL geometry ownership

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/glm.hpp>

namespace CS
{

class FMeshBuilder;

namespace GL
{

    struct FVertex final
    {
        glm::fvec3 Position = {};
        glm::fvec3 Normal   = {};
    };

    class FOpenGLMesh final
    {
    public:

        FOpenGLMesh () = default;
        FOpenGLMesh ( FOpenGLMesh &&Other ) noexcept;
        FOpenGLMesh &operator=( FOpenGLMesh &&Other ) noexcept;
        FOpenGLMesh ( const FOpenGLMesh & )           = delete;
        FOpenGLMesh &operator=( const FOpenGLMesh & ) = delete;
        ~FOpenGLMesh () noexcept;

        void Draw () const noexcept;

    private:

        friend class CS::FMeshBuilder;

        FOpenGLMesh ( UInt32 InVertexArrayId,
                      UInt32 InVertexBufferId,
                      UInt32 InIndexBufferId,
                      Int32 InVertexCount,
                      Int32 InIndexCount,
                      UInt32 InPrimitiveMode,
                      Bool bInIndexed ) noexcept;

        void Release () noexcept;

        UInt32 VertexArrayId  = 0;
        UInt32 VertexBufferId = 0;
        UInt32 IndexBufferId  = 0;
        Int32 VertexCount     = 0;
        Int32 IndexCount      = 0;
        UInt32 PrimitiveMode  = 0;
        Bool bIndexed         = false;
    };

} // namespace GL

} // namespace CS
