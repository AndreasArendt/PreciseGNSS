#include "gnss_rtk/core/epoch.hpp"

#include <cmath>
#include <ctime>

Epoch::Epoch()
{
    this->_year = 0;
    this->_month = 0;
    this->_day = 0;
    this->_hour = 0;
    this->_minute = 0;
    this->_second = 0.0;

    this->_EpochTime = std::chrono::system_clock::time_point::min();
}

Epoch::Epoch(int year, int month, int day, int hour, int minute, double second)    
{
    const auto wholeSeconds = static_cast<std::int64_t>(second);
    const auto date = std::chrono::sys_days{
        std::chrono::year{year} / static_cast<unsigned>(month) / static_cast<unsigned>(day)};
    const auto timeOfDay = std::chrono::hours{hour} + std::chrono::minutes{minute} +
                           std::chrono::seconds{wholeSeconds};
    const auto fractionalSeconds = std::chrono::duration<double>{second - static_cast<double>(wholeSeconds)};
    this->_EpochTime = date + timeOfDay +
                       std::chrono::duration_cast<std::chrono::system_clock::duration>(fractionalSeconds);

    this->_year = year;
    this->_month = month;
    this->_day = day;
    this->_hour = hour;
    this->_minute = minute;
    this->_second = second;    
}

Epoch::Epoch(double posixEpochTime__s) 
{
    // Split seconds into integer and fractional parts
    auto seconds_since_epoch = std::chrono::seconds(static_cast<int64_t>(posixEpochTime__s));
    auto fractional_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double>(posixEpochTime__s - seconds_since_epoch.count())
    );

    // Use system_clock to create a time_point
    auto epoch_time_point = std::chrono::system_clock::time_point(seconds_since_epoch);

    auto dp = floor<std::chrono::days>(epoch_time_point);
    std::chrono::year_month_day ymd{ dp };
    std::chrono::hh_mm_ss time{ floor<std::chrono::milliseconds>(epoch_time_point - dp) };

    this->_year = static_cast<int>(ymd.year());
    this->_month = static_cast<unsigned>(ymd.month());
    this->_day = static_cast<unsigned>(ymd.day());
    this->_hour = time.hours().count();
    this->_minute = time.minutes().count();
    this->_second = time.seconds().count() + fractional_seconds.count() * 1.0e-3;    
    this->_EpochTime = epoch_time_point + fractional_seconds; // Add the milliseconds duration
}

bool Epoch::operator==(const Epoch& other) const
{
    return std::abs(this->PosixEpochTime__s() - other.PosixEpochTime__s()) < 1e-6;
}

bool Epoch::operator!=(const Epoch& other) const
{
    return std::abs(this->PosixEpochTime__s() - other.PosixEpochTime__s()) >= 1e-6;
}

Epoch::~Epoch()
{

}
