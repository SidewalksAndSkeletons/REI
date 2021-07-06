#pragma once

// *** STL
#include <algorithm>

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

// *** Framework
#include "SystemFunctionsStrings.h"
#include "SystemFunctionsFilesystem.h"

// Исходный код движка
#include "Debug.h"
#include "DebugMacros.h"
#include "Kernel.h"
