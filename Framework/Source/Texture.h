#pragma once

// *** Класс, представляющий собой обёртку для OpenGL-текстуры
class CTexture final 
{
private:
    // Имя шейдера
    std::string Shader;

private:
    // Идентификатор текстуры
    GLuint ID;

private:
    // Скорость переключения кадров анимации
    int FrameSpeed;

    // Количество кадров в анимации
    int FramesCount;

    // Текущий кадр анимации
    int FrameCurrent;

    // Размеры кадра w * h
    SRect<int> FrameSize;

    // Оригинальные размеры текстуры
    SRect<int> Size;

    // Размеры анимационного листа x * y
    TRect<int> Rows;

    // Количество времени (процессорных тактов), выделенных под кадр
    Uint32 FrameTimer;

private:
    // Использование собственной матрицы с текстурными координатами
    bool UsingCustomTexCoords;

    // Матрица, содержащая текстурные координаты
    glm::mat4x2 TexCoords;

    // Буфер для работы с матрицей текстурных координат
    glm::vec4 TexBuffer;

public:
    CTexture(GLuint PreparedID, int w, int h);
    ~CTexture();

    // *** Подгрузить данные об анимации из конфига
    // [Основная нода уже открытого конфига; секция с данными об анимации]
    [[nodiscard]] bool InitAnimation(tinyxml2::XMLElement* Node);

private:
    // *** Разрезать текстуру на кадры
    void Crope(int w, int h, int Frames = 0);

    // *** Установить активным кадр анимации с указанными индексом
    void UpdateFrame(int FrameIndex);

public:
    // *** Обновление анимации
    void UpdateAnimation();

public:
    // *** Получить (если есть) уникальную матрицу с текстурными координатами
    void GetTexCoords(glm::mat4x2& To);

public:
    // *** Получить идентификатор текстуры
    const GLuint& GetID();

public:
    // *** Получить актуальную длину текстуры
    const int& w();

    // *** Получить актуальную ширину текстуры
    const int& h();

public:
    // *** Получить название использующегося шейдера
    const std::string& GetShader();
};
