#include "StdAfx.h"

CShadersManager::~CShadersManager()
{
    for (auto& It : Shaders)
    {
        glDeleteProgram(It.second);
    }

    Shaders.clear();
}

bool CShadersManager::Init()
{
    if (!CreateShader(std::string("Default")))
        return false;

    if (!CreateShader(std::string("Text")))

        return false;

    return true;
}

bool CShadersManager::CreateShader(const std::string& Name)
{
    GLuint CurrentProgram = glCreateProgram();

    if (!CurrentProgram)
    {
        DEBUG_ERROR("Shader program == 0!");
        return false;
    }

    // Пробуем создать вершинный шейдер
    GLuint VertexShader = 0;
    std::string CorrectPath = Name;
    String::Unite(CorrectPath, ".vert");
    VertexShader = Create(CorrectPath, GL_VERTEX_SHADER);

    if (!VertexShader)
    {
        DEBUG_ERROR("Shader not found: ", CorrectPath);
        return false;
    }

    // Пробуем создать фрагментный шейдер
    GLuint FragmentShader = 0;
    CorrectPath = Name;
    String::Unite(CorrectPath, ".frag");
    FragmentShader = Create(CorrectPath, GL_FRAGMENT_SHADER);

    if (!FragmentShader)
    {
        DEBUG_ERROR("Shader not found: ", CorrectPath);
        return false;
    }

    // Прикрепляем шейдеры в программу
    glAttachShader(CurrentProgram, VertexShader);
    glAttachShader(CurrentProgram, FragmentShader);

    // Настройка выходного значения для фрагментного шейдера
    glBindFragDataLocation(CurrentProgram, 0, "OutputColor");

    if (!Link(CurrentProgram))
    {
        DEBUG_ERROR("Linking was failed!");
        return false;
    }

    // Указываем слой под позицию вершин
    GLint Attrib = glGetAttribLocation(CurrentProgram, "InputPosition");
    glEnableVertexAttribArray(Attrib);
    glVertexAttribPointer(Attrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    // Указываем слой для цвета вершин
    Attrib = glGetAttribLocation(CurrentProgram, "InputColor");
    glEnableVertexAttribArray(Attrib);
    glVertexAttribPointer(Attrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // Указываем слой для вершинных координат текстур
    Attrib = glGetAttribLocation(CurrentProgram, "InputTexCoord");
    glVertexAttribPointer(Attrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(Attrib);

    // Освобождаем память, занятую временными шейдерами
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    Shaders.emplace(Name, CurrentProgram);
    return true;
}

void CShadersManager::Use(const std::string& Name)
{
    if (Shaders.find(Name) == Shaders.end())
        return;

    glUseProgram(Shaders[Name]);
}

std::string CShadersManager::ReadSource(const std::string& Path)
{
    // Указываем корректный путь согласно иерархии приложения
    std::string NewPath = "Shaders/";
    String::Unite(NewPath, Path);

    // Пробуем открыть файл
    std::ifstream File(String::GetPath(NewPath).c_str());

    if (!File.is_open())
    {
        DEBUG_ERROR("Shader file couldn't be opened: ", NewPath);
        return std::string();
    }

    // Получаем содержимое файла
    std::string Buffer = FS::ReadFile(File);

    File.close();
    return Buffer;
}

GLuint CShadersManager::Create(const std::string& Path, GLenum Type)
{
    GLuint Shader = glCreateShader(Type);

    if (!Shader)
    {
        DEBUG_ERROR("Failed to create shader program: Shader == 0!");
        return false;
    }

    return Compile(Shader, Path) ? Shader : 0;
}

GLuint CShadersManager::Get(const std::string& Name)
{
    if (Name.empty())
    {
        DEBUG_WARNING("Shader's name is nullptr!");
        return 0;
    }

    if (Shaders.empty())
    {
        DEBUG_WARNING("Shaders.size() == 0!");
        return 0;
    }

    return (Shaders.find(Name) != Shaders.end()) ? Shaders[Name] : 0;
}

bool CShadersManager::Compile(const GLuint& ShaderID, const std::string& Path)
{
    // Получаем исходный код шейдера
    std::string Source = ReadSource(Path);

    if (Source.empty())
    {
        DEBUG_ERROR("Shader error: Source.size() == 0");
        return false;
    }

    // Связываем шейдер с его кодом
    glShaderSource(ShaderID, 1, String::ToCPtr(Source), nullptr);

    // Компилируем
    glCompileShader(ShaderID);
    return GetCompileStatus(ShaderID);
}

bool CShadersManager::Link(const GLuint& ShaderID)
{
    glLinkProgram(ShaderID);
    return GetLinkStatus(ShaderID);
}

bool CShadersManager::GetCompileStatus(const GLuint& ShaderID)
{
    GLint Status = 0;

    // Получаем статус операции
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Status);

    if (Status)
        return true;

    // Получаем размер для сформированного лога
    GLsizei Size = 0;
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &Size);

    if (Size > 1)
    {
        GLchar* Log = new GLchar[static_cast<size_t>(Size) + 1];

        // Получаем описание проблемы
        glGetShaderInfoLog(ShaderID, Size, &Size, Log);

        DEBUG_ERROR("Shader compilation error: ", Log);
        delete[] Log;
    }

    return false;
}

bool CShadersManager::GetLinkStatus(const GLuint& ProgramID)
{
    GLint Status = 0;

    // Получаем статус операции
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Status);

    if (Status)
        return true;

    // Получаем размер для сформированного лога
    GLsizei Size = 0;
    glGetShaderiv(ProgramID, GL_INFO_LOG_LENGTH, &Size);

    if (Size > 1)
    {
        GLchar* Log = new GLchar[static_cast<size_t>(Size) + 1];

        // Получаем описание проблемы
        glGetProgramInfoLog(ProgramID, Size, &Size, Log);

        DEBUG_ERROR("Shader linking error: ", Log);
        delete[] Log;
    }

    return false;
}

GLint CShadersManager::GetUniformLocation(const GLuint& ShaderID, const char* Name)
{
    // Шейдер не был проинициализирован
    if (!Name || !ShaderID)
        return -1;

    GLint Location = glGetUniformLocation(ShaderID, Name);

    // Существует ли такой uniform вообще?
    if (Location == -1)
    {
        DEBUG_WARNING("Location for uniform [", Name, "] not defined!");
    }

    return Location;
}

void CShadersManager::SendProjectionMatrix(const glm::mat4& Matrix)
{
    for (auto& It : Shaders)
    {
        Use(It.first);

        SetMatrix4(Shaders[It.first], "ProjectionMatrix", Matrix);
    }
}
