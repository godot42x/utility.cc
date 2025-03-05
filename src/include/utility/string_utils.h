/*
 * @ Author: godot42
 * @ Create Time: 2025-01-06 21:14:30
 * @ Modified by: @godot42
 * @ Modified by: @godot42
 * @ Modified time: 2025-03-05 17:09:25
 */


#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "plat.h"

namespace ut
{



namespace str
{
UTILITY_CC_API std::string replace(std::string_view source, std::string_view from, const std::string_view to);
UTILITY_CC_API std::vector<std::string> split(std::string_view source, char delimiter = ' ');
UTILITY_CC_API bool                     split(std::string_view source, char sep, std::string &left, std::string_view &right);
UTILITY_CC_API std::string_view left(std::string_view source, std::string_view delimiter);
UTILITY_CC_API std::string trim(std::string_view source);
} // namespace str


} // namespace ut