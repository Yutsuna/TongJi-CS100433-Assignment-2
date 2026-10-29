/// @file Application.cpp
/// @brief Main application implementation

#include <Ex7/Application.hpp>

#include <Ex7/Camera.hpp>
#include <Ex7/SolarSystem.hpp>
#include <Ex7/Timer.hpp>

#include <chrono>
#include <thread>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace
{

constexpr CS::Float32 DragFactor = 0.005F;
constexpr CS::Float32 TimeFactor = 1.25F;

} // namespace

CS::FApplication::FApplication ( GL::FOpenGLWindow &InWindow, GL::FOpenGLContext &InContext )
    : Window( InWindow ), Context( InContext )
{
}

void CS::FApplication::Run () noexcept
{
    while ( not Window.ShouldClose() )
    {
        OneFrame();
    }
}

void CS::FApplication::OneFrame () noexcept
{
    const Float32 DeltaTime = Timer.Tick();

    Window.PollEvents();
    HandleUserInputs( DeltaTime );
    SolarSystem.Update( DeltaTime * TimeFactor );
    RenderObjects();
}

void CS::FApplication::HandleUserInputs ( const Float32 DeltaTime ) noexcept
{
    if ( Window.WasKeyPressed( GLFW_KEY_ESCAPE ) )
    {
        Window.RequestClose();
    }

    const glm::fvec2 DragDelta = Window.GetDragDelta();
    Camera.Rotate( -DragDelta.x * DragFactor, DragDelta.y * DragFactor );
    Camera.Zoom( Window.GetScrollDelta() );
    Camera.AutoRotate( DeltaTime );
}

void CS::FApplication::RenderObjects () noexcept
{
    const GL::FFrameBufferSize BufferSize = Window.GetFramebufferSize();
    if ( BufferSize.Width > 0 and BufferSize.Height > 0 )
    {
        RenderOneFrame( BufferSize );
        Window.SwapBuffers();
    }
    else
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 16 ) );
    }
}

void CS::FApplication::RenderOneFrame ( const GL::FFrameBufferSize &InBufferSize ) const noexcept
{
    const Float32 AspectRatio    = static_cast<Float32>( InBufferSize.Width ) / static_cast<Float32>( InBufferSize.Height );
    const FRenderView RenderView = { .View       = Camera.GetViewMatrix(),
                                     .Projection = Camera.GetProjectionMatrix( AspectRatio ),
                                     .Camera     = Camera.GetPosition(),
                                     .Viewport   = { InBufferSize.Width, InBufferSize.Height },
                                     .TotalTime  = Timer.GetTotalTime() };

    SolarSystem.Render( RenderView );
}
