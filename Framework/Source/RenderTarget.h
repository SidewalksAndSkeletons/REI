#pragma once

// *** Класс, обеспечивающий работу с видеокартой
class CRenderTarget final
{
private:
    // Текущее окно
    CWindow Window;

    // OpenGL-контекст, обеспечивающий отрисовку
    SDL_GLContext Context;

    // Модуль, обеспечивающий работу с шейдерами
    CShadersManager ShadersManager;

private:
    // Vertex array object
    GLuint VAO;

    // Element buffer object
    GLuint EBO;

    // Вертекстный буфер, применяемый при отрисовке
    CVertexBufferObject VBO;

private:
    // Хранилище, содержащее все загруженные шрифты
    std::unordered_map<const char*, TTF_Font*> Fonts;

    // Хранилище, содержащее доступные цвета
    std::unordered_map<const char*, SDL_Color> Colors;

public:
    CRenderTarget();
    ~CRenderTarget();

    [[nodiscard]] bool Init();

private:
    // *** Подготовка OpenGL
    [[nodiscard]] bool InitOpenGL();

    // *** Подготовка вспомогательных элементов для корректной работы OpenGL
    [[nodiscard]] bool InitGL3();

    // *** Загрузка шрифтов
    [[nodiscard]] bool InitFonts();

public:
    // *** Первая стадия рендеринга: очистка экрана
    void RenderFirstPhase();

    // *** Вторая стадия рендеринга: непосредственно, отрисовка
    void RenderSecondPhase();

private:
    // *** Отрисовка вертекстных буферов и их сброс
    void FlushBuffers();

private:
    // *** Получить масштабирование поля зрения камеры
    void GL_GetViewPointScale(int* w, int* h);

    // *** Актуализировать значение поля зрения камеры
    void GL_SetViewPoint();

    // *** Передать матрицы проеции в шейдеры
    void GL_ApplyProjection();

public:
    // *** Загрузить шрифт
    bool LoadFont(const char* Name, const std::string_view Path, int Size);

    // *** Добавить новый цвет в коллекцию
    void LoadColor(const char* Name, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

private:
    // *** Сгенерировать OpenGL-текстуру
    bool GenerateTexture(CTexture* Texture, SDL_Surface* Surface, const GLuint& Format);

public:
    // *** Создать текст
    CTexture* CreateTexture(const char* Msg, const char* Font, const char* Color);

    // *** Создать изображение
    CTexture* CreateTexture(const std::string_view Path);

public:
    // *** Отрисовать текст
    void Render(CTexture* Texture, int x, int y, int w = 0, int h = 0);
};

inline CRenderTarget RenderTarget;
