#pragma once
#include <cstdarg>
#include <core/utils/logger.h>

namespace cat::physics
{
	inline void TraceJPHCallback(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		VERB("JPH Trace: %s", buffer);
	}

	inline bool AssertJPHCallback(const char* inExpression, const char* inMessage, const char* inFile, std::uint32_t inLine)
	{
		//FATAL("%s : %i : (%s) : (%s)", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
		ERR("%s : %i : (%s) : (%s)", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
		return true;
	};
}