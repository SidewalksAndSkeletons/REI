#version 330 core

// Входные данные
in vec4 Color;          // Цвет
in vec2 TexCoord;       // Текстурные координаты
in vec2 Position;		// Позиция на экране

// Результирующие данные
out vec4 OutputColor;   // Итоговый цвет

// uniform
uniform sampler2D Texture; // 2д-шный текстурный элемент

void main() 
{
	// Применяем цвет и координатное смещение
    OutputColor = texture(Texture, TexCoord) * Color;
}