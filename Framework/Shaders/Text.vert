#version 330 core

// Ортографическая матрица проекции
uniform mat4 ProjectionMatrix;

// Входные данные
in vec4 InputPosition;  // Позиция
in vec4 InputColor;     // Цвет
in vec2 InputTexCoord;  // Координаты внутри текстуры

// Результирующие данные (направляются в фрагментный шейдер)
out vec4 Color; 		// Цвет
out vec2 TexCoord;		// Координаты внутри текстуры
out vec2 Position;		// Позиция на экране

void main() 
{
	// Отправляем цвет и текстурную позицию в фрагментный шейдер
    Color = InputColor;
    TexCoord = InputTexCoord;
	
	// Отправляем позицию
	Position = InputPosition.xy;
	
	// Трансформируем вершинную позицию, используя матрицу проекции
    gl_Position = ProjectionMatrix * InputPosition;
}