/// @file Timer.hpp
/// @brief Computes delta time

#pragma once

#include <Lib/CoreTypes.hpp>

#include <algorithm>
#include <chrono>

namespace CS
{

class FTimer
{
    using FClock = std::chrono::steady_clock;

public:

    FTimer () noexcept : StartTime( FClock::now() ), LastTickTime( StartTime )
    {
        ///
    }

    [[nodiscard]] Float32 Tick () noexcept
    {
        const FClock::time_point Now                 = FClock::now();
        const std::chrono::duration<Float32> Elapsed = Now - LastTickTime;
        LastTickTime                                 = Now;
        return std::min( Elapsed.count(), MaxDeltaTime );
    }

    [[nodiscard]] Float32 GetTotalTime () const noexcept
    {
        return std::chrono::duration<Float32>( FClock::now() - StartTime ).count();
    }

private:

    static constexpr Float32 MaxDeltaTime = 0.05F;

    FClock::time_point StartTime;
    FClock::time_point LastTickTime;
};

} // namespace CS
