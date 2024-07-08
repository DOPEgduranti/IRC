/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:54:21 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/08 12:37:14 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Client.hpp>

Client::Client( void ) : _nickname(""), _username("") , _logged(false) {
	
}

Client & Client::operator=( Client const & rhs ) {
	_fd = rhs._fd;
	_ipAddr = rhs._ipAddr;
	_nickname = rhs._nickname;
	_username = rhs._username;
	_hostname = rhs._hostname;
	_servername = rhs._servername;
	_realname = rhs._realname;
	_logged = rhs._logged;
	_channels = rhs._channels;
	return *this;
}

bool Client::operator==( int const & fd ) const {
	if (_fd == fd)
		return true;
	return false;
}

