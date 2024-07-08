/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 11:52:44 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 15:04:14 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <utils.hpp>

class Client {
private:
	int _fd;
	std::string _ipAddr;
	std::string _nick;
	bool _logged;
	std::vector<std::string> _channels;
public:
	Client( void );
	Client( Client const & other );
	~Client( void );
	
	Client & operator=( Client const & rhs );
	bool operator==( int const & fd ) const;

	int getFd( void ) const { return _fd; }
	std::string getIpAddr( void ) const { return _ipAddr; }
	std::string getNick( void ) const { return _nick; }
	bool getLogged( void ) const { return _logged; }
	
	void setFd( int fd ) { _fd = fd; }
	void setIpAddr( std::string ipAdd ) { _ipAddr = ipAdd; }
	void setNick( std::string nickname ) { _nick = nickname; }

	void login() { _logged = true; }
};

#endif