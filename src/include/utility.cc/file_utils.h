/**
 * @ Author: godot42
 * @ Create Time: 2024-08-02 10:47:59
 * @ Modified by: @godot42
 * @ Modified time: 2025-03-07 16:30:34
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
extern UTILITY_CC_API std::optional<std::string> read_all(const std::filesystem::path &filepath);
// extern UTILITY_CC_API void                       read_all(const std::filesystem::path &filepath, std::optional<std::string> &ret);

struct UTILITY_CC_API ImageInfo
{
    enum class Format
    {
        UNKNOWN,
        PNG,
        JPEG,
        BMP,
        GIF,
        WEBP,
        TIFF
    };

    Format                format = Format::UNKNOWN;
    std::string           format_name;
    std::filesystem::path file_path;
    bool                  bValid = false;

    bool is_png() const { return bValid && format == Format::PNG; }
    bool is_jpeg() const { return bValid && format == Format::JPEG; }
    bool is_bmp() const { return bValid && format == Format::BMP; }
    bool is_gif() const { return bValid && format == Format::GIF; }
    bool is_webp() const { return bValid && format == Format::WEBP; }
    bool is_tiff() const { return bValid && format == Format::TIFF; }
    bool is_valid() const { return bValid; }

    static ImageInfo detect(const std::filesystem::path &filepath);
};

extern UTILITY_CC_API ImageInfo detect_image(const std::filesystem::path &filepath);


extern UTILITY_CC_API std::optional<size_t> get_content_hash(const std::filesystem::path &filepath);
extern UTILITY_CC_API std::optional<size_t> get_hash(const std::string &text);
}; // namespace file
} // namespace ut
