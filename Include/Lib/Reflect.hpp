/// @file Reflect.hpp
/// @brief C++26 Reflection

#pragma once

#include <Lib/CoreTypes.hpp>
#include <Lib/Format.hpp>

#include <meta>
#include <print>
#include <string_view>

namespace CS
{

struct FReflect
{
    const AnsiChar *Text;

    consteval FReflect ( std::string_view InValue ) noexcept : Text( std::define_static_string( InValue ) )
    {
        ///
    }
};

template <typename Type> consteval auto MembersOf ()
{
    return std::define_static_array( std::meta::nonstatic_data_members_of( ^^Type, std::meta::access_context::unchecked() ) );
}

template <typename Type> void Dump ( const Type &Value )
{
    template for ( constexpr auto Member : MembersOf<Type>() )
    {
        constexpr auto Math = std::meta::extract<FReflect>( std::meta::annotations_of_with_type( Member, ^^FReflect )[0] );

        std::println( "{} ({})", std::meta::identifier_of( Member ), Math.Text );
        std::println( "{}", Format( Value.[:Member:] ) );
    }
}

} // namespace CS
