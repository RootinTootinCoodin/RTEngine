#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdio.h>
#include <vector>
#include <string>
#include <random>

#include "PCG/include/pcg_random.hpp"
#include "pcg/include/pcg_extras.hpp"
#include "pcg/include/pcg_uint128.hpp"




// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

typedef unsigned int uint;

#define LOG(format, ...) log(__FILE__,__LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

unsigned int Generate_UUID();

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI




enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum componentType
{
	NO_TYPE = 0,
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	SCRIPT
};
// Configuration -----------

#define SCREEN_SIZE 1

#define STANDARD_LINE_SIZE 0.5f

#define VSYNC true

//  NULL just in case ----------------------

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }

#define IN_RANGE( value, min, max ) ( ((value) >= (min) && (value) <= (max)) ? 1 : 0 )
#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )
#define TO_BOOL( a )  ( (a != 0) ? true : false )

uint num_of_digits(int number);

typedef unsigned int uint;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef unsigned char uchar;

template <class VALUE_TYPE> void SWAP(VALUE_TYPE& a, VALUE_TYPE& b)
{
	VALUE_TYPE tmp = a;
	a = b;
	b = tmp;
}

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char* file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

// Performance macros
#define PERF_START(timer) timer.Start()
#define PERF_PEEK(timer) LOG("%s took %f ms", __FUNCTION__, timer.ReadMs())

#define VERSION "0.4-alpha"
#define ASSETS_FOLDER "\\Assets\\"
#define SETTINGS_FOLDER "\\Settings\\"
#define ASSETS_MODELS_FOLDER "\\Assets\\Models\\"
#define ASSETS_TEXTURES_FOLDER "\\Assets\\Textures\\"
#define ASSETS_SCRIPTS_FOLDER "Assets\\Scripts\\"
#define ASSETS_PREFABS_FOLDER "Assets\\Prefabs\\"
#define LIBRARY_PREFABS_FOLDER "Library\\Prefabs\\"
#define LIBRARY_FOLDER "\\Library\\"
#define LIBRARY_MODELS_FOLDER "Library\\Models\\"
#define LIBRARY_MESHES_FOLDER "\\Library\\Meshes\\"
#define LIBRARY_TEXTURES_FOLDER "\\Library\\Textures\\"




#define MODEL_EXTENSIONS(extension) extension==".fbx"|| extension==".FBX"
#define TEXTURE_EXTENSIONS(extension) extension==".png"|| extension==".PNG"|| extension==".dds"|| extension==".DDS"
#define SCRIPT_EXTENSIONS(extension) extension==".lua" || extension==".LUA"



#endif
