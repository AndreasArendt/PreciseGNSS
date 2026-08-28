#pragma once

#include <chrono>
#include <cstdint>

namespace navigation
{
    inline constexpr auto gpsEpoch = std::chrono::sys_days{std::chrono::year{1980} / std::chrono::January / 6};

    using Seconds = std::chrono::duration<double>;
    using Milliseconds = std::chrono::duration<double, std::milli>;
    using Microseconds = std::chrono::duration<double, std::micro>;

    // Common internal coordinate after an input time scale has been normalized.
    // This deliberately does not claim POSIX or UTC semantics.
    struct GnssClock
    {
        using rep = std::int64_t;
        using period = std::nano;
        using duration = std::chrono::nanoseconds;
        using time_point = std::chrono::time_point<GnssClock, duration>;
        static constexpr bool is_steady = false;
    };

    struct GpsClock
    {
        using rep = std::int64_t;
        using period = std::nano;
        using duration = std::chrono::nanoseconds;
        using time_point = std::chrono::time_point<GpsClock, duration>;
        static constexpr bool is_steady = false;
    };

    struct GalileoClock
    {
        using rep = std::int64_t;
        using period = std::nano;
        using duration = std::chrono::nanoseconds;
        using time_point = std::chrono::time_point<GalileoClock, duration>;
        static constexpr bool is_steady = false;
    };

    using GnssTime = GnssClock::time_point;
    using GpsTime = GpsClock::time_point;
    using GalileoTime = GalileoClock::time_point;

    // conversions

    inline GnssTime toeEpoch(std::chrono::weeks week, Seconds toe, GnssTime reference)
    {
        constexpr auto oneWeek = std::chrono::weeks{1};
        constexpr auto halfWeek = oneWeek / 2;

        auto result = GnssTime{std::chrono::round<GnssClock::duration>(
            gpsEpoch.time_since_epoch() + week + toe)};

        const auto offset = result - reference;
        if (offset < -halfWeek)
            result += oneWeek;
        else if (offset > halfWeek)
            result -= oneWeek;

        return result;
    }
}
