#pragma once

// *** STL
#include <algorithm>
#include <type_traits>
#include <memory>

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
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// *** Framework
#include "SystemConstants.h"
#include "FunctionsStrings.h"
#include "FunctionsFilesystem.h"
#include "FunctionsOpenGL.h"

// *** TinyXML2
#include <TinyXML2.h>
#include <TinyXML2_Additional.h>

// Исходный код движка
#include "ISingleton.h"
#include "Debug.h"
#include "DebugMacros.h"
#include "Rect.h"
#include "Window.h"
#include "ShadersManager.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "SoundTarget.h"
#include "KeyboardManager.h"
#include "GameObject.h"
#include "Actor.h"
#include "Kernel.h"
