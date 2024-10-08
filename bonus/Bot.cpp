/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:54:11 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/24 10:13:16 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Bot.hpp>

bool Bot::_signal = false;

Bot::Bot( std::string IpAdd, std::string port, std::string key ) : _name("bot"), _serverIpAddr(IpAdd), _serverPort(static_cast<int>(std::strtol(port.c_str(), NULL, 10))), _serverKey(key) {
	fillSentences();
	fillFutures();
	fillJokes();
}

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
		if (str.find(":server 004") == 0) {
			std::string message = "JOIN #botchan\r\nTOPIC #botchan :Welcome to the bot channel!\r\nMODE #botchan +t\r\n";
			send(_socketOut, message.c_str(), message.size(), 0);
		}
		else if (str.find("PRIVMSG") != std::string::npos)
			answer(str);
		else if (str.find("Online users:") == 0)
			listUsers(str);
		else if (str.find("INVITE") != std::string::npos)
			joinChannel(str);
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
			ans = "PRIVMSG " + cli + " :list users : shows all logged users on the server\r\n";
			send(_socketOut, ans.c_str(), ans.size(), 0);
			ans = "PRIVMSG " + cli + " :tell me a joke : tells you a joke\r\n";
			send(_socketOut, ans.c_str(), ans.size(), 0);
			ans = "PRIVMSG " + cli + " :future= : ask me a yes or no question 'future=<question>' and I'll tell your future\r\n";
		}
		else if (message == "list users") {
			_client = cli;
			ans = "NAMES\r\n";
		}
		else if (message == "tell me a joke")
			ans += _jokes[rand() % _jokes.size()] + "\r\n";
		else if (message.find("future=") == 0)
			ans += tellFuture(message) + "\r\n";
		else
			ans += "Nice to meet you " + cli + "! How can I help you?\r\n";
		send(_socketOut, ans.c_str(), ans.size(), 0);
	}
	else if (message.find(_name) != std::string::npos) {
		ans += chan + " :" + _sentences[rand() % _sentences.size()] + "\r\n";
		send(_socketOut, ans.c_str(), ans.size(), 0);
	}
}

void Bot::listUsers( std::string str ) {
	if (_client.empty())
		return ;
	std::string message = "PRIVMSG " + _client + " :" + str;
	send(_socketOut, message.c_str(), message.size(), 0);
}

void Bot::joinChannel( std::string str ) {
	std::string cli = str.substr(1, str.find("!") - 1);
	std::string chan = str.substr(str.find(_name) + _name.size());
	chan.erase(chan.size() - 1);
	chan.erase(chan.size() - 1);
	std::string message = "JOIN " + chan + "\r\n";
	send(_socketOut, message.c_str(), message.size(), 0);
	message = "PRIVMSG " + chan + " :Hello Everyone! Thank you " + cli + " for the invitation\r\n";
	send(_socketOut, message.c_str(), message.size(), 0);
}

std::string Bot::tellFuture( std::string str ) {
	if (str.size() == 7)
		return ("Please don't be silly");
	return (_futures[rand() % _futures.size()]);
}

void Bot::fillSentences( void ) {
	_sentences.push_back("I'm gonna FIX that spaghetti!");
	_sentences.push_back("I REJECT MY HUMANITY, JOJO!!!");
	_sentences.push_back("Your Stand is like your asshole: you can't go around showing it off to other people.");
	_sentences.push_back("You pissed me off.");
	_sentences.push_back("German Science is the best in the world!");
	_sentences.push_back("Oh, you are approcing me!");
	_sentences.push_back("ZA WARUDO!");
	_sentences.push_back("MUDA MUDA MUDA MUDA MUDA");
	_sentences.push_back("ORA ORA ORA ORA ORA");
	_sentences.push_back("WRYYYY");
	_sentences.push_back("Your next line is...");
	_sentences.push_back("Nigerundayo, Smokey!");
	_sentences.push_back("Yare yare daze");
	_sentences.push_back("You think it was joust a normal robot.. but it was me, DIO!");
	_sentences.push_back("Yare yare daze");
	_sentences.push_back("I can't beat the shit out of you without getting closer.");
	_sentences.push_back("Rerorerorerorerorerorerorerorero");
	_sentences.push_back("My name is Yoshikage Kira, I am 33 years old.");
	_sentences.push_back("Dream Pig");
}

void Bot::fillJokes( void ) {
	_jokes.push_back("What do you call a pile of cats? A meow-ntain.");
	_jokes.push_back("Why do cows wear bells? Because their horns don't work.");
	_jokes.push_back("Why did the bicycle fall over? Because it was two tired.");
	_jokes.push_back("What do elves learn in school? The elf-abet.");
	_jokes.push_back("Why shouldn't you use a broken pencil? Because it's pointless.");
	_jokes.push_back("What has four wheels and flies? A garbage truck!");
	_jokes.push_back("What did the tomato say to the other tomato during a race? Ketchup.");
	_jokes.push_back("What did one wall say to the other? I'll meet you at the corner.");
	_jokes.push_back("What do sea monsters eat? Fish and ships.");
	_jokes.push_back("I lost an electron. You really have to keep an ion them!");
	_jokes.push_back("Your next line is...");
	_jokes.push_back("How many tickles does it take to make an octopus laugh? Ten-tickles.");
	_jokes.push_back("What did 0 say to 8? Nice belt.");
	_jokes.push_back("What does a spy do when he is cold? He goes undercover.");
	_jokes.push_back("What's brown and sticky? A stick.");
	_jokes.push_back("What do you call a sad cup of coffee? Depresso.");
	_jokes.push_back("What do you call a sleeping dinosaur? A dino-snore.");
	_jokes.push_back("What did the football coach say to the broken vending machine? Give me my quarterback.");
	_jokes.push_back("How did the barber win the race? He knew a shortcut!");
}

void Bot::fillFutures( void ) {
	_futures.push_back("As I see it, yes");
	_futures.push_back("It is certain");
	_futures.push_back("It is decidedly so");
	_futures.push_back("Most likely");
	_futures.push_back("Outlook good");
	_futures.push_back("Signs point to yes");
	_futures.push_back("Yes");
	_futures.push_back("Yes, definitely");
	_futures.push_back("You may rely on it");
	_futures.push_back("Reply hazy, try again");
	_futures.push_back("Ask again later");
	_futures.push_back("Better not tell you now");
	_futures.push_back("Cannot predict now");
	_futures.push_back("Concentrate and ask again");
	_futures.push_back("Don't count on it");
	_futures.push_back("My reply is no");
	_futures.push_back("My sources say no");
	_futures.push_back("Outlook not so good");
	_futures.push_back("Very doubtful");
	_futures.push_back("No");
	_futures.push_back("I am sorry but no");
}