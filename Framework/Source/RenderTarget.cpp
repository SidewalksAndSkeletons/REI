#include "StdAfx.h"

// Индексы, применяемые для отрисовки текстурных квадов
static GLuint Indices[] =
{
    0, 1, 2,
    2, 3, 0
};

// Ортографическая матрица проекции
static glm::mat4 OrthoMatrix =
{
    0.0f,    0.0f,     0.0f,    0.0f,
    0.0f,    0.0f,     0.0f,    0.0f,
    0.0f,    0.0f,     0.0f,    0.0f,
   -1.0f,    1.0f,    -1.0f,    1.0f
};

// Стандартная TexCoord-матрица для отрисовки единого спрайта
static glm::mat4x2 TexCoordsMatrix =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

CRenderTarget::CRenderTarget() : Context {}, VAO(0), EBO(0)
{}

CRenderTarget::~CRenderTarget()
{
    // Освобождаем память, занятую вертексными массивами VAO
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
    }

    // Освобождаем память, занятую буферами EBO
    if (EBO)
    {
        glDeleteBuffers(1, &EBO);
    }

    // Освобождаем память, занятую OpenGL-контекстом
    SDL_GL_DeleteContext(Context);

    // Очищаем память, занятую шрифтами
    for (auto& Iter : Fonts)
    {
        TTF_CloseFont(Iter.second);
    }

    Fonts.clear();

    Colors.clear();

    // Корректно выходим из библиотек
    TTF_Quit();
    IMG_Quit();
}

bool CRenderTarget::Init()
{
    // Настройка окна
    if (!Window.Init())
    {
        DEBUG_ERROR("Window initialization failed!");
        return false;
    }

    // Настройка OpenGL
    if (!InitOpenGL())
    {
        DEBUG_ERROR("OpenGL initialization failed!");
        return false;
    }

    // Инициализация шрифтов
    if (!InitFonts())
    {
        DEBUG_ERROR("Fonts initialization failed!");
        return false;
    }

    // Инициализация шейдеров
    if (!ShadersManager.Init())
    {
        DEBUG_ERROR("Shaders manager initialization failed!");
        return false;
    }

    // Устанавливаем поле зрение камеры
    GL_SetViewPoint();

    return true;
}

bool CRenderTarget::InitOpenGL()
{
    // Создаём OpenGL-контекст
    Context = SDL_GL_CreateContext(Window.Get());

    // Включаем вертикальную синхронизацию
    if (SDL_GL_SetSwapInterval(SDL_ENABLE))
    {
        DEBUG_ERROR("OpenGL: VSync error: ", SDL_GetError());
        return false;
    }

    // Устанавливаем Profile Core
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE))
    {
        DEBUG_ERROR("OpenGL: Context profile core disabled: ", SDL_GetError());
        return false;
    }

    // Устанавливаем версию контекста OpenGL
    if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) || SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3))
    {
        DEBUG_ERROR("OpenGL: Version select failed: ", SDL_GetError());
        return false;
    }

    // Инициализируем библиотеку glew
    if (glewInit() != GLEW_OK)
    {
        DEBUG_ERROR("OpenGL: glewInit() not success: ", SDL_GetError());
        return false;
    }

    // Инициализация GL3
    if (!InitGL3())
    {
        DEBUG_ERROR("OpenGL: initialization failed: ", SDL_GetError());
        return false;
    }

    return true;
}

bool CRenderTarget::InitGL3()
{
    // Включаем прозрачность у текстур
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Инициализируем VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    if (!VAO)
    {
        DEBUG_ERROR("VAO initialization failed!");
        return false;
    }

    // Инициализируем VBO
    if (!VBO.Init())
    {
        DEBUG_ERROR("VBO initialization failed!");
        return false;
    }

    // Инициализация EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    if (!EBO)
    {
        DEBUG_ERROR("EBO initialization failed!");
        return false;
    }

    return true;
}

bool CRenderTarget::InitFonts()
{
    // Инициализация библиотеки для работы с текстом
    if (TTF_Init() != 0)
    {
        DEBUG_ERROR("TTF initialization failed: ", TTF_GetError());
        return false;
    }

    // Загружаем шрифты
    if (!LoadFont("Krapka_36", "DR_Krapka_Square_ExtraBold.otf", 36))
        return false;

    // Загружаем цвета
    LoadColor("White", 255, 255, 255);
    LoadColor("Red", 255, 0, 0);

    return true;
}

