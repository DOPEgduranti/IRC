/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:52:42 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/02 10:36:04 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <Client.hpp>
# include <Channel.hpp>
# include <utils.hpp>

class Server {
private:
	std::string _name;
	int _port;
	std::string _key;
	int _socketFd;
	static bool _signal;
	std::vector<Client> _clients;
	std::vector<struct pollfd> _polls;
	std::vector<Channel> _channels;
	std::map<std::string, bool (Server::*)(Client &, std::deque<std::string>)> _cmds;
	Server( void ) { }
public:
	Server( std::string port, std::string key );
	Server( Server const & other ) { *this = other; }
	~Server( void ) { }
	
	Server & operator=( Server const & rhs );
	
	int getPort( void ) const { return _port; }
	std::string getKey( void ) const { return _key; }
	int getSocketFd( void ) const { return _socketFd; }
	bool getSignal( void ) const { return _signal; }

	static void signalHandler( int signum );
	
	void setupServer( void );
	void setupSocket( void );
	void acceptClient( void );
	bool clientLogin( Client & cli, std::deque<std::string> input );
	void receiveData( int fd );
	void closePolls( void );
	void removeClient( Client & cli );

	bool pass( Client & cli, std::deque<std::string> input );
	bool nick( Client & cli, std::deque<std::string> input );
	bool user( Client & cli, std::deque<std::string> input );
	bool quit( Client & cli, std::deque<std::string> input );

	bool privmsg( Client & cli, std::deque<std::string> input );
	
	bool join( Client & cli, std::deque<std::string> input );
	bool mode( Client & cli, std::deque<std::string> input );
	bool kick( Client & cli, std::deque<std::string> input );
	bool invite( Client & cli, std::deque<std::string> input );
	bool topic( Client & cli, std::deque<std::string> input );
	bool part( Client & cli, std::deque<std::string> input );
	
	bool help( Client & cli, std::deque<std::string> input );
	bool ping( Client & cli, std::deque<std::string> input );
	bool pong( Client & cli, std::deque<std::string> input );
	bool who( Client & cli, std::deque<std::string> input );
	bool userhost( Client & cli, std::deque<std::string> input );
};

#endif