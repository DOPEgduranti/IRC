/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:52:42 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 15:27:49 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <Client.hpp>
# include <Channel.hpp>

class Server {
private:
	int _port;
	std::string _key;
	int _socketFd;
	static bool _signal;
	std::vector<Client> _clients;
	std::vector<struct pollfd> _polls;
	std::vector<Channel> _channels;
	std::map<std::string, bool (Server::*)(Client &, std::string)> _cmds;
	Server( void );
public:
	Server( std::string port, std::string key );
	Server( Server const & other );
	~Server( void );
	
	Server & operator=( Server const & rhs );
	
	int getPort( void ) const;
	std::string getKey( void ) const;
	int getSocketFd( void ) const;
	bool getSignal( void ) const;

	static void signalHandler( int signum );
	
	void setupServer( void );
	void setupSocket( void );
	void acceptClient( void );
	void receiveData( int fd );

	bool pass( Client & cli, std::string str );
	bool nick( Client & cli, std::string str );
	bool join( Client & cli, std::string str );
	bool help( Client & cli, std::string str );
	
	void closePolls( void );
	void removeClient( int fd );
};

#endif