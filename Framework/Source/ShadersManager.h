#pragma once

// *** Класс, предназначенный для работы с шейдерами
class CShadersManager final : public ISingleton<CShadersManager>
{
private:
    // Контейнер, содержащий в себе скомпилированные шейдеры
    std::unordered_map<std::string, GLuint> Shaders;

public:
    CShadersManager() = default;
    CShadersManager(const CShadersManager&) = delete;
    CShadersManager& operator=(const CShadersManager&) = delete;
    ~CShadersManager();

    [[nodiscard]] bool Init();

public:
    // *** Использовать шейдер с указанным именем
    void Use(const std::string& Name);

    // *** Получить шейдер по имени
    GLuint Get(const std::string& Name);

private:
    // *** Создать новый шейдер
    bool CreateShader(const std::string& Name);

private:
    // *** Считать исходный код шейдера из файла
    std::string ReadSource(const std::string& Path);

    // *** Создать новый шейдер
    GLuint Create(const std::string& Path, GLenum Type);

    // *** Скомпилировать исходный код шейдера по указанному пути
    bool Compile(GLuint& ShaderID, const std::string& Path);

    // *** Слинковать шейдер
    bool Link(GLuint& ShaderID);

private:
    // *** Проверить статус скомпиленности шейдера
    bool GetCompileStatus(GLuint ShaderID);

    // *** Проверить статус линковки шейдера
    bool GetLinkStatus(GLuint ProgramID);

private:
    // *** Получить uniform по его имени
    GLint GetUniformLocation(GLuint& ShaderID, const char* Name);

public:
    // *** Отправить проекционную матрицу в шейдеры
    void SendProjectionMatrix(const glm::mat4& Matrix);

public:
    // *** Установить значение matrix-uniform по его имени
    void SetMatrix4(GLuint& ShaderID, const char* Name, const glm::mat4& Matrix)
    {
        GLint Location = GetUniformLocation(ShaderID, Name);
        if (Location == -1)
            return;

        glUniformMatrix4fv(Location, 1, false, glm::value_ptr(Matrix));
    }
};
