// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#pragma warning(disable : 4244 4800)
/// C4244 warns agains converting between floats/integers and similar, as you
/// may get bugs involving numerical precision if you don't handle them consciously.
/// C4800 warns against implicit conversions from int to bool. The compiler says
/// there might be a performance hit, but I'm skeptical of that really being the
/// case. Feel free to reenable these warnings if you think you might have a
/// bug related to them (you can do so in an indiviual file with #pragma warning enable).

#if !defined(WIN32) && !defined(CROSS)
#define CROSS
// msvc defines WIN32 if you're building a graphical application
#endif // WIN32

// C++ RunTime Header Files
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#ifdef CROSS
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#else
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include "targetver.h"

// Windows Header Files:
#include <windows.h>
#include <mmsystem.h>
#include <process.h>
#include <windowsx.h>

// DirectX
#include <d3d9.h>
#include <d3dx9.h>

#endif

#undef max
#undef min
// Unnecessary macros; we use functions that these conflict with