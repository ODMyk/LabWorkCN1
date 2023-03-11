#include "Server.hpp"

int main() {
	std::ofstream logfile("server.log");
	std::ofstream* p = &logfile;
	Server server(PORT, SIZE, p);

	while (true) {
		if (!server.aread()) {
			continue;
		}

		std::string command = server.getCommand();
		unsigned int ac = server.getAsize();
		if (!command.compare("shutdown") && !ac) {
			server.send("OFF");
			break;
		} else if (!command.compare("start") && ac == 1) {
			unsigned ms = std::stoul(server.getArg(0));
			server.guess(ms);
		} else if (!command.compare("getnum") && !ac) {
			server.send(std::to_string(server.getNumber()));
		} else if (!command.compare("Who") && !ac) {
			server.send("Ostapenko Dmytro K-14. Number guess with Monthe-Carlo method");
		} else {
			server.send("Invalid");
		}
	}
	
	return 0;
}
