#include "cpputils/fileops.h"

#include <fstream>

namespace Cpputils {

std::string ReadFileContent(const std::filesystem::path& file) {
    if (!std::filesystem::exists(file)) {
        return {};
    }

    std::ifstream ifs(file, std::ios::in);
    if (!ifs.good()) {
        return {};
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
}

}
