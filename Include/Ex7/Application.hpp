/// @file Application.hpp
/// @brief Main Application declaration

#pragma once

#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Ex7/OpenGL/OpenGLWindow.hpp>

#include <Ex7/Camera.hpp>
#include <Ex7/SolarSystem.hpp>
#include <Ex7/Timer.hpp>

namespace CS
{

/// @class FApplication
/// @brief Main Application
class FApplication final
{
public:

    FApplication ( GL::FOpenGLWindow &InWindow, GL::FOpenGLContext &InContext );

    void Run () noexcept;

private:

    void OneFrame () noexcept;
    void HandleUserInputs ( Float32 DeltaTime ) noexcept;
    void RenderObjects () noexcept;
    void RenderOneFrame ( const GL::FFrameBufferSize &InBufferSize ) const noexcept;

    GL::FOpenGLWindow &Window;
    GL::FOpenGLContext &Context;

    FTimer Timer;
    FCamera Camera;
    FSolarSystem SolarSystem;
};

} // namespace CS
