/// @file Main.cpp
/// @brief Ex7

#include <Ex7/Application.hpp>

#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Ex7/OpenGL/OpenGLWindow.hpp>

#include <exception>
#include <print>

CS::Int32 main () noexcept
{
    try
    {
        CS::GL::FGlfwRuntime Runtime;
        CS::GL::FOpenGLWindow Window( 1280, 720, "Procedural Solar System" );
        CS::GL::FOpenGLContext Context( Window );
        CS::FApplication Application( Window, Context );
        Application.Run();
    }
    catch ( const std::exception &Exception )
    {
        std::println( stderr, "FATAL ERROR: {}", Exception.what() );
        return 84;
    }
    return 0;
}
