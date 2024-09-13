/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:54:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/13 12:46:00 by gduranti         ###   ########.fr       */
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
	std::string loginMessage = "PASS " + _serverKey + "\r\nNICK bot\r\nUSER IRCbot b1 b2 :Hi, I am a bot!\r\n";
	send(_socketOut, loginMessage.c_str(), loginMessage.size(), 0);
	std::cout << "Bot fd:" << _socketOut << " Server fd:" << _socketIn << std::endl;
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
	_socketOut = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketOut == -1)
		throw std::runtime_error("server socket generation failed");
	connectServer();
	if (setsockopt(_socketOut, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw std::runtime_error("setting option SO_REUSEADDR on server socket: FAILURE");
	if (fcntl(_socketOut, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("setting option O_NONBLOCK on server socket: FAILURE");
	mypoll.fd = _socketOut;
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
	_socketIn = connect(_socketOut, reinterpret_cast<sockaddr *>(&incomeAddress), sizeof(incomeAddress));
	if (_socketIn == -1)
		throw std::runtime_error("connect function: FAILURE");
	if (fcntl(_socketIn, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("setting option O_NONBLOCK on new Client: FAILURE");
	myPoll.fd = _socketIn;
	myPoll.events = POLLIN;
	myPoll.revents = 0;
	_polls.push_back(myPoll);
}

void Bot::receiveData( int fd ) {
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::cout << "Server sent this: '" << buffer << "'" << std::endl;
	if (bytes <= 0) {
		std::cout << "Server has Disconnected" << std::endl;
		_signal = true;
	}
	else {
		buffer[bytes] = 0;
		std::string str = buffer;
		if (str.find(":server 004") != std::string::npos) {
			std::string message = "JOIN #botchan\r\nTOPIC #botchan :Welcome to the bot channel!\r\nMODE #botchan +t\r\n";
			send(_socketOut, message.c_str(), message.size(), 0);
		}
		else if (str.find("PRIVMSG") != std::string::npos)
			answer(str);
	}
}

void Bot::closeBot( void ) {
	close(_socketOut);
	std::cout << "Bot shutdown: SUCCESS" << std::endl;
}

void Bot::answer( std::string str ) {
	str.erase(str.begin());
	std::string name = str.substr(0, str.find(' '));
	std::string chan = str.substr(str.find("PRIVMSG"), str.find(':') - str.find("PRIVMSG"));
	chan = chan.substr(8);
	chan.erase(std::find(chan.begin(), chan.end(), ' '));
	std::string message = str.substr(str.find(':') + 1, str.find('\r') - str.find(':') - 1);
	std::string ans = "PRIVMSG ";
	if (chan == "bot") {
		ans += name + " :Hi, nice to meet you!\r\n";
		send(_socketOut, ans.c_str(), ans.size(), 0);
	}
	else if (message.find("bot") != std::string::npos) {
		ans += chan + " :Oh, you are approcing me!\r\n";
		send(_socketOut, ans.c_str(), ans.size(), 0);
	}
}