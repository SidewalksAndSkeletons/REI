#pragma once

// *** STL
#include <algorithm>

// Functions: Containers
#include <vector>
#include <unordered_map>

// Functions: Debug
#ifdef DEBUG
#include <iostream>
#endif

// Functions: Strings
#include <string>
#include <string_view>
#include <charconv>

// Functions: Filesystem
#include <fstream>
#include <filesystem>
#include <iterator>

// *** SDL2
// Исправление: "unresolved external symbol main"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// *** OpenGL
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// *** Framework
#include "SystemConstants.h"
#include "SystemFunctionsStrings.h"
#include "SystemFunctionsFilesystem.h"

// Исходный код движка
#include "Debug.h"
#include "DebugMacros.h"
#include "ViewPoint.h"
#include "Window.h"
#include "ShadersManager.h"
#include "Texture.h"
#include "VBO.h"
#include "RenderTarget.h"
#include "Kernel.h"
