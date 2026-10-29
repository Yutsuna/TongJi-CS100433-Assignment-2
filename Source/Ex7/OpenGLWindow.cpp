/// @file OpenGLWindow.cpp
/// @brief OpenGLWindow implementation

#include "Lib/CoreTypes.hpp"
#include <algorithm>
#include <glm/fwd.hpp>
#define GLFW_INCLUDE_NONE

#include <Ex7/OpenGLApi.hpp>
#include <Ex7/OpenGLWindow.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

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
/// Private
///

struct CS::GL::FWindowState final
{
    GLFWwindow *WindowHandle = nullptr;
    std::vector<CS::Int32> PressedKeys;
    CS::Float32 ScrollDelta = 0.F;
    glm::fvec2 DragDelta    = {};
    glm::dvec2 LastCursor   = {};
    CS::Bool bWasDragging   = false;
};

namespace
{

CS::GL::FWindowState *GetWindowState ( GLFWwindow *InWindowHandle ) noexcept
{
    return static_cast<CS::GL::FWindowState *>( glfwGetWindowUserPointer( InWindowHandle ) );
}

void OnKey ( GLFWwindow *InWindowHandle, const CS::Int32 Key, CS::Int32, const CS::Int32 Action, CS::Int32 )
{
    if ( Action == GLFW_RELEASE )
    {
        return;
    }
    GetWindowState( InWindowHandle )->PressedKeys.push_back( Key );
}

void OnScroll ( GLFWwindow *InWindowHandle, CS::Float64, const CS::Float64 OffsetY ) noexcept
{
    GetWindowState( InWindowHandle )->ScrollDelta += static_cast<CS::Float32>( OffsetY );
}

} // namespace

///
/// FOpenGLWindow
///

CS::GL::FOpenGLWindow::FOpenGLWindow ( Int32 Width, Int32 Height, const AnsiChar *Title )
{
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_SAMPLES, 4 );
#if defined( __APPLE__ )
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#endif

    auto *WindowHandle = glfwCreateWindow( Width, Height, Title, nullptr, nullptr );
    if ( WindowHandle == nullptr )
    {
        throw std::runtime_error( "Unable to create the GLFW window or its OpenGL 3.3 context." );
    }

    auto NewWindowState          = std::make_unique<FWindowState>();
    NewWindowState->WindowHandle = WindowHandle;

    glfwSetWindowUserPointer( WindowHandle, NewWindowState.get() );
    glfwSetKeyCallback( WindowHandle, &OnKey );
    glfwSetScrollCallback( WindowHandle, &OnScroll );

    WindowState = std::move( NewWindowState );
}

CS::GL::FOpenGLWindow::~FOpenGLWindow () noexcept
{
    glfwDestroyWindow( WindowState->WindowHandle );
}

CS::Bool CS::GL::FOpenGLWindow::ShouldClose () const noexcept
{
    return glfwWindowShouldClose( WindowState->WindowHandle ) == GLFW_TRUE;
}

void CS::GL::FOpenGLWindow::RequestClose () const noexcept
{
    glfwSetWindowShouldClose( WindowState->WindowHandle, GLFW_TRUE );
}

void CS::GL::FOpenGLWindow::PollEvents () noexcept
{
    WindowState->PressedKeys.clear();
    WindowState->ScrollDelta = 0.F;
    glfwPollEvents();

    glm::dvec2 Cursor = {};
    glfwGetCursorPos( WindowState->WindowHandle, &Cursor.x, &Cursor.y );

    const Bool bIsDragging = glfwGetMouseButton( WindowState->WindowHandle, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS;

    WindowState->DragDelta = ( bIsDragging and WindowState->bWasDragging )
                                 ? glm::fvec2{ static_cast<Float32>( Cursor.x - WindowState->LastCursor.x ),
                                               static_cast<Float32>( Cursor.y - WindowState->LastCursor.y ) }
                                 : glm::fvec2{};

    WindowState->bWasDragging = bIsDragging;
    WindowState->LastCursor   = Cursor;
}

void CS::GL::FOpenGLWindow::SwapBuffers () noexcept
{
    glfwSwapBuffers( WindowState->WindowHandle );
}

CS::GL::FOpenGLContext::FOpenGLContext ( const FOpenGLWindow &Window )
{
    glfwMakeContextCurrent( Window.GetNativeHandle() );
    glfwSwapInterval( 1 );
    Api.Load();
}

CS::Bool CS::GL::FOpenGLWindow::WasKeyPressed ( const Int32 Key ) const noexcept
{
    return std::ranges::contains( WindowState->PressedKeys, Key );
}

CS::GL::FFrameBufferSize CS::GL::FOpenGLWindow::GetFramebufferSize () const noexcept
{
    FFrameBufferSize Size;

    glfwGetFramebufferSize( WindowState->WindowHandle, &Size.Width, &Size.Height );
    return Size;
}

glm::fvec2 CS::GL::FOpenGLWindow::GetDragDelta () const noexcept
{
    return WindowState->DragDelta;
}

CS::Float32 CS::GL::FOpenGLWindow::GetScrollDelta () const noexcept
{
    return WindowState->ScrollDelta;
}
