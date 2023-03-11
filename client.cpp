#include "Client.hpp"

int main (int argc, char** args) {
	unsigned int ms;
	std::ofstream logfile("client.log");
	std::ofstream* p = &logfile;
	Client client("127.0.0.1", PORT, SIZE, p);

	if (argc > 1) {
		ms = std::stoul(args[1]);
	} else {
		ms = 200;
	}

	client.send("Who");
	client.read();
	
	client.guess(ms);

	client.send("getnum");
	client.read();

	client.send("shutdown");
	client.read();

	return 0;
}
