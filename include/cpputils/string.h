#include <string_view>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdint>

namespace Cpputils {

std::string ByteArr2HexStr(const std::vector<uint8_t>& arr);
std::string ReadFileContent(const std::filesystem::path& file);

}