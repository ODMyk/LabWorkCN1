#ifndef SERVER_HPP
#define SERVER_HPP

#include <cstdlib>

#include "Socket.hpp"

class Server: public Socket {
public:
	Server(short port, short msgSize, std::ofstream* logfile);
	~Server();
	void guess(unsigned int timeout);
	void pollCommand();
	int getNumber() const;
	void read() override;
	void send(std::string msg) const override;
	bool aread() override;
private:
	long long number;
	int opt;
	int addrrlen;
	int connection;
};

#endif
