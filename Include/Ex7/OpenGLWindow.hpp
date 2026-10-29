/// @brief OpenGLWindow.hpp
/// @brief OpenGL GLFW Window wrapper declaration

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/fwd.hpp>
#include <memory>

struct GLFWwindow;

namespace CS
{

namespace GL
{

    struct FWindowState;

    struct FFrameBufferSize

    {
        Int32 Width  = 0;
        Int32 Height = 0;
    };

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
        [[nodiscard]] Bool WasKeyPressed ( Int32 Key ) const noexcept;
        [[nodiscard]] FFrameBufferSize GetFramebufferSize () const noexcept;
        [[nodiscard]] glm::fvec2 GetDragDelta () const noexcept;
        [[nodiscard]] Float32 GetScrollDelta () const noexcept;

    public:

        void RequestClose () const noexcept;
        void PollEvents () noexcept;

    private:

        std::unique_ptr<FWindowState> WindowState;
    };

} // namespace GL

} // namespace CS
