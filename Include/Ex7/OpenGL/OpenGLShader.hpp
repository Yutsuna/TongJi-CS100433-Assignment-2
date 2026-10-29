/// @file OpenGLShader.hpp
/// @brief OpenGL shader program loading and uniform management

#pragma once

#include <Ex7/OpenGL/OpenGLApi.hpp>
#include <Lib/CoreTypes.hpp>

#include <concepts>
#include <filesystem>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace CS
{

struct FStringHash final
{
    using is_transparent = void;

    [[nodiscard]] USize operator()( const std::string_view Value ) const noexcept
    {
        return std::hash<std::string_view>{}( Value );
    }
};

namespace GL
{

    class FOpenGLShader final
    {
    public:

        FOpenGLShader () = default;
        FOpenGLShader ( FOpenGLShader &&Other ) noexcept;
        FOpenGLShader &operator=( FOpenGLShader &&Other ) noexcept;
        FOpenGLShader ( const FOpenGLShader & )           = delete;
        FOpenGLShader &operator=( const FOpenGLShader & ) = delete;
        ~FOpenGLShader () noexcept;

        [[nodiscard]] static FOpenGLShader Make ( const std::filesystem::path &VertexPath,
                                                  const std::filesystem::path &FragmentPath );

        void Use () const noexcept;

        template <class Type> void SetUniform ( const std::string_view Name, const Type &InValue ) const
        {
            const Int32 Location = FindUniformLocation( Name );
            if ( Location < 0 )
            {
                return;
            }

            if constexpr ( std::same_as<Type, Float32> )
            {
                Api.Uniform1Float( Location, InValue );
            }
            else if constexpr ( std::same_as<Type, glm::fvec3> )
            {
                Api.Uniform3Float( Location, InValue.x, InValue.y, InValue.z );
            }
            else if constexpr ( std::same_as<Type, glm::fmat4> )
            {
                Api.UniformMatrix4Float( Location, 1, GL_FALSE, glm::value_ptr( InValue ) );
            }
            else
            {
                static_assert( std::same_as<Type, Float32>, "Unsupported shader uniform type." );
            }
        }

    private:

        [[nodiscard]] Int32 FindUniformLocation ( std::string_view Name ) const noexcept;
        void Release () noexcept;

        UInt32 ProgramId = 0;
        mutable std::unordered_map<std::string, Int32, FStringHash, std::equal_to<>> UniformLocations;
    };

} // namespace GL

} // namespace CS
