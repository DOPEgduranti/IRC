/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:52:44 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/04 11:53:56 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <utils.hpp>
#include <Channel.hpp>

# define MAX_JOINABLECHANNELS 3

class Channel;

class Client {
private:
	int _fd;
	std::string _ipAddr;
	std::string _nickname;
	std::string _username;
	std::string _hostname;
	std::string _servername;
	std::string _realname;
	bool _logged;
	std::vector<Channel> _channels;
public:
	Client( void );
	Client( Client const & other ) { *this = other; }
	~Client( void ) {}
	
	Client & operator=( Client const & rhs );
	bool operator==( Client const & other ) const;
	bool operator==( int const & fd ) const;
	bool operator==( std::string const & nick ) const;

	int getFd( void ) const { return _fd; }
	std::string getIpAddr( void ) const { return _ipAddr; }
	std::string getUsername( void ) const { return _username; }
	std::string getNickname( void ) const { return _nickname; }
	std::string getRealName( void ) const { return _realname; }
	std::string getHostName( void ) const { return _hostname; }
	bool getLogged( void ) const { return _logged; }
	int getChannelNbr( void ) const { return _channels.size(); }
	std::vector<Channel> const & getChannels( void ) const { return _channels; }
	
	void setFd( int fd ) { _fd = fd; }
	void setIpAddr( std::string ipAdd ) { _ipAddr = ipAdd; }
	void setNickname( std::string nickname ) { _nickname = nickname; }
	void setUser( std::string username, std::string hostname, std::string servername, std::string realname );
	void quitChannel( Channel & chan ) { _channels.erase(std::find(_channels.begin(), _channels.end(), chan)); }

	void login( void ) { _logged = true; }
	bool joinChannel( Channel & chan, std::string key );
};

#endif