/// @file Application.cpp
/// @brief Main Application implementation

#include <Ex7/Application.hpp>

CS::FApplication::FApplication ( GL::FOpenGLWindow &InWindow,
                                 GL::FOpenGLContext &InContext,
                                 const std::filesystem::path &ShaderDirectory )
    : Window( InWindow ), Context( InContext ), Api( Context.GetApi() )
{
    ///
}
