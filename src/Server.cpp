/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:35:41 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 15:48:39 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <Server.hpp>

typedef std::map<std::string, bool (Server::*)(Client &, std::string)>::iterator cmd;
typedef std::pair<std::string, bool (Server::*)(Client &, std::string)> functions;

bool Server::_signal = false;

Server::Server() {
	_cmds.insert(functions("PASS", &Server::pass));
	_cmds.insert(functions("NICK", &Server::nick));
	_cmds.insert(functions("JOIN", &Server::join));
	_cmds.insert(functions("HELP", &Server::help));
}

Server::Server( std::string port, std::string key ) : _port(static_cast<int>(std::strtol(port.c_str(), NULL, 10))), _key(key) {
	_cmds.insert(functions("PASS", &Server::pass));
	_cmds.insert(functions("NICK", &Server::nick));
	_cmds.insert(functions("JOIN", &Server::join));
	_cmds.insert(functions("HELP", &Server::help));
}

Server::Server( Server const & other ) {
	*this = other;
}

Server::~Server() {
	
}

Server & Server::operator=( Server const & rhs ) {
	if (this == &rhs)
		return *this;
	_port = rhs.getPort();
	_key = rhs.getKey();
	_socketFd = rhs.getSocketFd();
	_signal = rhs.getSignal();
	_clients = rhs._clients;
	_polls = rhs._polls;
	_channels = rhs._channels;
	_cmds = rhs._cmds;
	return *this;
}

int Server::getPort( void ) const {
	return _port;
}

std::string Server::getKey( void ) const {
	return _key;
}

int Server::getSocketFd( void ) const {
	return _socketFd;
}

bool Server::getSignal( void ) const {
	return _signal;
}

void Server::signalHandler( int signum ) {
	(void)signum;
	std::cout << "Signal Received!" << std::endl;
	_signal = true;
}

void Server::setupSocket( void ) {
	int value = 1;
	struct sockaddr_in addr;
	struct pollfd mypoll;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_port);
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
		throw std::runtime_error("server socket generation failed");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw std::runtime_error("setting option SO_REUSEADDR on server socket: FAILURE");
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("setting option O_NONBLOCK on server socket: FAILURE");
	if (bind(_socketFd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("binding server socket: FAILURE");
	if (listen(_socketFd, SOMAXCONN) == -1)
		throw std::runtime_error("server socket cannot accept any connections");
	mypoll.fd = _socketFd;
	mypoll.events = POLLIN;
	mypoll.revents = 0;
	_polls.push_back(mypoll);
}

void Server::setupServer( void ) {
	std::cout << "Setting server socket" << std::endl;
	setupSocket();
	std::cout << "Server socket initialization: SUCCESS" << std::endl;
	std::cout << "Server is waiting for connections" << std::endl;
	while (!_signal) {
		if (poll(&(*_polls.begin()), _polls.size(), -1) == -1 && !_signal)
			throw std::runtime_error("poll(): FAILURE");
		for (size_t i = 0; i < _polls.size(); i++) {
			if (_polls[i].revents & POLLIN) {
				if (_polls[i].fd == _socketFd)
					acceptClient();
				else
					receiveData(_polls[i].fd);
			}
		}
	}
	closePolls();
}

void Server::acceptClient( void ) {
	Client myClient;
	struct sockaddr_in clientAddr;
	struct pollfd myPoll;
	socklen_t len = sizeof(clientAddr);
	int clientFd = accept(_socketFd, (sockaddr *)&clientAddr, &len);
	if (clientFd == -1) {
		std::cout << "Client <" << clientFd << "> connection: FAILURE" << std::endl;
		return ;
	}
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		std::cout << "setting option O_NONBLOCK on new Client: FAILURE" << std::endl;
		return ;
	}
	myPoll.fd = clientFd;
	myPoll.events = POLLIN;
	myPoll.revents = 0;
	myClient.setFd(clientFd);
	myClient.setIpAddr(inet_ntoa(clientAddr.sin_addr));
	_clients.push_back(myClient);
	_polls.push_back(myPoll);
	std::cout << "Client <" << clientFd << "> connection: SUCCESS" << std::endl;
	send(myClient.getFd(), "Welcome!\nInsert server password to continue.\n", 46, 0);
}

void Server::receiveData( int fd ) {
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::vector<Client>::iterator cli = std::find(_clients.begin(), _clients.end(), fd);
	if (bytes <= 0) {
		std::cout << "Client <" << (*cli).getFd() << "> has Disconnected" << std::endl;
		removeClient(fd);
		close(fd);
	}
	else {
		buffer[bytes] = 0;
		std::string str = buffer;
		size_t i = 0;
		std::cout << "Client <" << (*cli).getFd() << "> sent this: '" << buffer << "'" << std::endl;
		while (buffer[i] && !std::isspace(buffer[i]))
			i++;
		if (_cmds.find(str.substr(0, i)) == _cmds.end()) {
			std::cout << str.substr(0, i) << " not found" << std::endl;
			return ;
		}
		else if (str.substr(0, i) == "HELP")
			help(*cli, buffer);
		else if ((*cli).getNick().empty()) {
			if ((*cli).getLogged() == false) {
				if (str.substr(0, i) == "PASS")
					pass(*cli, buffer);
				else {
					send(fd, "Please insert server password using 'PASS'\n", 44, 0);
					return ;
				}
			}
			else if (str.substr(0, i) == "NICK")
				nick(*cli, buffer);
			else {
					send(fd, "Please choose a nickname using 'NICK'\n", 39, 0);
					return ;
				}
		}
		else
			(this->*(_cmds[str.substr(0, i)]))(*cli, buffer);
	}
}

void Server::closePolls( void ) {
	for (size_t i = 0; i < _clients.size(); i++) {
		std::cout << "Client <" << _clients[i].getFd() << "> disconnection: SUCCESS" << std::endl;
		close(_clients[i].getFd());
	}
	if (_socketFd != -1) {
		std::cout << "Server <" << _socketFd << "> shutdown: SUCCESS" << std::endl;
		close(_socketFd);
	}
}

void Server::removeClient( int fd ) {
	for(size_t i = 0; i < _polls.size(); i++) {
		if (_polls[i].fd == fd) {
			_polls.erase(_polls.begin() + i);
			break ;
		}
	}
	for(size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getFd() == fd) {
			_clients.erase(_clients.begin() + i);
			break ;
		}
	}
}
