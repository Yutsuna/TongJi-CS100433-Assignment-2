/// @file OpenGLShader.hpp
/// @brief OpenGLShader loader

#pragma once

#include <Lib/CoreTypes.hpp>
#include <Ex7/OpenGLApi.hpp>

#include <filesystem>
#include <glm/fwd.hpp>
#include <string_view>
#include <unordered_map>

namespace CS
{

    /// @struct FStringHash
    /// @brief Because unordered map doesnt hash string by default
    struct FStringHash
    {

     using is_transparent = void;

     [[nodiscard]] USize operator()(const std::string_view Value) const noexcept{
         return std::hash<std::string_view>{}(Value);
     }
    };

namespace GL
{

    /// @class FOpenGLShader
    /// @brief Shader Loader
    class FOpenGLShader
    {

    public:

        FOpenGLShader () = default;
        FOpenGLShader ( FOpenGLShader &&Other ) noexcept;
        FOpenGLShader &operator=( FOpenGLShader &&Other ) noexcept;
        FOpenGLShader ( const FOpenGLShader & )           = delete;
        FOpenGLShader &operator=( const FOpenGLShader & ) = delete;
        ~FOpenGLShader ();

    public:

        static void Make(const std::filesystem::path &VertexPath, const std::filesystem::path &FragmentPath );
        void Use()noexcept;

    public:

        template <class Type> void SetUniform ( const std::string_view Name, const Type &InValue ) const
        {
            const Int32 Location = FindUniformLocation( Name );

            if constexpr ( std::same_as<Type, Float32> )
            {
                Api.Uniform1Float( Location, InValue );
            }
            else if constexpr ( std::same_as<Type, glm::vec3> )
            {
                Api.Uniform3Float( Location, InValue.X, InValue.Y, InValue.Z );
            }
            else
            {
                Api.UniformMatrix4Float( Location, 1, false, InValue.data() );
            }
        }

    private:

        [[nodiscard]] Int32 FindUniformLocation( std::string_view Name) const noexcept;
        void Release() noexcept;

    private:

        UInt32 ProgramId = 0;
        mutable std::unordered_map<std::string, Int32, FStringHash, std::equal_to<>> UniformLocations;
    };

} // namespace GL

} // namespace CS
