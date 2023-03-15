#include "Client.hpp"
#include <iostream>

void defaultProcedure(Client& client, unsigned int ms);

int main (int argc, char** args) {
	srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	unsigned int ms;
	std::ofstream logfile("client.log");
	std::ofstream* p = &logfile;
	Client client("127.0.0.1", PORT, SIZE, p);

	if (argc > 1) {
		ms = std::stoul(args[1]);
		defaultProcedure(client, ms);
	} else {
		std::string line, command;
		std::vector<std::string> args;
		while (true) {
			std::vector<std::string>().swap(args);
			std::cout << ">> ";
			std::getline(std::cin, line);
			Utils::parse(line, command, args);
			if (!command.compare("guess") && args.size() == 1 && !args.at(0).compare("auto")) {
				std::cout << "Going to auto-guess\n";
				unsigned int number = 0;
				command = "";

				while (command.compare("stop") && command.compare("Invalid")) {
					number = rand() % (MAX_RANGE + 1);
					client.send("guess " + std::to_string(number));
					std::cout << "Sent: guess " + std::to_string(number) << std::endl;
					client.read();
					command = client.getCommand();
					std::cout << "Server: " << client.getLine() << std::endl;
				}
				if (command.compare("Invalid")) {
					client.send("Stopped");
					std::cout << "Sent: Stopped\n";
				} else {
					continue;
				}
			} else {
				client.send(line);
			}

			client.read();
			line = client.getLine();
			std::cout << "Server: " << line << std::endl;
			if (!line.substr(0, 4).compare("stop")) {
				client.send("Stopped");
				std::cout << "Auto-reply to 'stop': 'Stopped'\n";
				client.read();
				std::cout << "Server: " << client.getLine() << std::endl;
			} else if (!line.substr(0, 3).compare("OFF")) {
				break;
			}
		}
	}

	
	return 0;
}

void defaultProcedure(Client& client, unsigned int ms) {
	client.send("Who");
	client.read();
	
	client.guess(ms);

	client.send("getnum");
	client.read();

	client.send("shutdown");
	client.read();
}
