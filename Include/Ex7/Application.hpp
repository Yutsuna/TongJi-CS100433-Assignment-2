/// @file Application.hpp
/// @brief Main Application declaration

#pragma once

#include <Ex7/OpenGLApi.hpp>
#include <Ex7/OpenGLWindow.hpp>

namespace CS
{

    class FTimer;
    class FCamera;

/// @class FApplication
/// @brief Main Application
class FApplication final
{
public:

    FApplication ( GL::FOpenGLWindow &InWindow, GL::FOpenGLContext &InContext );

    void Run () noexcept;

private:

    void OneFrame() noexcept;
    void HandleUserInputs(Float32 DeltaTime) noexcept;
    void RenderObjects() noexcept;
    void RenderOneFrame( const GL::FFrameBufferSize &InBufferSize ) const noexcept;

    GL::FOpenGLWindow &Window;
    GL::FOpenGLContext &Context;
    const GL::FOpenGLApi &Api;

    FTimer Timer;
    FCamera Camera;
};

} // namespace CS
