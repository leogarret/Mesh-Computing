#ifndef __MT_LOGGER_CPP_
#define __MT_LOGGER_CPP_

#include "mtlogger.hpp"

#include <sstream>
#include <fstream>

#include <stdarg.h>

using namespace mt;

#endif // !__MT_LOGGER_CPP_

/*
**	Cette fonction créée un nom de fichier en fonction de la sortie et de l'id du thread.
**	*** Exemple ***
**	Si on sélectionne la sortie mttrace et que l'id du thread est 1234, le nom du fichier sera trace-1234.txt.
*/
std::string get_filename(MT_OUT stream, std::thread::id tid)
{
	std::stringstream ss;
	ss << tid;
	std::string s = ss.str();

	std::string fn(stream);
	fn += "-";
	fn += s.c_str();
	fn += ".txt";
	return fn.c_str();
}

/*
**	Cette fonction écrit un texte formaté (comme la fonction printf), mais elle peut
**	rediriger son texte dans n'importe quelle sortie.
**	Le paramètre type est un enum qui contient les valeurs suivantes :
**	- TRACE_LOG = 0,
**	- ERROR_LOG = 1,
**	- CRITICAL_LOG = 2,
**	- DEFAULT_LOG = 3
*/
void mt::log(FILE *out, LOG_TYPE type, LOG str, ...)
{
	va_list args = NULL;
	va_start(args, str);

	switch (type)
	{
		case TRACE_LOG:
			std::fprintf(out, "TRACE: ");
			std::vfprintf(out, str, args);
			break;
		case ERROR_LOG:
			std::fprintf(out, "ERROR: ");
			std::vfprintf(out, str, args);
			break;
		case CRITICAL_LOG:
			std::fprintf(out, "CRITICAL: ");
			std::vfprintf(out, str, args);
			break;
		case DEFAULT_LOG:
			std::vfprintf(out, str, args);
	}

	va_end(args);
}

/*
**	Cette fonction écrit un texte formaté (comme la fonction printf), mais elle peut
**	rediriger son texte dans n'importe quelle sortie.
*/
void mt::log(FILE *out, LOG str, ...)
{
	va_list args = NULL;
	va_start(args, str);

	std::vfprintf(out, str, args);

	va_end(args);
}

/*
**	Cette fonction écrit un texte formaté (comme la fonction printf).
**  Elle redirige le texte dans un fichier texte associé à son thread.
**	*** Exemple ***
**  Si on appelle cette fonction dans un thread don l'id est 3456 et que
**	nous choisissons tmttrace comment paramètre out, le texte
**  sera alors redirigé dans le fichier texte trace-3456.txt.
*/
int mt::tlog(MT_OUT out, LOG str, ...)
{
	FILE *fi = fopen(get_filename(out, std::this_thread::get_id()).c_str(), "a");

	if (fi == nullptr)
		return -1;

	va_list args = NULL;
	va_start(args, str);

	std::vfprintf(fi, str, args);

	va_end(args);

	return 0;
}

/*
**	Cette fonction écrit un texte formaté (comme la fonction printf) dans la sortie
**	standard.
**	Le paramètre type peut être égal à:
**	- TRACE_LOG = 0,
**	- ERROR_LOG = 1,
**	- CRITICAL_LOG = 2,
**	- DEFAULT_LOG = 3
*/
void mt::log(LOG_TYPE type, LOG str, ...)
{
	va_list args = NULL;
	va_start(args, str);

	switch (type)
	{
		case TRACE_LOG:
			std::fprintf(stdout, "TRACE: ");
			std::vfprintf(stdout, str, args);
			break;
		case ERROR_LOG:
			std::fprintf(stdout, "ERROR: ");
			std::vfprintf(stdout, str, args);
			break;
		case CRITICAL_LOG:
			std::fprintf(stdout, "CRITICAL: ");
			std::vfprintf(stdout, str, args);
			break;
		case DEFAULT_LOG:
			std::vfprintf(stdout, str, args);
	}

	va_end(args);
}