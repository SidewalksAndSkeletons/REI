#version 330 core

// Активная текстура
uniform sampler2D Texture;

// *** Входные данные
in vec4 Color;          // Цвет
in vec2 TexCoord;       // Текстурные координаты
in vec2 Position;		// Позиция на экране

// *** Результирующие данные
out vec4 OutputColor;   // Итоговый цвет

void main() 
{
	// Применяем цвет и координатное смещение
    OutputColor = texture(Texture, TexCoord) * Color;
}