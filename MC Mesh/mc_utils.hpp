#ifndef MC_TRACE_HPP
#define MC_TRACE_HPP

#include <ctime>
#include <string>

namespace mc::utils
{

	bool haveExtension(const char *fileName);
	bool extensionCompare(const char *fileName, char *extension);

	void traceWithTime(std::string str, int time);
	void trace(std::string str);

}

#endif //!MC_TRACE_HPP