/// @file Check.hpp
/// @brief Defines Check helper

#pragma once

#include <Lib/CoreTypes.hpp>
#include <Lib/Maths.hpp>

#include <print>
#include <string_view>

namespace CS
{

Bool Check ( std::string_view Statement, const auto &Lhs, const auto &Rhs ) noexcept
{
    const CS::Bool Holds = CS::IsEqual( Lhs, Rhs );

    std::println( "  [{}] {}", Holds ? "OK  " : "FAIL", Statement );
    return Holds;
}

} // namespace CS
