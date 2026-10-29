/// @file Main.cpp
/// @brief Exercise 7

#include <Lib/CoreTypes.hpp>

#include <print>

namespace
{

}

CS::Int32 main () noexcept
{
    try
    {
        ///
    }
    catch ( const std::exception &Exception )
    {
        std::println( stderr, "FATAL ERROR: {}", Exception.what() );
        return 84;
    }
    return 0;
}
