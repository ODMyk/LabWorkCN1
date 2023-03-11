#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <ctime>
#include <chrono>

const short SIZE = 256;
const unsigned MAX_RANGE = 2000000;
const short PORT = 1045;

namespace Utils {
	void parse(std::string &line, std::string &command, std::vector<std::string> &args);
}

#endif
