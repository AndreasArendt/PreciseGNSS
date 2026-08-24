#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <ranges>
#include <iostream>

namespace util
{
	class astring
	{
	public:
		// Checks if a string is empty or contains only whitespace characters
		static bool emptyOrWhiteSpace(const std::string& str)
		{
			return std::ranges::all_of(str, [](unsigned char c) { return std::isspace(c); });
		}

		// Parses a string to a double, replacing 'D' with 'E' for scientific notation compatibility
		static double parseDouble(const std::string& str)
		{
			if (emptyOrWhiteSpace(str))
				return 0.0;

			try
			{
				std::string modifiedStr = str;
				std::ranges::replace(modifiedStr, 'D', 'E'); // Replace 'D' notation
				return std::stod(modifiedStr);
			}
			catch (const std::invalid_argument& e)
			{
				std::cerr << e.what() << std::endl;
				throw std::runtime_error("Invalid argument: Unable to convert string to double.");
			}
			catch (const std::out_of_range& e)
			{
				std::cerr << e.what() << std::endl;
				throw std::runtime_error("Out of range: The double value is too large to represent.");
			}
		}

		// Parses a string to an integer
		static int parseInt(const std::string& str)
		{
			if (emptyOrWhiteSpace(str))
				return 0;

			try
			{
				return std::stoi(str);
			}
			catch (const std::invalid_argument& e)
			{
				std::cerr << e.what() << std::endl;
				throw std::runtime_error("Invalid argument: Unable to convert string to int.");
			}
			catch (const std::out_of_range& e)
			{
				std::cerr << e.what() << std::endl;
				throw std::runtime_error("Out of range: The int value is too large to represent.");
			}
		}
	};
}

inline std::string& ltrim(std::string& value, const char* characters = " \t\n\r\f\v")
{
	value.erase(0, value.find_first_not_of(characters));
	return value;
}

inline std::string& rtrim(std::string& value, const char* characters = " \t\n\r\f\v")
{
	value.erase(value.find_last_not_of(characters) + 1);
	return value;
}

inline std::string& trim(std::string& value, const char* characters = " \t\n\r\f\v")
{
	return ltrim(rtrim(value, characters), characters);
}
