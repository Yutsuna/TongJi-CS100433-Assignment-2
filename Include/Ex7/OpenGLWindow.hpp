/// @brief OpenGLWindow.hpp
/// @brief OpenGL GLFW Window wrapper declaration

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/vec2.hpp>

struct GLFWwindow;

namespace CS
{

namespace GL
{

    /// @class FOpenGLWindow
    /// @brief OpenGL GLFW Window wrapper
    class FOpenGLWindow final
    {
    public:

        FOpenGLWindow ( Int32 Width, Int32 Height, const AnsiChar *Title );
        ~FOpenGLWindow () noexcept;

        FOpenGLWindow ( const FOpenGLWindow & )           = delete;
        FOpenGLWindow &operator=( const FOpenGLWindow & ) = delete;

    public:

        [[nodiscard]] GLFWwindow *GetNativeHandle () const noexcept;
        [[nodiscard]] glm::ivec2 GetFrameBufferSize () const noexcept;
        [[nodiscard]] Bool ShouldClose () const noexcept;

    public:

        void RequestClose () const noexcept;
        static void PollEvents () noexcept;

    private:

        static void FramebufferSizeCallback ( GLFWwindow *Handle, Int32 Width, Int32 Height ) noexcept;

        GLFWwindow *WindowHandle = nullptr;
        Int32 FramebufferWidth    = 0;
        Int32 FramebufferHeight   = 0;
    };

} // namespace GL

} // namespace CS
