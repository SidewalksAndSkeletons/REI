#version 330 core

// Входные данные
in vec4 Color;          // Цвет
in vec2 TexCoord;       // Текстурные координаты

// Результирующие данные
out vec4 OutputColor;   // Итоговый цвет

// uniform
uniform sampler2D Texture; // 2д-шный текстурный элемент

// ### ХРОМАТИЧЕСКАЯ АБЕРРАЦИЯ
// Фактор хроматической аберрации
const float CHROMATIC_FACTOR = 5.0;

// Преобразование red-канала в рамках реализации
float ChromaticAberration()
{
	vec2 Scale = 1.0 / vec2(1920, 1080);
	vec2 UV_MOD = vec2(CHROMATIC_FACTOR, 0.0);
	vec4 Res = texture2D(Texture, TexCoord + UV_MOD * Scale);
	return Res.r;
}

void main() 
{
	// Применяем цвет и координатное смещение
    OutputColor = texture(Texture, TexCoord) * Color;
	
	OutputColor.r = ChromaticAberration();
}