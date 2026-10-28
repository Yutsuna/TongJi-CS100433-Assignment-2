/// @file Maths.hpp
/// @brief Maths definitions for GLM

#pragma once

#include <Lib/CoreTypes.hpp>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vector_relational.hpp>

namespace CS
{

constexpr Float64 EPSILON = 1e-9;

static inline glm::dmat4 Translation ( const glm::dvec3 &Offset )
{
    return glm::translate( glm::dmat4( 1.0 ), Offset );
}

static inline glm::dmat4 Rotation ( Float64 Angle, const glm::dvec3 &Axis )
{
    return glm::rotate( glm::dmat4( 1.0 ), Angle, glm::normalize( Axis ) );
}

static inline glm::dmat4 Scaling ( const glm::dvec3 &Factors )
{
    return glm::scale( glm::dmat4( 1.0 ), Factors );
}

template <Int32 Length>
static inline Bool IsEqual ( const glm::vec<Length, Float64> &A, const glm::vec<Length, Float64> &B ) noexcept
{
    return glm::all( glm::lessThanEqual( glm::abs( A - B ), glm::vec<Length, Float64>( EPSILON ) ) );
}

static inline Bool IsEqual ( const glm::dmat4 &A, const glm::dmat4 &B ) noexcept
{
    for ( Int32 Column = 0; Column < 4; ++Column )
    {
        if ( not IsEqual( A[Column], B[Column] ) )
        {
            return false;
        }
    }
    return true;
}

} // namespace CS
