/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:35:41 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/12 16:02:07 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <Server.hpp>

typedef std::pair<std::string, bool (Server::*)(Client &, std::deque<std::string>)> functions;

bool Server::_signal = false;

Server::Server( std::string port, std::string key ) : _port(static_cast<int>(std::strtol(port.c_str(), NULL, 10))), _key(key) {
	_cmds.insert(functions("PASS", &Server::pass));
	_cmds.insert(functions("NICK", &Server::nick));
	_cmds.insert(functions("USER", &Server::user));
	_cmds.insert(functions("QUIT", &Server::quit));
	_cmds.insert(functions("PRIVMSG", &Server::privmsg));
	_cmds.insert(functions("JOIN", &Server::join));
	_cmds.insert(functions("MODE", &Server::mode));
	_cmds.insert(functions("KICK", &Server::kick));
	_cmds.insert(functions("INVITE", &Server::invite));
	_cmds.insert(functions("TOPIC", &Server::topic));
	_cmds.insert(functions("HELP", &Server::help));
	_cmds.insert(functions("PING", &Server::ping));
	_cmds.insert(functions("PONG", &Server::pong));
	_cmds.insert(functions("WHO", &Server::who));
	_cmds.insert(functions("USERHOST", &Server::userhost));
	_cmds.insert(functions("PART", &Server::part));
	_cmds.insert(functions("NAMES", &Server::names));
	_cmds.insert(functions("LIST", &Server::list));
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
	struct pollfd mypoll;
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) == -1)
		throw std::runtime_error("Hostname retrieval: FAILURE");
	struct hostent *host = gethostbyname(hostname);
	if (host == NULL)
		throw std::runtime_error("Host information retrieval: FAILURE");
	struct in_addr **addr_list = reinterpret_cast<struct in_addr **>(host->h_addr_list);
	if (addr_list[0] == NULL)
		throw std::runtime_error("No IP addresses found");
	_addr.sin_addr = *addr_list[0];
	std::cout << inet_ntoa(_addr.sin_addr) << std::endl;
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1)
		throw std::runtime_error("server socket generation failed");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) == -1)
		throw std::runtime_error("setting option SO_REUSEADDR on server socket: FAILURE");
	if (fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("setting option O_NONBLOCK on server socket: FAILURE");
	if (bind(_socketFd, reinterpret_cast<struct sockaddr *>(&_addr), sizeof(_addr)) == -1)
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
	int clientFd = accept(_socketFd, reinterpret_cast<sockaddr *>(&clientAddr), &len);
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
}

bool Server::clientLogin( Client & cli, std::deque<std::string> input ) {
	if (cli.getNickname().empty() || cli.getUsername().empty()) {
		if (cli.getLogged() == false) {
			if (input.front() == "PASS")
				pass(cli, input);
			else
				ft_sendMsg(cli.getFd(), ":server 300 " + cli.getNickname() + " :please insert server password using 'PASS' command");
			return false;
		}
		else if (input.front() == "NICK")
			nick(cli, input);
		else if (input.front() == "USER")
			user(cli, input);
		else
			ft_sendMsg(cli.getFd(), ":server 300 " + cli.getNickname() + " :please complete your registration using 'NICK' and 'USER' commands");
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
		std::deque<std::string> dq = ft_split(str);
		for (size_t i = 0; i < dq.size(); i++)
			std::cout << "dq[" << i << "]: '" << dq[i] << "'" << std::endl;
		while (!dq.empty()) {
			std::deque<std::string> input = ft_split(dq.front(), ' ');
			for (size_t i = 0; i < input.size(); i++)
				std::cout << "input[" << i << "]: '" << input[i] << "'" << std::endl;
			if (input.empty() || input.front().empty()) {
				dq.pop_front();
				continue ;
			}
			if (_cmds.find(input.front()) == _cmds.end()) {
				ft_sendMsg(fd, ":server 400 " + (*cli).getNickname() + " :command '" + input.front() + "' not found");
				dq.pop_front();
				continue ;
			}
			else if (input.front() == "HELP")
				help(*cli, input);
			else if (clientLogin(*cli, input) == false) {
				dq.pop_front();
				continue ;
			}
			else
				(this->*(_cmds[input.front()]))(*cli, input);
			dq.pop_front();
		}
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
	for (size_t i = 0; i < _channels.size(); i++)
		_channels[i].removeUser(cli);
	close(cli.getFd());
	for(size_t i = 0; i < _polls.size(); i++) {
		if (_polls[i].fd == cli.getFd()) {
			_polls.erase(_polls.begin() + i);
			break ;
		}
	}
	_clients.erase(std::find(_clients.begin(), _clients.end(), cli));
}
