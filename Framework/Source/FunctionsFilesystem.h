#pragma once

// *** Функции, предназначенные для работы с файловой системой
namespace FS
{
    // *** Считать содержимое файла в переменную-буфер
    inline std::string ReadFile(std::ifstream& File)
    {
        return std::string(std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
    }

    // *** Создать директорию по указанному пути
    inline void CreateDirectories(const std::string_view Path)
    {
        std::filesystem::create_directory(String::GetPath(Path).c_str());
    }
}
