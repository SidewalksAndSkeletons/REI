#pragma once

// *** Класс, обеспечивающий работу с видеокартой
class CRenderTarget final : public ISingleton<CRenderTarget>
{
private:
    // Текущее окно
    std::shared_ptr<CWindow> Window;

    // Модуль, обеспечивающий работу с шейдерами
    std::shared_ptr<CShadersManager> ShadersManager;

    // OpenGL-контекст, обеспечивающий отрисовку
    SDL_GLContext Context;

private:
    // Vertex array object
    GLuint VAO;

    // Element buffer object
    GLuint EBO;

    // Vertex buffer object
    GLuint VBO;

private:
    // Хранилище, содержащее все загруженные шрифты
    std::unordered_map<std::string, TTF_Font*> Fonts;

    // Хранилище, содержащее доступные цвета
    std::unordered_map<std::string, SDL_Color> Colors;

public:
    CRenderTarget();
    CRenderTarget(const CRenderTarget&) = delete;
    CRenderTarget& operator=(const CRenderTarget&) = delete;
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
    // *** Актуализировать значение поля зрения камеры
    void SetViewPoint();

public:
    // *** Загрузить шрифт
    bool LoadFont(std::string_view Name, std::string_view Path, int Size);

    // *** Добавить новый цвет в коллекцию
    void LoadColor(std::string_view Name, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

private:
    // *** Сгенерировать OpenGL-текстуру
    GLuint GenerateTexture(SDL_Surface* Surface, GLuint Format);

public:
    // *** Создать текстуру, содержащую в себе запечённый текст
    [[nodiscard]] CTexture* CreateTexture(const char* Msg, const std::string& Font, const std::string& Color);

    // *** Создать текстуру, содержащую в себе изображение
    [[nodiscard]] CTexture* CreateTexture(std::string_view Path);

    // *** Создать текстуру-анимацию, основываясь на информации из конфига
    [[nodiscard]] CTexture* CreateTexture(std::string_view Section, tinyxml2::XMLElement* Node);

public:
    // *** Отрисовать текстуру
    // [Если не указать размеры, будут использованы размеры исходного изображения/текста]
    void Render(CTexture* Texture, int x, int y, int w = 0, int h = 0, bool FlipX = false, bool FlipY = false, GLfloat Angle = 0.0f);
};

inline std::shared_ptr<CRenderTarget> RenderTarget = nullptr;
