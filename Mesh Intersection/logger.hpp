#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include <iostream>
#include <fstream>
#include <stdarg.h>

namespace logger {

	static FILE *logStream = fopen("trace.txt", "w");
	static FILE *errorStream = fopen("error.txt", "w");
	static FILE *criticalStream = fopen("critical.txt", "w");

	enum LOG_TYPE {
		TRACE_LOG = 0,
		ERROR_LOG = 1,
		CRITICAL_LOG = 2,
		DEFAULT_LOG = 3
	};

	enum COLOR {
		BLACK = 0,
		GREEN = 2,
		BLUE = 3,
		RED = 12,
		YELLOW = 14,
		WHITE = 15
	};

	typedef const char* LOG;

	void setColor(COLOR text, COLOR background = BLACK);

	void mcLog(FILE *out, LOG_TYPE type, LOG str, ...);
	void mcLog(FILE *out, LOG str, ...);
	void mcLog(LOG_TYPE type, LOG str, ...);

}

#endif //!_LOGGER_HPP