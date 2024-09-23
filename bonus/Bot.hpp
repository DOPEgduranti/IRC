/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 10:40:40 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/23 12:24:50 by gduranti         ###   ########.fr       */
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
	std::string _name;
	int _socketOut;
	int _socketIn;
	static bool _signal;
	std::vector<struct pollfd> _polls;
	std::string _serverIpAddr;
	int _serverPort;
	std::string _serverKey;
	std::vector<std::string> _futures;
	std::vector<std::string> _jokes;
	std::vector<std::string> _sentences;
	std::string _client;
	Bot( void ) {}
public:
	Bot( std::string IpAdd, std::string port, std::string key );
	~Bot( void ) {}
	
	static void signalHandler( int signum );
	
	void setupBot( void );
	void setupSocket( void );
	void connectServer( void );
	void receiveData( int fd );
	void closeBot( void );

	void answer( std::string str );
	void listUsers( std::string str );
	void joinChannel( std::string str );
	
	void fillSentences ( void );
	void fillJokes ( void );
	void fillFutures ( void );
	std::string tellFuture( std::string str );
};

#endif