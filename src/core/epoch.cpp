#include "core/epoch.hpp"

Epoch::Epoch(int year, int month, int day, int hour, int minute, double second)
{
    const std::chrono::year_month_day date{
        std::chrono::year{year},
        std::chrono::month{static_cast<unsigned>(month)},
        std::chrono::day{static_cast<unsigned>(day)}};

    if (!date.ok())
        throw std::invalid_argument{"Invalid date or time"};

    const auto timeSinceUnixEpoch =
        std::chrono::sys_days{date}.time_since_epoch() +
        std::chrono::hours{hour} +
        std::chrono::minutes{minute} +
        navigation::Seconds{second};

    this->_time = navigation::GnssTime{
        std::chrono::round<navigation::GnssClock::duration>(timeSinceUnixEpoch)};
}
