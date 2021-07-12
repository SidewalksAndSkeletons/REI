#include "StdAfx.h"

CVertexBufferObject::CVertexBufferObject() : Data(nullptr), CurrentData(nullptr), DataIndex(0), Buffer(0), Size(0)
{}

CVertexBufferObject::~CVertexBufferObject()
{
    // Освобождаем память, занятую буфером
    if (Buffer)
    {
        glDeleteBuffers(1, &Buffer);
    }

    // Освобождаем память, занятую данными при помощи malloc
    if (Data)
    {
        free(Data);
        Data = nullptr;
    }

    CurrentData = nullptr;
}

bool CVertexBufferObject::Init()
{
    // Если буфер уже создан - не порядок
    if (Buffer)
    {
        DEBUG_WARNING("VBO: Buffer already created!");
        return false;
    }

    // Генерируем буферы
    glGenBuffers(1, &Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);

    // Получаем размер
    Size = GL_DETAILS::VBO_OBJECTS_CAPACITY * sizeof(GLfloat) * 24;

    // Подготавливаем данные
    Data = (GLfloat*)malloc(Size);
    CurrentData = Data;

    if (!Buffer)
    {
        DEBUG_ERROR("VBO: Buffer is nullptr!");
        return false;
    }

    return true;
}

void CVertexBufferObject::FlushBuffers()
{
    // Связываем вертекстный буфер с объектом
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);

    // Обновляем данные
    glBufferData(GL_ARRAY_BUFFER, Size, nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)* DataIndex * 24, Data);

    // Отрисовываем все треугольники в буфере
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(DataIndex * 3));

    // Сбрасываем индексы и данные о буферах
    DataIndex = 0;
    CurrentData = Data;
}
