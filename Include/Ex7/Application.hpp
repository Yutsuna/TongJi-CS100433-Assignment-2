/// @file Application.hpp
/// @brief Main Application declaration

#pragma once

#include "Lib/CoreTypes.hpp"
#include <Ex7/OpenGLApi.hpp>
#include <Ex7/OpenGLWindow.hpp>

#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <filesystem>

namespace CS
{

/// @class FApplication
/// @brief Main Application
class FApplication final
{
public:

    FApplication ( GL::FOpenGLWindow &InWindow, GL::FOpenGLContext &InContext, const std::filesystem::path &ShaderDirectory );

    void Run () noexcept;

private:

    GL::FOpenGLWindow &Window;
    GL::FOpenGLContext &Context;
    const GL::FOpenGLApi &Api;
};

} // namespace CS
