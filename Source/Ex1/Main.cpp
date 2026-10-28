/// @file Main.cpp
/// @brief Exercise 1

#include <Lib/Check.hpp>
#include <Lib/CoreTypes.hpp>
#include <Lib/Maths.hpp>
#include <Lib/Reflect.hpp>

namespace
{

struct FTransfromation final
{
    /// Data
    [[= CS::FReflect{ "p" }]] glm::dvec4 P;
    [[= CS::FReflect{ "T_1" }]] glm::dmat4 T1;
    [[= CS::FReflect{ "T_2^local" }]] glm::dmat4 T2Local;

    /// Derived
    [[= CS::FReflect{ "T_1^(-1)" }]] glm::dmat4 T1Inverse = glm::inverse( T1 );
    /// T_2^global = T_1 · T_2^local · T_1^(-1)
    [[= CS::FReflect{ "T_2^global" }]] glm::dmat4 T2Global = T1 * T2Local * T1Inverse;
    /// p'         = T_2^global · (T_1 · p)
    [[= CS::FReflect{ "p'" }]] glm::dvec4 PPrime = T2Global * ( T1 * P );

    [[nodiscard]] CS::Bool Verify () const noexcept
    {
        CS::Bool Holds = true;

        /// p' = (T_1 · T_2^local · T_1^(-1)) · T_1 · p
        Holds &= CS::Check( "PPrime == (T1 * T2Local * T1Inverse) * T1 * P", PPrime, ( T1 * T2Local * T1Inverse ) * T1 * P );

        /// p' = T_1 · T_2^local · p
        Holds &= CS::Check( "PPrime == T1 * T2Local * P", PPrime, T1 * T2Local * P );

        /// T_2^global · T_1 = T_1 · T_2^local
        Holds &= CS::Check( "T2Global * T1 == T1 * T2Local", T2Global * T1, T1 * T2Local );

        /// T_2^global · T_1 = T_1 · (T_1^(-1) · T_2^global · T_1)
        Holds &= CS::Check( "T2Global * T1 == T1 * (T1Inverse * T2Global * T1)", T2Global * T1, T1 * ( T1Inverse * T2Global * T1 ) );

        /// T_1^(-1) · T_2^global · T_1 = T_2^local
        Holds &= CS::Check( "T1Inverse * T2Global * T1 == T2Local", T1Inverse * T2Global * T1, T2Local );

        return Holds;
    }
};

CS::Bool CheckAgainstGlm ( const glm::dmat4 &T1 ) noexcept
{
    const glm::dvec3 Offset( 1.5, -2.0, 0.5 );
    const glm::dvec3 Axis( 0.3, 1.0, -0.4 );
    const CS::Float64 Angle( 0.9 );
    const glm::dvec3 Factors( 1.5, 0.75, 2.0 );
    CS::Bool Holds = true;

    const auto GlobalOf = [&] ( const glm::dmat4 &T2Local ) noexcept
    {
        return FTransfromation{
            .P = glm::dvec4( 0.0, 0.0, 0.0, 1.0 ),
            .T1 = T1,
            .T2Local = T2Local
        }.T2Global * T1;
    };

    Holds &= CS::Check( "glm::translate(T1, Offset) == T2Global * T1",
                        glm::translate( T1, Offset ), GlobalOf( CS::Translation( Offset ) ) );

    Holds &= CS::Check( "glm::rotate(T1, Angle, Axis) == T2Global * T1",
                        glm::rotate( T1, Angle, glm::normalize( Axis ) ), GlobalOf( CS::Rotation( Angle, Axis ) ) );

    Holds &= CS::Check( "glm::scale(T1, Factors) == T2Global * T1",
                        glm::scale( T1, Factors ), GlobalOf( CS::Scaling( Factors ) ) );
    return Holds;
}

} // namespace

CS::Int32 main () noexcept
{
    const FTransfromation Example = {
        .P  = glm::dvec4( 1.0, 2.0, 3.0, 1.0 ),
        .T1 = CS::Translation( { 4.0, -1.0, 2.0 } ) * CS::Rotation( 0.7, { 0.0, 1.0, 0.0 } ) * CS::Scaling( { 2.0, 1.0, 0.5 } ),
        .T2Local = CS::Translation( { 1.0, 0.0, 0.0 } ) * CS::Rotation( 1.1, { 1.0, 1.0, 0.0 } ),
    };

    CS::Dump( Example );
    CS::Bool ReturnValue = Example.Verify();

    return static_cast<CS::Int32>( ReturnValue &= CheckAgainstGlm( Example.T1 ) );
}
