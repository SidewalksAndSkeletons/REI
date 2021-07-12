#pragma once

// *** Буфер, применяемый для отрисовки объектов
class CVertexBufferObject final
{
private:
    // Указатель на данные VBO
    GLfloat* Data;

    // Указатель на область данных, содержащую текущие вертексы
    GLfloat* CurrentData;

    // Индекс текущего объекта, отправленного в отрисовку
    GLuint DataIndex;

    // Ядро класса. Основной Vertex Buffer Object
    GLuint Buffer;

    // Размер VBO в байтах
    GLuint Size;

public:
    CVertexBufferObject();
    ~CVertexBufferObject();

    [[nodiscard]] bool Init();

public:
    // *** Отрисовка вертекстных буферов и их сброс
    void FlushBuffers();
};
