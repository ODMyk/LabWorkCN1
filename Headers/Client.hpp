#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"

class Client: public Socket {
public:
	Client(std::string ip, short port, short msgSize, std::ofstream* logfile);
	~Client();
	void guess(unsigned int timeout);
private:
	const std::string IP;
};

#endif
