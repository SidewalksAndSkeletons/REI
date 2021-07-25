#pragma once

// *** Класс, представляющий собой обёртку для OpenGL-текстуры
class CTexture final
{
private:
    // Идентификатор текстуры
    GLuint ID;

private:
    // Длина текстуры
    int _w;

    // Длина кадра текстуры
    int _wc;

    // Высота текстуры
    int _h;

    // Высота кадра текстуры
    int _hc;

private:
    // Скорость переключения кадров анимации
    int FramesSpeed;

    // Количество кадров в анимации
    int FramesCount;

    // Текущий кадр анимации
    int FrameCurrent;

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
    CTexture(const GLuint& PreparedID, const int& w, const int& h);
    ~CTexture();

    // *** Подгрузить данные об анимации из конфига
    // [Основная нода уже открытого конфига; секция с данными об анимации]
    [[nodiscard]] bool InitAnimation(tinyxml2::XMLElement* Node);

private:
    // *** Разрезать текстуру на кадры
    void Crope(const int& w, const int& h, const int& Frames = 0);

    // *** Установить активным кадр анимации с указанными индексом
    void UpdateFrame(const int& FrameIndex);

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
    // *** Получить исходную длину текстуры
    const int& w();

    // *** Получить исходную высоту текстуры
    const int& h();
};
