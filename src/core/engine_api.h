#pragma once
#ifdef CATENGINE_EXPORTS
#define CATENGINE_API __declspec(dllexport)
#else
#define CATENGINE_API __declspec(dllimport)
#endif

// Disable warning "needs to have dll-interface to be used by clients of class"
#pragma warning(disable: 4251)