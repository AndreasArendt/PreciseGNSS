#pragma once

#include "gnss_rtk/rinex/RinexFile.hpp"

#include <filesystem>
#include <string>

class RinexParser
{
public:
	RinexFile Parse(const std::filesystem::path& path) const;
};
