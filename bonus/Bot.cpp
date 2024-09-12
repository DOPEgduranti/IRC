/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:54:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/12 16:44:16 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Bot.hpp>

bool Bot::_signal = false;

void Bot::signalHandler( int signum ) {
	(void)signum;
	std::cout << "Signal Received!" << std::endl;
	_signal = true;
}

void Bot::setupBot( void ) {
	std::cout << "Setting bot socket" << std::endl;
	setupSocket();
	std::cout << "Bot socket initialization: SUCCESS" << std::endl;
	std::string loginMessage = "PASS " + _serverKey + "\r\nNICK bot\r\nUSER b0 b1 b2 b3\r\n";
	send(_socketFd, loginMessage.c_str(), loginMessage.size(), 0);
	std::cout << "Bot fd:" << _socketFd << " Server fd:" << _serverFd << std::endl;
	while (!_signal) {
		if (poll(&(*_polls.begin()), _polls.size(), -1) == -1 && !_signal)
			throw std::runtime_error("poll(): FAILURE");
		for (size_t i = 0; i < _polls.size(); i++) {
			if (_polls[i].revents & POLLIN) {
				receiveData(_polls[i].fd);
			}
		}
	}
	closeBot();
}

void Bot::setupSocket( void ) {
	int value = 1;
	struct pollfd mypoll;
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
		throw std::runtime_error("server socket generation failed");
	connectServer();
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw std::runtime_error("setting option SO_REUSEADDR on server socket: FAILURE");
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("setting option O_NONBLOCK on server socket: FAILURE");
	mypoll.fd = _socketFd;
	mypoll.events = POLLIN;
	mypoll.revents = 0;
	_polls.push_back(mypoll);
}

void Bot::connectServer( void ) {
	struct sockaddr_in incomeAddress;
	struct pollfd myPoll;
	incomeAddress.sin_port = htons(_serverPort);
	incomeAddress.sin_family = AF_INET;
	incomeAddress.sin_addr.s_addr = inet_addr(_serverIpAddr.c_str());
	if (incomeAddress.sin_addr.s_addr == INADDR_NONE)
		throw std::runtime_error("ip address binding: FAILURE");
	_serverFd = connect(_socketFd, reinterpret_cast<sockaddr *>(&incomeAddress), sizeof(incomeAddress));
	if (_serverFd == -1)
		throw std::runtime_error("connect function: FAILURE");
	if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("setting option O_NONBLOCK on new Client: FAILURE");
	myPoll.fd = _serverFd;
	myPoll.events = POLLIN;
	myPoll.revents = 0;
	_polls.push_back(myPoll);
}

void Bot::receiveData( int fd ) {
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::cout << "Server <" << fd << "> sent this: '" << buffer << "'" << std::endl;
	if (bytes <= 0) {
		std::cout << "Server <" << fd << "> has Disconnected" << std::endl;
		_signal = true;
	}
	else {
		buffer[bytes] = 0;
		std::string str = buffer;
		// scrivere qui cosa farne dei messaggi ricevuti dal server
	}
}

void Bot::closeBot( void ) {
	close(_serverFd);
	if (_socketFd != -1) {
		std::cout << "Bot <" << _socketFd << "> shutdown: SUCCESS" << std::endl;
		close(_socketFd);
	}
}
