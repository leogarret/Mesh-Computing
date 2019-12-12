#ifndef MC_TRACE_CPP
#define MC_TRACE_CPP

#include "mc_utils.hpp"

#include <string>
#include <mtlogger\mtlogger.hpp>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#endif // MC_TRACE_CPP

/*
** Cette fonction permet d'afficher une erreur suivie d'une durée en milliseconde.
*/
void mc::utils::traceWithTime(std::string str, int timeInMilliseconde)
{
	std::string buff = std::to_string(timeInMilliseconde);
	OutputDebugString(str.c_str());
	OutputDebugString(" [");
	OutputDebugString((!buff.empty()) ? buff.c_str() : "?");
	OutputDebugString("ms]\n");
}

/*
** Cette fonction permet d'afficher une erreur.
*/
void mc::utils::trace(std::string str)
{
	OutputDebugString(str.c_str());
}