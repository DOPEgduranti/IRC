/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:35:41 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 11:13:14 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <Server.hpp>

// typedef std::map<std::string, bool (Server::*)(Client &, std::deque<std::string>)>::iterator cmd;
typedef std::pair<std::string, bool (Server::*)(Client &, std::deque<std::string>)> functions;

bool Server::_signal = false;

Server::Server() {
	_cmds.insert(functions("PASS", &Server::pass));
	_cmds.insert(functions("NICK", &Server::nick));
	_cmds.insert(functions("USER", &Server::user));
	_cmds.insert(functions("QUIT", &Server::quit));
	_cmds.insert(functions("JOIN", &Server::join));
	_cmds.insert(functions("MODE", &Server::mode));
	_cmds.insert(functions("HELP", &Server::help));
}

Server::Server( std::string port, std::string key ) : _port(static_cast<int>(std::strtol(port.c_str(), NULL, 10))), _key(key) {
	_cmds.insert(functions("PASS", &Server::pass));
	_cmds.insert(functions("NICK", &Server::nick));
	_cmds.insert(functions("USER", &Server::user));
	_cmds.insert(functions("QUIT", &Server::quit));
	_cmds.insert(functions("JOIN", &Server::join));
	_cmds.insert(functions("MODE", &Server::mode));
	_cmds.insert(functions("HELP", &Server::help));
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
	ft_sendMsg(myClient.getFd(), "Server :Welcome!");
	ft_sendMsg(myClient.getFd(), "Server :Insert server password to continue.");
}

bool Server::clientLogin( Client & cli, std::deque<std::string> input ) {
	if (cli.getUsername().empty()) {
		if (cli.getNickname().empty()) {
			if (cli.getLogged() == false) {
				if (input.front() == "PASS")
					pass(cli, input);
				else
					ft_sendMsg(cli.getFd(), "Server :Please insert server password using 'PASS'");
				return false;
			}
			else if (input.front() == "NICK")
				nick(cli, input);
			else
				ft_sendMsg(cli.getFd(), "Server :Please choose a nickname using 'NICK'");
			return false;
		}
		else if (input.front() == "USER")
			user(cli, input);
		else
			ft_sendMsg(cli.getFd(), "Server :Please complete your login using 'USER'");
		return false;
	}
	return true;
}

void Server::receiveData( int fd ) {
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	std::vector<Client>::iterator cli = std::find(_clients.begin(), _clients.end(), fd);
	std::cout << "Client <" << (*cli).getFd() << "> sent this: '" << buffer << "'" << std::endl;
	if (bytes <= 0) {
		std::cout << "Client <" << (*cli).getFd() << "> has Disconnected" << std::endl;
		removeClient(*cli);
		close(fd);
	}
	else {
		buffer[bytes] = 0;
		std::string str = buffer;
		std::deque<std::string> input = ft_split(str, ' ');
		for (size_t i = 0; i < input.size(); i++)
			std::cout << "input[" << i << "]: '" << input[i] << "'" << std::endl;
		if (input.empty() || input.front().empty())
			return ;
		if (_cmds.find(input.front()) == _cmds.end()) {
			ft_sendMsg(fd, "Server :Command '" + input.front() + "' not found");
			return ;
		}
		else if (input.front() == "HELP")
			help(*cli, input);
		else if (clientLogin(*cli, input) == false)
			return ;
		else
			(this->*(_cmds[input.front()]))(*cli, input);
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

void Server::removeClient( Client & cli ) {
	close(cli.getFd());
	for(size_t i = 0; i < _polls.size(); i++) {
		if (_polls[i].fd == cli.getFd()) {
			_polls.erase(_polls.begin() + i);
			break ;
		}
	}
	if (std::find(_clients.begin(), _clients.end(), cli) != _clients.end()) {
		for (size_t i = 0; i < _channels.size(); i++)
			_channels[i].removeUser(cli);
		_clients.erase(std::find(_clients.begin(), _clients.end(), cli));
	}
	// for(size_t i = 0; i < _clients.size(); i++) {
	// 	if (_clients[i].getFd() == fd) {
	// 		_clients.erase(_clients.begin() + i);
	// 		break ;
	// 	}
	// }
}
