#ifndef __MT_LOGGER_HPP_
#define __MT_LOGGER_HPP_

#include <fstream>
#include <thread>

#pragma warning(disable:4996) // Pour éviter les soucis de compatibilité avec C++20

namespace mt {

	static FILE *mttrace = fopen("trace.txt", "w");
	static FILE *mterror = fopen("error.txt", "w");
	static FILE *mtcritical = fopen("critical.txt", "w");

	typedef const char* MT_OUT; // Type du nom d'une sortie pour le multi-threading.
	static MT_OUT tmttrace = "trace";
	static MT_OUT tmterror = "error";
	static MT_OUT tmtcritical = "critical";

	enum LOG_TYPE {
		TRACE_LOG = 0,
		ERROR_LOG = 1,
		CRITICAL_LOG = 2,
		DEFAULT_LOG = 3
	};

	typedef const char* LOG; // Type d'un texte envoyé dans une fonction log

	// Prototypes
	void log(FILE *out, LOG_TYPE type, LOG str, ...);
	void log(FILE *out, LOG str, ...);
	int tlog(MT_OUT out, LOG str, ...);
	void log(LOG_TYPE type, LOG str, ...);

}

#endif // !__MT_LOGGER_HPP_