#include "cpputils/string.h"

#include <fstream>

namespace {

std::string_view HEX_CHARS {"0123456789abcdef"};

}

namespace Cpputils {

std::string ByteArr2HexStr(const std::vector<uint8_t>& arr) {
    std::string hexStr;
    hexStr.reserve(arr.size() * 2);

    for (auto byte : arr) {
        hexStr.push_back(HEX_CHARS[byte >> 4]);
        hexStr.push_back(HEX_CHARS[byte & 0x0f]);
    }
    
    return hexStr;
}

}
