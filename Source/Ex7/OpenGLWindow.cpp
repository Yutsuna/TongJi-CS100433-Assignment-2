/// @file OpenGLWindow.cpp
/// @brief OpenGLWindow implementation

#define GLFW_INCLUDE_NONE
#include <Ex7/OpenGLWindow.hpp>
#include <Ex7/OpenGLApi.hpp>

#include <GLFW/glfw3.h>

#include <stdexcept>

///
/// FGlfwRuntime
///

CS::GL::FGlfwRuntime::FGlfwRuntime ()
{
#if defined( __linux__ )
    glfwInitHint( GLFW_PLATFORM, GLFW_PLATFORM_X11 );
#endif
    if ( glfwInit() != GLFW_TRUE )
    {
        throw std::runtime_error( "Unable to initialize GLFW." );
    }
}

CS::GL::FGlfwRuntime::~FGlfwRuntime () noexcept
{
    glfwTerminate();
}

///
/// FOpenGLWindow
///

CS::GL::FOpenGLWindow::FOpenGLWindow ( Int32 Width, Int32 Height, const AnsiChar *Title )
{
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_SAMPLES, 0 );
#if defined( __APPLE__ )
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#endif

    WindowHandle = glfwCreateWindow( Width, Height, Title, nullptr, nullptr );
    if ( WindowHandle == nullptr )
    {
        throw std::runtime_error( "Unable to create the GLFW window or its OpenGL 3.3 context." );
    }

    glfwSetWindowUserPointer( WindowHandle, this );
    glfwSetFramebufferSizeCallback( WindowHandle, FramebufferSizeCallback );
    glfwGetFramebufferSize( WindowHandle, &FramebufferWidth, &FramebufferHeight );
}

CS::GL::FOpenGLWindow::~FOpenGLWindow () noexcept
{
    glfwDestroyWindow( WindowHandle );
}

GLFWwindow *CS::GL::FOpenGLWindow::GetNativeHandle () const noexcept
{
    return WindowHandle;
}

glm::ivec2 CS::GL::FOpenGLWindow::GetFrameBufferSize () const noexcept
{
    return { FramebufferWidth, FramebufferHeight };
}

CS::Bool CS::GL::FOpenGLWindow::ShouldClose () const noexcept
{
    return glfwWindowShouldClose( WindowHandle ) == GLFW_TRUE;
}

void CS::GL::FOpenGLWindow::RequestClose () const noexcept
{
    glfwSetWindowShouldClose( WindowHandle, GLFW_TRUE );
}

void CS::GL::FOpenGLWindow::PollEvents () noexcept
{
    glfwPollEvents();
}

void CS::GL::FOpenGLWindow::FramebufferSizeCallback ( GLFWwindow *Handle, Int32 Width, Int32 Height ) noexcept
{
    FOpenGLWindow *Window = static_cast<FOpenGLWindow *>( glfwGetWindowUserPointer( Handle ) );

    if ( Window != nullptr )
    {
        Window->FramebufferWidth  = Width;
        Window->FramebufferHeight = Height;
    }
}

CS::GL::FOpenGLContext::FOpenGLContext ( const FOpenGLWindow &Window )
{
    glfwMakeContextCurrent( Window.GetNativeHandle() );
    glfwSwapInterval( 1 );
    Api.Load();
}

const CS::GL::FOpenGLApi &CS::GL::FOpenGLContext::GetApi () const noexcept
{
    return Api;
}

void CS::GL::FOpenGLContext::SwapBuffers ( const FOpenGLWindow &Window ) const noexcept
{
    glfwSwapBuffers( Window.GetNativeHandle() );
}
