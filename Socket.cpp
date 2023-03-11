#include "Socket.hpp"

Socket::Socket(short port, short msgSize, std::ofstream* logfile):
	PORT(port), SIZE(msgSize), logfile(logfile) {

	if ((fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
		log("Failed to create socket\n");
		throw 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
}

Socket::~Socket() {
	logfile->close();
}

void Socket::send(std::string msg) const {
	std::string m = msg;
	while (msg.size() != SIZE) {
		msg = msg + " ";
	}
	while (::send(fd, msg.c_str(), msg.size(), 0) < 0) {}
	log("Sent | " + m + '\n');
}

void Socket::read() {
	char buffer[SIZE];
	while (recv(fd, buffer, SIZE, O_NONBLOCK) < 0) {}

	std::string line = buffer;
	std::vector<std::string>().swap(args);
	Utils::parse(line, command, args);
	log("Accepted | " + getLine() + "\n");
}

bool Socket::aread() {
	char buffer[SIZE];
	if (recv(fd, buffer, SIZE, O_NONBLOCK) < 0) {
		return false;
	}
	std::string line = buffer;
	std::vector<std::string>().swap(args);
	Utils::parse(line, command, args);
	log("Accepted | " + getLine() + "\n");
	return true;
}

void Socket::log(std::string msg) const {
	auto timepoint = std::chrono::system_clock::to_time_t(std::chrono::high_resolution_clock::now());
	std::string tmp = std::string(std::ctime(&timepoint));
	tmp = tmp.substr(0, tmp.size() - 1);
	tmp += " | " + msg;
	logfile->write(tmp.c_str(), tmp.size());
	logfile->flush();
}

std::string Socket::getLine() const {
	std::string temp = command;
	for (int i = 0; i < args.size(); ++i) {
		temp += " " + args.at(i);
	}
	return temp;
}

std::string Socket::getCommand() const {
	return command;
}

std::string Socket::getArg(unsigned int ind) const {
	return args.at(ind);
}

unsigned int Socket::getAsize() const {
	return args.size();
}
