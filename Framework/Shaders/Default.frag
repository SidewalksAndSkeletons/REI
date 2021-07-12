#version 330 core

// Входные данные
in vec4 Color; // Цвет

// Результирующие данные
out vec4 OutputColor; 	// Итоговый цвет

void main()
{
	OutputColor = Color;
}