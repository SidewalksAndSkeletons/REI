#pragma once

namespace System
{
    namespace FS
    {
        // *** Считать содержимое файла в переменную-буфер
        inline std::string ReadFile(std::ifstream& File)
        {
            std::string Result((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
            return std::move(Result);
        }

        // *** Создать директорию по указанному пути
        inline void CreateDirectories(const std::string_view Path)
        {
            std::filesystem::create_directory(String::GetPath(Path).c_str());
        }
    }
}
