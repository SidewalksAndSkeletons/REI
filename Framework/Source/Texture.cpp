#include "StdAfx.h"


CTexture::CTexture(GLuint PreparedID, int w, int h) : ID(PreparedID), Size{ w, h }, AnimationDetails{}
{}

CTexture::~CTexture()
{
    delete AnimationDetails;

    if (ID)
    {
        glDeleteTextures(1, &ID);
    }
}

bool CTexture::InitAnimation(tinyxml2::XMLElement* Node)
{
    if (AnimationDetails)
    {
        DEBUG_WARNING("Texture animation already initialized!");
        return true;
    }

    AnimationDetails = new CTextureAnimation();

    if (!AnimationDetails->Init(Size, Node))
    {
        DEBUG_ERROR("Texture animation initialization error!");
        return false;
    }

    // Обновляем название используемого шейдера
    Shader = std::move(GetElementText(Node->FirstChildElement("Shader")));

    if (Shader.empty())
    {
        DEBUG_WARNING("Shader type not defined!");
    }

    return true;
}

void CTexture::Update()
{
    if (AnimationDetails)
    {
        AnimationDetails->Update();
    }
}

void CTexture::GetTexCoords(glm::mat4x2& To)
{
    if (AnimationDetails)
    {
        AnimationDetails->GetTexCoords(To);
    }
}

const GLuint& CTexture::GetID()
{
    return ID;
}

int CTexture::w()
{
    // Если текстура кадрирована - используем длину кадра
    return AnimationDetails ? AnimationDetails->w() : Size.w;
}

int CTexture::h()
{
    // Если текстура кадрирована - используем высоту кадра
    return AnimationDetails ? AnimationDetails->h() : Size.h;
}

const std::string& CTexture::GetShader()
{
    return Shader;
}
