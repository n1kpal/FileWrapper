#include "File.hpp"
#include <windows.h>
#include <algorithm>


std::expected<File, std::error_code> File::try_open(const std::filesystem::path& p, DWORD access, DWORD creation) noexcept {
    HANDLE handle = CreateFileW(p.c_str(), access, 0, nullptr, creation, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (handle == INVALID_HANDLE_VALUE) {
        return std::unexpected(std::error_code(static_cast<int>(GetLastError()), std::system_category()));
    } else {
        return File(handle);
    }
}

File File::open(const std::filesystem::path& p, DWORD access, DWORD creation) {
    auto file = try_open(p, access, creation);
    if (!file) {
        throw std::system_error(file.error());
    } else {
        return std::move(file.value());
    }
}

File::File(File &&other) noexcept {
    std::swap(this->h_, other.h_);
}

File& File::operator=(File &&other) noexcept {
    if (this != &other) {
        if (this->h_ != INVALID_HANDLE_VALUE) {
            CloseHandle(this->h_);
            this->h_ = INVALID_HANDLE_VALUE;
        }
        std::swap(this->h_, other.h_);
    }

    return *this;
}

void File::swap(File& other) noexcept {    
    std::swap(this->h_, other.h_);
}

bool File::is_open() const noexcept {
    return this->h_ != INVALID_HANDLE_VALUE;
}

std::size_t File::read(std::span<std::byte> buf) {
    if (buf.empty()) {
        return 0;
    }

    DWORD to_read = static_cast<DWORD>(std::min<size_t>(buf.size(), MAXDWORD));

    DWORD counter = 0;
    if (!ReadFile(this->h_, buf.data(), to_read, &counter, nullptr)) {
        throw std::system_error(static_cast<int>(GetLastError()), std::system_category());
    }

    return counter;
}

void File::write_all(std::span<const std::byte> data) {
    std::size_t offset = 0;
    while (offset < data.size()) {
        DWORD cur = 0;
        DWORD to_write = static_cast<DWORD>(std::min<size_t>(data.size() - offset, MAXDWORD));
        
        if (!WriteFile(this->h_, data.data() + offset, to_write, &cur, nullptr)) {
            throw std::system_error(static_cast<int>(GetLastError()), std::system_category());
        }

        if (cur == 0) {
            throw std::system_error(static_cast<int>(ERROR_WRITE_FAULT), std::system_category());
        }

        offset += cur;
    }
}

std::string File::read_all() const {
    std::string res = "";
    LARGE_INTEGER start{};
    SetFilePointerEx(this->h_, LARGE_INTEGER(0), &start, FILE_BEGIN);

    while (true) {
        DWORD cur = 0;
        res += std::string(MAXDWORD, ' ');
        if (!ReadFile(this->h_, &res, MAXDWORD, &cur, nullptr)) {

        }
    }
}