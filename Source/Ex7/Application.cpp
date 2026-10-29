/// @file Application.cpp
/// @brief Main Application implementation

#include <Ex7/Application.hpp>

#include <Ex7/Camera.hpp>
#include <Ex7/OpenGLWindow.hpp>
#include <Ex7/Timer.hpp>

#include <thread>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace
{

constexpr CS::Float32 GDragFactor = 0.005F;
constexpr CS::Float32 GTimeFactor = 1.25F;

} // namespace

///
/// Public
///

CS::FApplication::FApplication ( GL::FOpenGLWindow &InWindow, GL::FOpenGLContext &InContext )
    : Window( InWindow ), Context( InContext ), Api( Context.GetApi() )
{
    ///
}

void CS::FApplication::Run () noexcept
{
    while ( not Window.ShouldClose() )
    {
        OneFrame();
    }
}

///
/// Private
///

void CS::FApplication::OneFrame () noexcept
{
    const Float32 DeltaTime = Timer.Tick();

    Window.PollEvents();
    HandleUserInputs( DeltaTime );
    RenderObjects( DeltaTime );
}

void CS::FApplication::HandleUserInputs ( const Float32 DeltaTime ) noexcept
{
    if ( Window.WasKeyPressed( GLFW_KEY_ESCAPE ) )
    {
        Window.RequestClose();
    }

    const glm::fvec2 DragDelta = Window.GetDragDelta();
    Camera.Rotate( -DragDelta.x * GDragFactor, DragDelta.y * GDragFactor );
    Camera.Zoom( Window.GetScrollDelta() );
    Camera.AutoRotate( DeltaTime );
}

void CS::FApplication::RenderObjects () noexcept
{
    const GL::FFrameBufferSize BufferSize = Window.GetFramebufferSize();
    if ( BufferSize.Width > 0 and BufferSize.Height > 0 )
    {
        RenderOneFrame( BufferSize );
    }
    else
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
    }
}

void CS::FApplication::RenderOneFrame ( const GL::FFrameBufferSize &InBufferSize ) const noexcept
{
    const FRenderView RenderView = { .View       = Camera.GetViewMatrix(),
                                     .Projection = Camera.GetProjectionMatrix( static_cast<Float32>( InBufferSize.Width ) ),
                                     .Viewport   = InBufferSize,
                                     .TotalTime  = Timer.GetTotalTime() };

    SolarSystem.Render( RenderView );
}
