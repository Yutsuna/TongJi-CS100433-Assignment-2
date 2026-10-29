/// @file OpenGLApi
/// @brief OpenGL API mapping

#pragma once

#include <cstddef>

#if defined( __APPLE__ )
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#if !defined( APIENTRY )
    #define APIENTRY
#endif

#ifndef GL_ARRAY_BUFFER
    #define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_ELEMENT_ARRAY_BUFFER
    #define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif
#ifndef GL_STATIC_DRAW
    #define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_FRAGMENT_SHADER
    #define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_VERTEX_SHADER
    #define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_COMPILE_STATUS
    #define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
    #define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
    #define GL_INFO_LOG_LENGTH 0x8B84
#endif
#ifndef GL_PROGRAM_POINT_SIZE
    #define GL_PROGRAM_POINT_SIZE 0x8642
#endif
#ifndef GL_MULTISAMPLE
    #define GL_MULTISAMPLE 0x809D
#endif

namespace CS
{

namespace GL
{

    struct FOpenGLApi final
    {
        using FClear                   = void( APIENTRY * )( GLbitfield );
        using FClearColor              = void( APIENTRY * )( GLfloat, GLfloat, GLfloat, GLfloat );
        using FViewport                = void( APIENTRY * )( GLint, GLint, GLsizei, GLsizei );
        using FEnable                  = void( APIENTRY * )( GLenum );
        using FDisable                 = void( APIENTRY * )( GLenum );
        using FDepthFunc               = void( APIENTRY * )( GLenum );
        using FDepthMask               = void( APIENTRY * )( GLboolean );
        using FBlendFunc               = void( APIENTRY * )( GLenum, GLenum );
        using FGenVertexArrays         = void( APIENTRY * )( GLsizei, GLuint * );
        using FDeleteVertexArrays      = void( APIENTRY * )( GLsizei, const GLuint * );
        using FBindVertexArray         = void( APIENTRY * )( GLuint );
        using FGenBuffers              = void( APIENTRY * )( GLsizei, GLuint * );
        using FDeleteBuffers           = void( APIENTRY * )( GLsizei, const GLuint * );
        using FBindBuffer              = void( APIENTRY * )( GLenum, GLuint );
        using FBufferData              = void( APIENTRY * )( GLenum, std::ptrdiff_t, const void *, GLenum );
        using FVertexAttribPointer     = void( APIENTRY * )( GLuint, GLint, GLenum, GLboolean, GLsizei, const void * );
        using FEnableVertexAttribArray = void( APIENTRY * )( GLuint );
        using FDrawArrays              = void( APIENTRY * )( GLenum, GLint, GLsizei );
        using FDrawElements            = void( APIENTRY * )( GLenum, GLsizei, GLenum, const void * );
        using FCreateShader            = GLuint( APIENTRY * )( GLenum );
        using FShaderSource            = void( APIENTRY * )( GLuint, GLsizei, const char *const *, const GLint * );
        using FCompileShader           = void( APIENTRY * )( GLuint );
        using FGetShaderParameters     = void( APIENTRY * )( GLuint, GLenum, GLint * );
        using FGetShaderInfoLog        = void( APIENTRY * )( GLuint, GLsizei, GLsizei *, char * );
        using FDeleteShader            = void( APIENTRY * )( GLuint );
        using FCreateProgram           = GLuint( APIENTRY * )();
        using FAttachShader            = void( APIENTRY * )( GLuint, GLuint );
        using FLinkProgram             = void( APIENTRY * )( GLuint );
        using FGetProgramParameters    = void( APIENTRY * )( GLuint, GLenum, GLint * );
        using FGetProgramInfoLog       = void( APIENTRY * )( GLuint, GLsizei, GLsizei *, char * );
        using FDeleteProgram           = void( APIENTRY * )( GLuint );
        using FUseProgram              = void( APIENTRY * )( GLuint );
        using FGetUniformLocation      = GLint( APIENTRY * )( GLuint, const char * );
        using FUniform1Float           = void( APIENTRY * )( GLint, GLfloat );
        using FUniform3Float           = void( APIENTRY * )( GLint, GLfloat, GLfloat, GLfloat );
        using FUniformMatrix4Float     = void( APIENTRY * )( GLint, GLsizei, GLboolean, const GLfloat * );

        void Load ();

        FClear Clear                                     = nullptr;
        FClearColor ClearColor                           = nullptr;
        FViewport Viewport                               = nullptr;
        FEnable Enable                                   = nullptr;
        FDisable Disable                                 = nullptr;
        FDepthFunc DepthFunc                             = nullptr;
        FDepthMask DepthMask                             = nullptr;
        FBlendFunc BlendFunc                             = nullptr;
        FGenVertexArrays GenVertexArrays                 = nullptr;
        FDeleteVertexArrays DeleteVertexArrays           = nullptr;
        FBindVertexArray BindVertexArray                 = nullptr;
        FGenBuffers GenBuffers                           = nullptr;
        FDeleteBuffers DeleteBuffers                     = nullptr;
        FBindBuffer BindBuffer                           = nullptr;
        FBufferData BufferData                           = nullptr;
        FVertexAttribPointer VertexAttribPointer         = nullptr;
        FEnableVertexAttribArray EnableVertexAttribArray = nullptr;
        FDrawArrays DrawArrays                           = nullptr;
        FDrawElements DrawElements                       = nullptr;
        FCreateShader CreateShader                       = nullptr;
        FShaderSource ShaderSource                       = nullptr;
        FCompileShader CompileShader                     = nullptr;
        FGetShaderParameters GetShaderParameters         = nullptr;
        FGetShaderInfoLog GetShaderInfoLog               = nullptr;
        FDeleteShader DeleteShader                       = nullptr;
        FCreateProgram CreateProgram                     = nullptr;
        FAttachShader AttachShader                       = nullptr;
        FLinkProgram LinkProgram                         = nullptr;
        FGetProgramParameters GetProgramParameters       = nullptr;
        FGetProgramInfoLog GetProgramInfoLog             = nullptr;
        FDeleteProgram DeleteProgram                     = nullptr;
        FUseProgram UseProgram                           = nullptr;
        FGetUniformLocation GetUniformLocation           = nullptr;
        FUniform1Float Uniform1Float                     = nullptr;
        FUniform3Float Uniform3Float                     = nullptr;
        FUniformMatrix4Float UniformMatrix4Float         = nullptr;
    };

    class FOpenGLWindow;

    /// @class FGlfwRuntime
    /// @brief OpenGL Runtime
    class FGlfwRuntime final
    {
    public:

        FGlfwRuntime ();
        ~FGlfwRuntime () noexcept;

        FGlfwRuntime ( const FGlfwRuntime & )           = delete;
        FGlfwRuntime &operator=( const FGlfwRuntime & ) = delete;
    };

    /// @class FOpenGLContext
    /// @brief OpenGL API Context
    class FOpenGLContext final
    {
    public:

        explicit FOpenGLContext ( const FOpenGLWindow &InWindow );

        FOpenGLContext ( const FOpenGLContext & )           = delete;
        FOpenGLContext &operator=( const FOpenGLContext & ) = delete;

    public:

        [[nodiscard]] const FOpenGLApi &GetApi () const noexcept;
        void SwapBuffers ( const FOpenGLWindow &InWindow ) const noexcept;

    private:

        FOpenGLApi Api;
    };

} // namespace GL

} // namespace CS
