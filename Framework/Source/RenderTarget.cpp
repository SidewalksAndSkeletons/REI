#include "StdAfx.h"

// Индексы, применяемые для отрисовки текстурных квадов
static GLuint Indices[] =
{
    0, 1, 2,
    2, 3, 0
};

// Стандартная TexCoord-матрица для отрисовки единого спрайта
static glm::mat4x2 TexCoordsMatrix =
{
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f
};

CRenderTarget::CRenderTarget() : Context{}, VBO{}, Window{}, ShadersManager{}
{
    VAO = 0;
    EBO = 0;
}

CRenderTarget::~CRenderTarget()
{
    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
    }

    if (EBO)
    {
        glDeleteBuffers(1, &EBO);
    }

    SDL_GL_DeleteContext(Context);

    for (auto& Iter : Fonts)
    {
        TTF_CloseFont(Iter.second);
    }

    Fonts.clear();
    Colors.clear();

    TTF_Quit();
    IMG_Quit();
}

bool CRenderTarget::Init()
{
    Window = Window->CreateInstance();

    // Инициализация окна
    if (!Window->Init())
    {
        DEBUG_ERROR("Window initialization failed!");
        return false;
    }

    // Инициализация OpenGL
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

    ShadersManager = ShadersManager->CreateInstance();

    // Инициализация шейдеров
    if (!ShadersManager->Init())
    {
        DEBUG_ERROR("Shaders manager initialization failed!");
        return false;
    }

    // Настраиваем поле зрения камеры
    SetViewPoint();

    return true;
}

bool CRenderTarget::InitOpenGL()
{
    // Создаём OpenGL-контекст
    Window->CreateContext(Context);

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

    // Инициализируем GL3
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
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Инициализируем EBO
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
    // Обновляем картинку на экране
    Window->Swap();
}

void CRenderTarget::SetViewPoint()
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    // Обновляем значение поля зрения
    Window->GetViewPoint(x, y, w, h);
    glViewport(x, y, w, h);

    // Используем Projection-матрицу
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Настраиваем проекционную матрицу
    Window->GetProjection(w, h);
    glm::mat4 Projection = glm::ortho(0.0f, static_cast<GLfloat>(w), static_cast<GLfloat>(h), 0.0f, -1.0f, 1.0f);

    // Передаём матрицу в шейдеры
    ShadersManager->SendProjectionMatrix(Projection);
}

bool CRenderTarget::LoadFont(std::string_view Name, std::string_view Path, int Size)
{
    if (Name.empty() || Path.empty())
    {
        DEBUG_ERROR("Font details was empty!");
        return false;
    }

    // Указываем корректный путь согласно иерархии приложения
    std::string NewPath = "Fonts/";
    String::Unite(NewPath, Path);

    // Пробужем загрузить шрифт
    TTF_Font* Font = TTF_OpenFont(String::GetPath(NewPath).c_str(), Size);

    if (!Font)
    {
        DEBUG_ERROR("Can't load this font: ", Name);
        return false;
    }

    Fonts.emplace(Name, Font);
    return true;
}

void CRenderTarget::LoadColor(std::string_view Name, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    // Собираем из исходный данных новый цвет
    Colors.emplace(Name, SDL_Color(r, g, b, a));
}

GLuint CRenderTarget::GenerateTexture(SDL_Surface* Surface, GLuint Format)
{
    GLuint ID = 0;
    glGenTextures(1, &ID);

    // Не удалось сгенерировать - всё плохо
    if (!ID)
    {
        DEBUG_ERROR("Texture ID is nullptr!");
        return 0;
    }

    // Связываем идентификатор и созданную текстуру
    glBindTexture(GL_TEXTURE_2D, ID);

    // Генерируем 2D-изображение при помощи данные о пикселях поверхности
    glTexImage2D(GL_TEXTURE_2D, 0, Format, Surface->w, Surface->h, 0, Format, GL_UNSIGNED_BYTE, Surface->pixels);

    // Устанавливаем настройки фильтрации
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return ID;
}

