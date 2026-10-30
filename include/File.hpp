#pragma once
#include <filesystem>
#include <expected>
#include <span>
#include <windows.h>

class File {
  public:
    // Бросает std::system_error, если файл не открылся
    static File open(const std::filesystem::path& p, DWORD access, DWORD creation);

    // Ошибка возвращается значением, без исключений
    static std::expected<File, std::error_code> try_open(const std::filesystem::path& p, DWORD access, DWORD creation) noexcept;

    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&&) noexcept;
    File& operator=(File&&) noexcept;

    void swap(File& other) noexcept;
    bool is_open() const noexcept;

    std::size_t read(std::span<std::byte> buf);
    void write_all(std::span<const std::byte> data);
    std::string read_all() const;
    void reopen(const std::filesystem::path& p, DWORD access);
    void close();   // Явное закрытие с проверкой ошибки

  private:
    HANDLE h_ = INVALID_HANDLE_VALUE;
    File(HANDLE) noexcept;
};
