#ifndef _LOGGER_CPP
#define _LOGGER_CPP

#include "logger.hpp"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
using namespace logger;

void logger::setColor(COLOR text, COLOR background)
{
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(H, background * 16 + text);
}

void logger::mcLog(FILE *out, LOG_TYPE type, LOG str, ...)
{
	va_list args;
	va_start(args, str);

	switch (type)
	{
		case TRACE_LOG:
			setColor(BLUE);
			std::fprintf(out, "TRACE: ");
			setColor(WHITE);
			std::vfprintf(out, str, args);
			break;
		case ERROR_LOG:
			setColor(RED);
			std::fprintf(out, "ERROR: ");
			setColor(WHITE);
			std::vfprintf(out, str, args);
			break;
		case CRITICAL_LOG:
			setColor(YELLOW);
			std::fprintf(out, "CRITICAL: ");
			setColor(WHITE);
			std::vfprintf(out, str, args);
			break;
		case DEFAULT_LOG:
			std::vfprintf(out, str, args);
	}

	va_end(args);
}

void logger::mcLog(FILE *out, LOG str, ...)
{
	va_list args;
	va_start(args, str);

	std::vfprintf(out, str, args);

	va_end(args);
}

void logger::mcLog(LOG_TYPE type, LOG str, ...)
{
	FILE *out;

	switch (type)
	{
		case logger::TRACE_LOG:
			out = logStream;
			break;
		case logger::ERROR_LOG:
			out = errorStream;
			break;
		case logger::CRITICAL_LOG:
			out = criticalStream;
	}

	switch (type)
	{
		va_list args;
		va_start(args, str);

		case TRACE_LOG:
			setColor(BLUE);
			std::fprintf(out, "TRACE: ");
			setColor(WHITE);
			std::vfprintf(out, str, args);
			break;
		case ERROR_LOG:
			setColor(RED);
			std::fprintf(out, "ERROR: ");
			setColor(WHITE);
			std::vfprintf(out, str, args);
			break;
		case CRITICAL_LOG:
			setColor(YELLOW);
			std::fprintf(out, "CRITICAL: ");
			setColor(WHITE);
			std::vfprintf(out, str, args);
			break;
		case DEFAULT_LOG:
			std::vfprintf(out, str, args);

		va_end(args);
	}
}

#endif //!_LOGGER_CPP