CTexture* CRenderTarget::CreateTexture(const char* Msg, const std::string& Font, const std::string& Color)
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

    // Генерируем OpenGL-текстуру на основе данных о поверхности
    GLuint ID = GenerateTexture(Surface, GL_RGBA);

    // Создаём новую текстуру с указанными размерами
    CTexture* Texture = new CTexture(ID, w, h);

    // Освобождаем поверхность
    SDL_FreeSurface(Surface);

    return Texture;
}

CTexture* CRenderTarget::CreateTexture(std::string_view Path)
{
    // Указываем корректный путь согласно иерархии приложения
    std::string CorrectPath = "Textures/";
    String::Unite(CorrectPath, Path);

    // Загружаем изображение по указанному пути
    SDL_Surface* Surface = IMG_Load(String::GetPath(CorrectPath).c_str());

    if (!Surface)
    {
        DEBUG_ERROR("Surface is nullptr, path: ", CorrectPath);
        return nullptr;
    }

    // Устанавливаем формат текстуры
    GLuint Format = (Surface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;

    // Генерируем OpenGL-текстуру на основе данных о поверхности
    GLuint ID = GenerateTexture(Surface, Format);

    // Создаём новую текстуру с указанными размерами
    CTexture* Texture = new CTexture(ID, Surface->w, Surface->h);

    // Освобождаем поверхность
    SDL_FreeSurface(Surface);

    return Texture;
}

CTexture* CRenderTarget::CreateTexture(std::string_view Section, tinyxml2::XMLElement* Node)
{
    tinyxml2::XMLElement* Element = Node->FirstChildElement(Section.data());

    if (!Element)
    {
        DEBUG_ERROR("TinyXML2: Config error: element [", Section, "] not found!");
        return nullptr;
    }

    // Подгружаем текстуру
    CTexture* Texture = CreateTexture(GetElementText(Element->FirstChildElement("Texture")));

    if (!Texture)
        return nullptr;

    // Подгружаем специфическую информацию об анимации
    if (!Texture->InitAnimation(Element))
    {
        DEBUG_ERROR("Texture animation details was incorrect!");
        return nullptr;
    }

    return Texture;
}

void CRenderTarget::Render(CTexture* Texture, int x, int y, int w, int h, bool FlipX, bool FlipY, GLfloat Angle)
{
    if (!Texture)
    {
        DEBUG_ERROR("Texture is nullptr!");
        return;
    }

    // Устанавливаем корректные размеры текстур
    w = (w > 0) ? w : Texture->w();
    h = (h > 0) ? h : Texture->h();

    // Развороты по осям
    if (FlipX)
    {
        x += w;
        w = -w;
    }

    if (FlipY)
    {
        y += h;
        h = -h;
    }

    // Настраиваем координаты вершин
    glm::vec2 v1 = { static_cast<GLfloat>(x),                static_cast<GLfloat>(y) };
    glm::vec2 v2 = { static_cast<GLfloat>(x + w),            static_cast<GLfloat>(y) };
    glm::vec2 v3 = { static_cast<GLfloat>(x + w),            static_cast<GLfloat>(y + h) };
    glm::vec2 v4 = { static_cast<GLfloat>(x),                static_cast<GLfloat>(y + h) };

    // Если используется вращение
    if (Angle > 1.0f)
    {
        // Получаем точку, относительно которой требуется производить вращение
        glm::vec2 Rotate = OpenGL::GetCenter(x, y, w, h);

        // Вращаем вертексы под указанным углом
        OpenGL::Rotate(v1, Angle, Rotate);
        OpenGL::Rotate(v2, Angle, Rotate);
        OpenGL::Rotate(v3, Angle, Rotate);
        OpenGL::Rotate(v4, Angle, Rotate);
    }

    // Используемая матрица с текстурными координатами
    glm::mat4x2& TMat = TexCoordsMatrix;

    // Если есть - используем уникальную
    Texture->GetTexCoords(TMat);

    // Формируем массив, основываясь на данных текстурного квадрата
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
    ShadersManager->Use(Texture->GetShader());

    // Связываем активную текстуру с идентификатором
    glBindTexture(GL_TEXTURE_2D, Texture->GetID());

    // Создаём и инициализируем вертексные данные и вершины
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // Отрисовываем затекстуренные элементы
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