void CRenderTarget::RenderFirstPhase()
{
    // Очищаем экран
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void CRenderTarget::RenderSecondPhase()
{
    FlushBuffers();

    // Обновляем картинку на экране
    SDL_GL_SwapWindow(Window.Get());
}

void CRenderTarget::FlushBuffers()
{
    // Используем стандартную шейдерную программу
    ShadersManager.Use("Default");

    // Работаем с VBO
    VBO.FlushBuffers();
}

void CRenderTarget::GL_GetViewPointScale(int* w, int* h)
{
    double s = fmin(
        Window.w() / static_cast<double>(Window.GetViewPoint().w()),
        Window.h() / static_cast<double>(Window.GetViewPoint().h())
    );

    *w = static_cast<int>(Window.GetViewPoint().w() * s);
    *h = static_cast<int>(Window.GetViewPoint().h() * s);
}

void CRenderTarget::GL_SetViewPoint()
{
    int x = 0;
    int y = 0;
    int w = Window.w();
    int h = Window.h();

    GL_GetViewPointScale(&w, &h);

    // Центрируем точку обзора
    x = Window.w() / 2 - w / 2;
    y = Window.h() / 2 - h / 2;

    glViewport(x, y, w, h);

    // Обновляем значение ортографической матрицы проекции
    OrthoMatrix[0][0] = 2.0f / Window.GetViewPoint().w();
    OrthoMatrix[1][1] = -2.0f / Window.GetViewPoint().h();

    GL_ApplyProjection();
}

void CRenderTarget::GL_ApplyProjection()
{
    // Используем стандартную шейдерную программу
    ShadersManager.Use("Default");

    // Передаём матрицу проекции в активный шейдер
    ShadersManager.SetMatrix4(ShadersManager.Get("Default"), "u_mvpMatrix", OrthoMatrix);

    // Используем шейдер для отрисовки текста
    ShadersManager.Use("Text");

    // Передаём матрицу проекции в активный шейдер
    ShadersManager.SetMatrix4(ShadersManager.Get("Text"), "u_mvpMatrix", OrthoMatrix);
}

bool CRenderTarget::LoadFont(const char* Name, const std::string_view Path, int Size)
{
    // Если нечего загружать
    if (!Name || Path.empty())
    {
        DEBUG_ERROR("Font details was empty!");
        return false;
    }

    // Указываем корректный путь относительно папки Fonts
    std::string NewPath = "Fonts/";
    String::Unite(NewPath, Path);

    // Пробужем проинициализировать шрифт
    TTF_Font* Font = TTF_OpenFont(String::GetPath(NewPath).c_str(), Size);

    // Если не удалось подгрузить - выводим сообщение об ошибке
    if (!Font)
    {
        DEBUG_ERROR("Can't load this font: ", Name);
        return false;
    }

    Fonts.emplace(Name, Font);
    return true;
}

void CRenderTarget::LoadColor(const char* Name, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Собираем из исходный данных новый цвет
    Colors.emplace(Name, SDL_Color(r, g, b, a));
}

bool CRenderTarget::GenerateTexture(CTexture* Texture, SDL_Surface* Surface, const GLuint& Format)
{
    if (!Texture)
    {
        DEBUG_ERROR("Texture is nullptr!");
        return false;
    }

    // Получаем идентификатор текстуры
    GLuint* ID = &Texture->GetID();

    if (!ID)
    {
        glGenTextures(1, ID);
    }

    // Не удалось сгенерировать - всё плохо
    if (!ID)
    {
        DEBUG_ERROR("Texture ID is nullptr!");
        return false;
    }

    // Связываем идентификатор и созданную текстуру
    glBindTexture(GL_TEXTURE_2D, *ID);

    // Генерируем 2D-изображение при помощи данные о пикселях поверхности
    glTexImage2D(GL_TEXTURE_2D, 0, Format, Texture->w(), Texture->h(), 0, Format, GL_UNSIGNED_BYTE, Surface->pixels);

    // Устанавливаем настройки фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

CTexture* CRenderTarget::CreateTexture(const char* Msg, const char* Font, const char* Color)
{
    // Запекаем изображение в поверхность
    SDL_Surface* Surface = TTF_RenderUTF8_Blended(Fonts[Font], Msg, Colors[Color]);

    if (!Surface)
    {
        DEBUG_ERROR("TTF error: ", TTF_GetError());
        return nullptr;
    }

    // Получаем размеры запечённого текста
    int w = 0;
    int h = 0;
    TTF_SizeUTF8(Fonts[Font], Msg, &w, &h);

    // Создаём новую текстуру с указанными размерами
    CTexture* Texture = new CTexture(w, h);

    // Генерируем OpenGL-текстуру на основе данных о поверхности
    bool Result = GenerateTexture(Texture, Surface, GL_RGBA);

    // Освобождаем поверхность
    SDL_FreeSurface(Surface);

    return Result ? Texture : nullptr;
}

CTexture* CRenderTarget::CreateTexture(const std::string_view Path)
{
    // Указываем корректный путь
    std::string CorrectPath = "../Textures/";
    String::Unite(CorrectPath, Path);

    // Подгружаем поверхность по новому пути
    SDL_Surface* Surface = IMG_Load(CorrectPath.c_str());

    if (!Surface)
    {
        DEBUG_ERROR("Surface is nullptr, path: ", CorrectPath);
        return nullptr;
    }

    // Устанавливаем формат текстуры
    GLuint Format = (Surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

    // Создаём новую текстуру с указанными размерами
    CTexture* Texture = new CTexture(Surface->w, Surface->h);

    // Генерируем OpenGL-текстуру на основе данных о поверхности
    bool Result = GenerateTexture(Texture, Surface, Format);

    // Освобождаем поверхность
    SDL_FreeSurface(Surface);

    return Result ? Texture : nullptr;
}

void CRenderTarget::Render(CTexture* Texture, int x, int y, int w, int h)
{
    if (!Texture)
    {
        DEBUG_ERROR("Texture is nullptr!");
        return;
    }

    // На данный момент текстуры ещё не находятся в буферах, поэтому нам нужно
    // очиситить все буферы для того, чтобы текстуры отрисовывались в корректной последовательности
    FlushBuffers();

    // Устанавливаем корректные размеры текстур
    w = (w > 0) ? w : Texture->w();
    h = (h > 0) ? h : Texture->h();

    // Настраиваем координаты вершин
    glm::vec2 v1 = { static_cast<GLfloat>(x),                static_cast<GLfloat>(y) };
    glm::vec2 v2 = { static_cast<GLfloat>(x + w),            static_cast<GLfloat>(y) };
    glm::vec2 v3 = { static_cast<GLfloat>(x + w),            static_cast<GLfloat>(y + h) };
    glm::vec2 v4 = { static_cast<GLfloat>(x),                static_cast<GLfloat>(y + h) };

    // Используемая матрица с текстурными координатами
    glm::mat4x2& TMat = TexCoordsMatrix;

    // Если есть - используем уникальную
    Texture->GetTexCoords(TMat);

    // Форматируем массив, основываясь на данных текстурного квадрата
    GLfloat Vertices[] =
    {
        /*
            Координаты           Цвета          Текстурные координаты
              вершин                                   (x, y)
        */
        v1.x, v1.y,     1.0f, 1.0f, 1.0f, 1.0f,   TMat[0][0], TMat[0][1],    // Верхний левый угол
        v2.x, v2.y,     1.0f, 1.0f, 1.0f, 1.0f,   TMat[1][0], TMat[1][1],    // Верхний правый угол
        v3.x, v3.y,     1.0f, 1.0f, 1.0f, 1.0f,   TMat[2][0], TMat[2][1],    // Нижний правый угол
        v4.x, v4.y,     1.0f, 1.0f, 1.0f, 1.0f,   TMat[3][0], TMat[3][1]     // Нижний левый угол
    };

    // Используем шейдер для отрисовки текста
    ShadersManager.Use("Text");

    // Связываем активную текстуру с идентификатором
    glBindTexture(GL_TEXTURE_2D, Texture->GetID());

    // Создаём и инициализируем вертексные данные и вершины
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // Отрисовываем затекстуренные элементы
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
