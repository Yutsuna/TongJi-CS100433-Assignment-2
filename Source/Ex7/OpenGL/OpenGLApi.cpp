/// @file OpenGLApi.cpp
/// @brief OpenGLApi implementation

#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Lib/CoreTypes.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <string>

CS::GL::FOpenGLApi CS::GL::Api = {};

namespace
{

template <typename TFunction> [[nodiscard]] TFunction LoadFunction ( const CS::AnsiChar *const FunctionName )
{
    const GLFWglproc Address = glfwGetProcAddress( FunctionName );
    if ( Address == nullptr )
    {
        throw std::runtime_error( "Unable to load OpenGL function: " + std::string( FunctionName ) );
    }
    return reinterpret_cast<TFunction>( Address );
}

} // namespace

void CS::GL::FOpenGLApi::Load ()
{
    Clear                   = LoadFunction<FClear>( "glClear" );
    ClearColor              = LoadFunction<FClearColor>( "glClearColor" );
    Viewport                = LoadFunction<FViewport>( "glViewport" );
    Enable                  = LoadFunction<FEnable>( "glEnable" );
    Disable                 = LoadFunction<FDisable>( "glDisable" );
    DepthFunc               = LoadFunction<FDepthFunc>( "glDepthFunc" );
    DepthMask               = LoadFunction<FDepthMask>( "glDepthMask" );
    BlendFunc               = LoadFunction<FBlendFunc>( "glBlendFunc" );
    GenVertexArrays         = LoadFunction<FGenVertexArrays>( "glGenVertexArrays" );
    DeleteVertexArrays      = LoadFunction<FDeleteVertexArrays>( "glDeleteVertexArrays" );
    BindVertexArray         = LoadFunction<FBindVertexArray>( "glBindVertexArray" );
    GenBuffers              = LoadFunction<FGenBuffers>( "glGenBuffers" );
    DeleteBuffers           = LoadFunction<FDeleteBuffers>( "glDeleteBuffers" );
    BindBuffer              = LoadFunction<FBindBuffer>( "glBindBuffer" );
    BufferData              = LoadFunction<FBufferData>( "glBufferData" );
    VertexAttribPointer     = LoadFunction<FVertexAttribPointer>( "glVertexAttribPointer" );
    EnableVertexAttribArray = LoadFunction<FEnableVertexAttribArray>( "glEnableVertexAttribArray" );
    DrawArrays              = LoadFunction<FDrawArrays>( "glDrawArrays" );
    DrawElements            = LoadFunction<FDrawElements>( "glDrawElements" );
    CreateShader            = LoadFunction<FCreateShader>( "glCreateShader" );
    ShaderSource            = LoadFunction<FShaderSource>( "glShaderSource" );
    CompileShader           = LoadFunction<FCompileShader>( "glCompileShader" );
    GetShaderParameters     = LoadFunction<FGetShaderParameters>( "glGetShaderiv" );
    GetShaderInfoLog        = LoadFunction<FGetShaderInfoLog>( "glGetShaderInfoLog" );
    DeleteShader            = LoadFunction<FDeleteShader>( "glDeleteShader" );
    CreateProgram           = LoadFunction<FCreateProgram>( "glCreateProgram" );
    AttachShader            = LoadFunction<FAttachShader>( "glAttachShader" );
    LinkProgram             = LoadFunction<FLinkProgram>( "glLinkProgram" );
    GetProgramParameters    = LoadFunction<FGetProgramParameters>( "glGetProgramiv" );
    GetProgramInfoLog       = LoadFunction<FGetProgramInfoLog>( "glGetProgramInfoLog" );
    DeleteProgram           = LoadFunction<FDeleteProgram>( "glDeleteProgram" );
    UseProgram              = LoadFunction<FUseProgram>( "glUseProgram" );
    GetUniformLocation      = LoadFunction<FGetUniformLocation>( "glGetUniformLocation" );
    Uniform1Float           = LoadFunction<FUniform1Float>( "glUniform1f" );
    Uniform3Float           = LoadFunction<FUniform3Float>( "glUniform3f" );
    UniformMatrix4Float     = LoadFunction<FUniformMatrix4Float>( "glUniformMatrix4fv" );
}
