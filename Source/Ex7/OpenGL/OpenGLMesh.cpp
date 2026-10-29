/// @file OpenGLMesh.cpp
/// @brief Procedural geometry generation and OpenGL buffer management

#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Ex7/OpenGL/OpenGLMesh.hpp>

#include <glm/vec2.hpp>
#include <utility>

CS::GL::FOpenGLMesh::FOpenGLMesh ( const UInt32 InVertexArrayId,
                                   const UInt32 InVertexBufferId,
                                   const UInt32 InIndexBufferId,
                                   const Int32 InVertexCount,
                                   const Int32 InIndexCount,
                                   const UInt32 InPrimitiveMode,
                                   const Bool bInIndexed ) noexcept
    : VertexArrayId( InVertexArrayId ), VertexBufferId( InVertexBufferId ), IndexBufferId( InIndexBufferId ),
      VertexCount( InVertexCount ), IndexCount( InIndexCount ), PrimitiveMode( InPrimitiveMode ), bIndexed( bInIndexed )
{
    ///
}

CS::GL::FOpenGLMesh::FOpenGLMesh ( FOpenGLMesh &&Other ) noexcept
    : VertexArrayId( std::exchange( Other.VertexArrayId, 0 ) ), VertexBufferId( std::exchange( Other.VertexBufferId, 0 ) ),
      IndexBufferId( std::exchange( Other.IndexBufferId, 0 ) ), VertexCount( std::exchange( Other.VertexCount, 0 ) ),
      IndexCount( std::exchange( Other.IndexCount, 0 ) ), PrimitiveMode( std::exchange( Other.PrimitiveMode, 0 ) ),
      bIndexed( std::exchange( Other.bIndexed, false ) )
{
    ///
}

CS::GL::FOpenGLMesh &CS::GL::FOpenGLMesh::operator=( FOpenGLMesh &&Other ) noexcept
{
    if ( this != &Other )
    {
        Release();
        VertexArrayId  = std::exchange( Other.VertexArrayId, 0 );
        VertexBufferId = std::exchange( Other.VertexBufferId, 0 );
        IndexBufferId  = std::exchange( Other.IndexBufferId, 0 );
        VertexCount    = std::exchange( Other.VertexCount, 0 );
        IndexCount     = std::exchange( Other.IndexCount, 0 );
        PrimitiveMode  = std::exchange( Other.PrimitiveMode, 0 );
        bIndexed       = std::exchange( Other.bIndexed, false );
    }
    return *this;
}

CS::GL::FOpenGLMesh::~FOpenGLMesh() noexcept
{
    Release();
}

void CS::GL::FOpenGLMesh::Draw () const noexcept
{
    if ( VertexArrayId == 0 )
    {
        return;
    }

    Api.BindVertexArray( VertexArrayId );
    if ( bIndexed )
    {
        Api.DrawElements( PrimitiveMode, IndexCount, GL_UNSIGNED_INT, nullptr );
    }
    else
    {
        Api.DrawArrays( PrimitiveMode, 0, VertexCount );
    }
}

void CS::GL::FOpenGLMesh::Release () noexcept
{
    if ( IndexBufferId != 0 )
    {
        Api.DeleteBuffers( 1, &IndexBufferId );
    }
    if ( VertexBufferId != 0 )
    {
        Api.DeleteBuffers( 1, &VertexBufferId );
    }
    if ( VertexArrayId != 0 )
    {
        Api.DeleteVertexArrays( 1, &VertexArrayId );
    }

    VertexArrayId  = 0;
    VertexBufferId = 0;
    IndexBufferId  = 0;
    VertexCount    = 0;
    IndexCount     = 0;
    PrimitiveMode  = 0;
    bIndexed       = false;
}
