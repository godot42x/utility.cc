/**
 * @ Author: godot42
 * @ Create Time: 2024-08-02 10:47:59
 * @ Modified by: @godot42
 * @ Modified time: 2025-03-05 17:10:19
 * @ Description:
 */


#pragma once



#include <cstddef>
#include <filesystem>
#include <optional>


#include "plat.h"

namespace ut
{

namespace file
{

/**
 * Reads the entire contents of a file into a char string
 * @param filepath The path to the file to be read.
 * @return An optional vector of bytes containing the contents of the file.
 *         If the file cannot be opened or read, the optional is empty.
 * @throws None
 */
UTILITY_CC_API std::optional<std::string> read_all(const std::filesystem::path &filepath);
// extern UTILITY_CC_API void                       read_all(const std::filesystem::path &filepath, std::optional<std::string> &ret);

UTILITY_CC_API bool is_image(const std::filesystem::path &filepath);


UTILITY_CC_API std::optional<size_t> get_content_hash(const std::filesystem::path &filepath);
UTILITY_CC_API std::optional<size_t> get_hash(const std::string &text);
}; // namespace file
} // namespace ut
