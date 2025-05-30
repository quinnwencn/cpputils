#include "cpputils/fileops.h"

#include <fstream>

namespace Cpputils {

std::string ReadFile(const std::filesystem::path& file) {
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

void WriteFile(const std::filesystem::path& file, std::string_view content) {
    std::ofstream ofs(file, std::ios::out | std::ios::trunc);
    if (!ofs.good()) {
        return;
    }

    ofs << content;
    ofs.close();
}

TemporaryFile::TemporaryFile(std::string_view hint) {
    TemporaryFilePath_ = std::filesystem::temp_directory_path() / hint;
}

TemporaryFile::~TemporaryFile() {
    if (std::filesystem::exists(TemporaryFilePath_)) {
        std::filesystem::remove(TemporaryFilePath_);
    }
}

void TemporaryFile::Append(std::string_view content) {
    std::ofstream ofs(TemporaryFilePath_, std::ios::out | std::ios::app);
    if (!ofs.good()) {
        return;
    }

    ofs << content;
    ofs.close();
}

void TemporaryFile::Append(const std::vector<uint8_t>& content) {
    std::ofstream ofs(TemporaryFilePath_, std::ios::out | std::ios::app | std::ios::binary);
    if (!ofs.good()) {
        return;
    }

    ofs.write(reinterpret_cast<const char*>(content.data()), content.size());
    ofs.close();
}

void TemporaryFile::Write(std::string_view content) {
    std::ofstream ofs(TemporaryFilePath_, std::ios::out | std::ios::trunc);
    if (!ofs.good()) {
        return;
    }

    ofs << content;
    ofs.close();
}

void TemporaryFile::Write(const std::vector<uint8_t>& content) {
    std::ofstream ofs(TemporaryFilePath_, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!ofs.good()) {
        return;
    }

    ofs.write(reinterpret_cast<const char*>(content.data()), content.size());
    ofs.close();
}

} // namespace Cpputils
