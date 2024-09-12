/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:40:40 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/12 12:13:19 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <vector> //-> for vector
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket()
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()
# include <cstring>
# include <algorithm>
# include <map>
# include <fstream>
# include <sstream>
# include <cstdlib>
# include <deque>
# include <climits>
# include <netdb.h>
# include <errno.h>

class Bot {
private:
	int _port;
	struct sockaddr_in _addr;
	int _socketFd;
	static bool _signal;
	std::vector<struct pollfd> _polls;
	int _serverFd;
	std::string _serverIpAddr;
	int _serverPort;
	Bot( void ) {}
public:
	Bot( std::string IpAdd, std::string port ) : _serverIpAddr(IpAdd), _serverPort(static_cast<int>(std::strtol(port.c_str(), NULL, 10))) {}
	Bot ( Bot const & other ) { *this = other; }
	~Bot( void ) {}
	
	Bot & operator=( Bot const & rhs );
	
	static void signalHandler( int signum );
	
	void setupBot( void );
	void setupSocket( void );
	void connectServer( void );
	void receiveData( int fd );
	void closeBot( void );
};

#endif