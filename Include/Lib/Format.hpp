/// @file Format.hpp
/// @brief Format GLM

#pragma once

#include <Lib/CoreTypes.hpp>
#include <glm/glm.hpp>

#include <format>

namespace CS
{

static inline std::string Format ( const glm::dvec4 &InVector ) noexcept
{
    return std::format( "[{:9.4f} {:9.4f} {:9.4f} {:9.4f}]^T", InVector.x, InVector.y, InVector.z, InVector.w );
}

static inline std::string Format ( const glm::dmat4 &InMatrix ) noexcept
{
    std::string Buffer;
    for ( Int32 Row = 0; Row < 4; ++Row )
    {
        Buffer += std::format( "    [{:9.4f} {:9.4f} {:9.4f} {:9.4f}]\n", InMatrix[0][Row], InMatrix[1][Row], InMatrix[2][Row],
                               InMatrix[3][Row] );
    }
    Buffer.pop_back();
    return Buffer;
}

} // namespace CS
