#pragma once

// *** Дополнительные данные для анимации
class CTextureAnimation final
{
private:
    // Матрица, содержащая текстурные координаты
    glm::mat4x2 TexCoords;

    // Буфер для работы с матрицей текстурных координат
    glm::vec4 TexBuffer;

private:
    // Размеры кадра w * h
    SRect<int> Size;

    // Количество кадров x * y
    TRect<int> Rows;

    // Количество процессорных тактов, выделенных на кадр
    Uint32 Timer;

    // Количество кадров
    int Count;

    // Текущий кадр
    int Current;

    // Скорость переключения кадров анимации
    int Speed;

public:
    CTextureAnimation();

    [[nodiscard]] bool Init(SRect<int>& Origin, tinyxml2::XMLElement* Node);

private:
    // *** Кадрировать анимацию
    // [Размеры оригинальной текстуры; размеры кадра; количество кадров (рассчитывается автоматически)]
    void Crope(SRect<int>& Origin, int w, int h, int Frames = 0);

    // *** Установить указанный кадр активным
    void SetFrame(int FrameIndex);

public:
    // *** Обновление логики
    void Update();

public:
    // *** Получить активную матрицу с текстурными координатами
    void GetTexCoords(glm::mat4x2& To);

public:
    // *** Получить длину кадра
    int w();

    // *** Получить высоту кадра
    int h();
};
