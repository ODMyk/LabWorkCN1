#include "Client.hpp"

Client::Client(std::string ip, short port, short msgSize, std::ofstream* logfile):
	Socket(port, msgSize, logfile), IP(ip) {

	if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
		log("Failed to setup address\n");
		throw 1;
	}

	while (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {}
}

Client::~Client() {
	close(fd);
	Socket::~Socket();
}

void Client::guess(unsigned int timeout) {
	int valread = 0;
	unsigned int number = 0;

	send("start " + std::to_string(timeout));

	while (command.compare("stop")) {
		command = "0";
		number = rand() % (MAX_RANGE + 1);
		send("guess " + std::to_string(number));
		read();
	}
	send("Stopped");
	read();
}
