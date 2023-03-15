#include "Utils.hpp"

void Utils::parse(std::string line, std::string& command, std::vector<std::string> &args) {
	line = line.substr(0, SIZE);
	int ind, ind2;
	std::string arg;

	ind = line.find_first_not_of(' ', 0);
	if (ind == std::string::npos) {
		command = "";
		return;
	}
	command = line.substr(ind, line.find_first_of(' ', ind) - ind);
	{
		int tind = line.find_first_of(' ', ind);
		if (tind == std::string::npos) {
			return;
		}
		tind = line.find_first_not_of(' ', ind + command.size());
		if (tind == std::string::npos) {
			return;
		}
		ind2 = tind;
	}
	line = line.substr(ind2, line.size() - 1 - command.size() - ind);

	while (ind != std::string::npos) {
		ind = line.find_first_of(' ', 0);
		arg = line.substr(0, ind);
		args.push_back(arg);
		int ind2 = line.find_first_not_of(' ', ind + 1);
		if (ind2 != std::string::npos) {
			line = line.substr(ind2, line.size() - 1 - arg.size());
		} else {
			ind = ind2;
		}
	}
}

