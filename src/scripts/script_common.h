#pragma once
#include "core/common.h"
// FIXME if possible 
//		  warning C5260: the constant variable "..." has internal linkage in an included header file context, but external linkage in imported header unit context; 
//		  consider declaring it 'inline' as well if it will be shared across translation units, or 'static' to express intent to use it local to this translation unit

#pragma warning (disable : 5260)
#include <sol/sol.hpp>

// If we need a faster compilation 
// Because sol slow asf
//#define CAT_DISABLE_LUA_CAT_API