#pragma once 

#include <string>
#include <filesystem>

namespace Cpputils {

std::string ReadFileContent(const std::filesystem::path& file);

}