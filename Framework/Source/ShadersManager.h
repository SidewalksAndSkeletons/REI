#pragma once

// *** Класс, предназначенный для работы с шейдерами
class CShadersManager final
{
private:
    // Контейнер, содержащий в себе скомпилированные шейдеры
    std::unordered_map<const char*, GLuint> Shaders;

public:
    CShadersManager() = default;
    ~CShadersManager();

    [[nodiscard]] bool Init();

public:
    // *** Использовать шейдер с указанным именем
    void Use(const char* Name);

    // *** Получить шейдер по имени
    GLuint Get(const char* Name);

private:
    // *** Создать новый шейдер
    bool CreateShader(const char* Name);

private:
    // *** Считать исходный код шейдера из файла
    std::string ReadSource(const std::string_view Path);

    // *** Создать новый шейдер
    GLuint Create(const std::string_view Path, GLenum Type);

    // *** Скомпилировать исходный код шейдера по указанному пути
    bool Compile(const GLuint& ShaderID, const std::string_view Path);

    // *** Слинковать шейдер
    bool Link(const GLuint& ShaderID);

private:
    // *** Проверить статус скомпиленности шейдера
    bool GetCompileStatus(const GLuint& ShaderID);

    // *** Проверить статус линковки шейдера
    bool GetLinkStatus(const GLuint& ProgramID);

private:
    // *** Получить uniform по его имени
    GLint GetUniformLocation(const GLuint& ShaderID, const char* Name);

public:
    // *** Установить значение matrix-uniform по его имени
    void SetMatrix4(const GLuint& ShaderID, const char* Name, const glm::mat4& Matrix)
    {
        GLint Location = GetUniformLocation(ShaderID, Name);
        if (Location == -1)
            return;

        glUniformMatrix4fv(Location, 1, false, glm::value_ptr(Matrix));
    }
};
