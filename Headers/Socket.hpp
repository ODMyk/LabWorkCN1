#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <fstream>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "Utils.hpp"

class Socket {
protected:
	int fd;
	const short PORT;
	const short SIZE;
	std::ofstream* logfile;
	std::string command;
	std::vector<std::string> args;
	struct sockaddr_in addr;
public:
	Socket(short port, short msgSize, std::ofstream* logfile);
	~Socket();
	virtual void send(std::string msg) const;
	virtual void read();
	virtual bool aread();
	void log(std::string msg) const;
	std::string getLine() const;
	std::string getCommand() const;
	std::string getArg(unsigned ind) const;
	unsigned int getAsize() const;
};

#endif
