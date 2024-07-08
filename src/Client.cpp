/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:54:21 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 10:57:09 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client( void ) : _nick(""), _logged(false) {
	
}

Client::Client( Client const & other ) : _fd(other._fd), _ipAddr(other._ipAddr), _nick(other._nick), _logged(other._logged) {
	_channels.insert(_channels.end(), other._channels.begin(), other._channels.end());
}

Client::~Client( void ) {
	
}

Client & Client::operator=( Client const & rhs ) {
	_fd = rhs._fd;
	_ipAddr = rhs._ipAddr;
	_nick = rhs._nick;
	_logged = rhs._logged;
	_channels.insert(_channels.end(), rhs._channels.begin(), rhs._channels.end());
	return *this;
}

bool Client::operator==( int const & fd ) const {
	if (_fd == fd)
		return true;
	return false;
}

