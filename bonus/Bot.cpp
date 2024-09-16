/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:54:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/16 16:32:37 by gduranti         ###   ########.fr       */
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
	std::string loginMessage = "PASS " + _serverKey + "\r\nNICK " + _name + "\r\nUSER IRCbot b0 b0 :Hi, I am a bot!\r\n";
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
	std::string cli = str.substr(0, str.find(' '));
	std::string chan = str.substr(str.find("PRIVMSG"), str.find(':') - str.find("PRIVMSG"));
	chan = chan.substr(8);
	chan.erase(std::find(chan.begin(), chan.end(), ' '));
	std::string message = str.substr(str.find(':') + 1, str.find('\r') - str.find(':') - 1);
	std::string ans = "PRIVMSG ";
	if (chan == _name) {
		ans += cli + " :";
		if (message.find("help") != std::string::npos) {
			ans += "Here's a list of command I can provide you\r\n";
			send(_socketOut, ans.c_str(), ans.size(), 0);
			ans = "PRIVMSG " + cli + " :";
			ans += "list users : shows all logged users on the server\r\n";
			send(_socketOut, ans.c_str(), ans.size(), 0);
			ans = "PRIVMSG " + cli + " :";
			ans += "tell me a joke : tells you a joke\r\n";
			send(_socketOut, ans.c_str(), ans.size(), 0);
			ans = "PRIVMSG " + cli + " :";
			ans += "answer= : ask me a yes or no question 'answer=<question>' and I'll tell your future\r\n";
			send(_socketOut, ans.c_str(), ans.size(), 0);
		}
		else
			ans += "Nice to meet you " + cli + "! How can I help you?\r\n";
		send(_socketOut, ans.c_str(), ans.size(), 0);
	}
	else if (message.find(_name) != std::string::npos) {
		ans += chan + " :" + randomMessage() + "\r\n";
		send(_socketOut, ans.c_str(), ans.size(), 0);
	}
}

std::string Bot::randomMessage( void ) {
	std::vector<std::string> vec;
	vec.push_back("I'm gonna FIX that spaghetti!");
	vec.push_back("I REJECT MY HUMANITY, JOJO!!!");
	vec.push_back("Your Stand is like your asshole: you can't go around showing it off to other people.");
	vec.push_back("You pissed me off.");
	vec.push_back("German Science is the best in the world!");
	vec.push_back("Oh, you are approcing me!");
	vec.push_back("ZA WARUDO!");
	vec.push_back("MUDA MUDA MUDA MUDA MUDA");
	vec.push_back("ORA ORA ORA ORA ORA");
	vec.push_back("WRYYYY");
	vec.push_back("Your next line is...");
	vec.push_back("Nigerundayo, Smokey!");
	vec.push_back("Yare yare daze");
	vec.push_back("You think it was joust a normal robot.. but it was me, DIO!");
	vec.push_back("Yare yare daze");
	vec.push_back("I can't beat the shit out of you without getting closer.");
	vec.push_back("Rerorerorerorerorerorerorerorero");
	vec.push_back("My name is Yoshikage Kira, I am 33 years old.");
	return(vec[rand() % vec.size()]);
}