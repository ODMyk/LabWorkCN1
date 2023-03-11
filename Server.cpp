#include "Server.hpp"

Server::Server(short port, short msgSize, std::ofstream* inputfile):
	Socket(port, msgSize, inputfile) {

	opt = 1;
	addrrlen = sizeof(addr);
	
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		log("Failed to setup socket options\n");
		throw 1;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(PORT);

	if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
		log("Failed to bind socket\n");
		throw -1;
	}

	if (listen(fd, 1) < 0) {
		log("Failed to listen for incoming connections\n");
		throw -1;
	}

	while ((connection = accept4(fd, (struct sockaddr*) &addr, (socklen_t*) &addrrlen, SOCK_NONBLOCK)) < 0) {}
}

Server::~Server() {
	close(connection);
	shutdown(fd, SHUT_RDWR);
	Socket::~Socket();
}

void Server::guess(unsigned int timeout) {
	number = rand() % MAX_RANGE;
	auto begin = std::chrono::high_resolution_clock::now().time_since_epoch();
	auto end = begin;
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	int closest = (number > MAX_RANGE / 2 ? 0 : MAX_RANGE);
	unsigned attempts = 0;
	while (duration < timeout && number != closest) {
		if (aread() < 1) {
			end = std::chrono::high_resolution_clock::now().time_since_epoch();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			continue;
		}

		if (command.compare("guess") || args.size() != 1) {
			end = std::chrono::high_resolution_clock::now().time_since_epoch();
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			send("Wrong");
			continue;
		}

		++attempts;
		int n = std::stoi(args.at(0));
		closest = (std::abs(number - n) < std::abs(number - closest) ? n : closest);

		end = std::chrono::high_resolution_clock::now().time_since_epoch();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		send("Accepted");
	}
	read();
	if (closest != number) {
		++attempts;
		closest = (std::abs(closest - number) < std::abs(std::stoi(args.at(0)) - number) ? closest : std::stoul(args.at(0)));
	}
	send("stop");
	read();
	std::string tmp = "Attempts: " + std::to_string(attempts) + ". " + (number == closest ? "Guessed" : "Closest: " + std::to_string(closest));
	send(tmp);
}

int Server::getNumber() const {
	return number;
}

void Server::send(std::string msg) const {
	std::string m = msg;
	while (msg.size() != SIZE) {
		msg = msg + " ";
	}
	while (::send(connection, msg.c_str(), msg.size(), 0) < 0) {}
	log("Sent | " + m + '\n');
}

void Server::read() {
	char buffer[SIZE];
	while (recv(connection, buffer, SIZE, O_NONBLOCK) < 0) {}

	std::string line = buffer;
	std::vector<std::string>().swap(args);
	Utils::parse(line, command, args);
	log("Accepted | " + getLine() + "\n");
}

bool Server::aread() {
	char buffer[SIZE];
	if (recv(connection, buffer, SIZE, O_NONBLOCK) < 0) {
		return false;
	}
	std::string line = buffer;
	std::vector<std::string>().swap(args);
	Utils::parse(line, command, args);
	log("Accepted | " + getLine() + "\n");
	return true;
}

