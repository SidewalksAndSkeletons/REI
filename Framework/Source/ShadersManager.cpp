#include "StdAfx.h"

CShadersManager::~CShadersManager()
{
    // Освобождаем память, занятую шейдерами
    if (Shaders.size())
    {
        for (auto& It : Shaders)
        {
            glDeleteProgram(It.second);
        }

        Shaders.clear();
    }
}

bool CShadersManager::Init()
{
    // Создаём стандартный шейдер, подходящий под большинство
    // поверхностей
    if (!CreateShader("Default"))
        return false;

    // Создаём специализированный шейдер для текста
    if (!CreateShader("Text"))
        return false;

    return true;
}

bool CShadersManager::CreateShader(const char* Name)
{
    // Пробуем создать новую шейдерную программу
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

    // Link the shader program
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

    // Помещаем собранную шейдерную программу в контейнер
    Shaders.emplace(Name, CurrentProgram);
    return true;
}

void CShadersManager::Use(const char* Name)
{
    if (Shaders.empty())
        return;

    if (Shaders.find(Name) == Shaders.end())
        return;

    glUseProgram(Shaders[Name]);
}

std::string CShadersManager::ReadSource(const std::string_view Path)
{
    // Подготавливаем новый путь
    std::string NewPath = "../Shaders/";
    String::Unite(NewPath, Path);

    // Пробуем открыть файл
    std::ifstream File(NewPath.data());

    if (!File.is_open())
    {
        DEBUG_ERROR("Shader file couldn't be opened: ", NewPath.data());
        return std::string();
    }

    // Получаем содержимое файла
    std::string Buffer = System::FS::ReadFile(File);

    // Закрываем открытый раннее файл
    File.close();

    return std::move(Buffer);
}

GLuint CShadersManager::Create(const std::string_view Path, GLenum Type)
{
    // Выделяем память под шейдер
    GLuint Shader = glCreateShader(Type);
    if (!Shader)
    {
        DEBUG_ERROR("Failed to create shader program: Shader == 0!");
        return false;
    }

    // Компилируем
    return Compile(Shader, Path) ? Shader : 0;
}

GLuint CShadersManager::Get(const char* Name)
{
    if (!Name)
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

bool CShadersManager::Compile(const GLuint& ShaderID, const std::string_view Path)
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

    // А после возвращаем статус компиляции
    return GetCompileStatus(ShaderID);
}

bool CShadersManager::Link(const GLuint& ShaderID)
{
    glLinkProgram(ShaderID);

    // Возвращаем итог
    return GetLinkStatus(ShaderID);
}

bool CShadersManager::GetCompileStatus(const GLuint& ShaderID)
{
    GLint Status = 0;

    // Получаем статус операции
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Status);

    // Всё прошло хорошо
    if (Status)
        return true;

    // Получаем размер для сформированного лога
    GLsizei Size = 0;
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &Size);

    if (Size > 1)
    {
        // Создаём символьный массив для последующего вывода
        GLchar* Log = new GLchar[static_cast<size_t>(Size) + 1];

        // Получаем описание проблемы
        glGetShaderInfoLog(ShaderID, Size, &Size, Log);

        // Выводим сообщение в лог
        DEBUG_ERROR("Shader compilation error: ", Log);

        // Не забываем очистить память
        delete[] Log;
    }

    return false;
}

bool CShadersManager::GetLinkStatus(const GLuint& ProgramID)
{
    GLint Status = 0;

    // Получаем статус операции
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Status);

    // Всё прошло хорошо
    if (Status)
        return true;

    // Получаем размер для сформированного лога
    GLsizei Size = 0;
    glGetShaderiv(ProgramID, GL_INFO_LOG_LENGTH, &Size);

    if (Size > 1)
    {
        // Создаём символьный массив для последующего вывода
        GLchar* Log = new GLchar[static_cast<size_t>(Size) + 1];

        // Получаем описание проблемы
        glGetProgramInfoLog(ProgramID, Size, &Size, Log);

        // Выводим сообщение в лог
        DEBUG_ERROR("Shader linking error: ", Log);

        // Не забываем очистить память
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
