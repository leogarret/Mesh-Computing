#ifndef MC_UTILS_H
#define MC_UTILS_H

#include <ctime>
#include <string>

namespace mc::utils {

	bool haveExtension(const char *fileName);
	bool extensionCompare(const char *fileName, char *extension);

	void traceWithTime(std::string str, int time);
	void trace(std::string str);

}

#endif //!MC_UTILS_H