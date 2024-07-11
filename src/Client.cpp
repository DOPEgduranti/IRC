/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 15:54:21 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 10:19:09 by gduranti         ###   ########.fr       */
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

bool Client::operator==( Client const & other ) const {
	if (_fd != other._fd)
		return false;
	if (_nickname != other._nickname)
		return false;
	return true;
}

bool Client::operator==( int const & fd ) const {
	if (_fd == fd)
		return true;
	return false;
}

void Client::setUser( std::string username, std::string hostname, std::string servername, std::string realname ) {
	_username = username;
	_hostname = hostname;
	_servername = servername;
	_realname = realname;
}

bool Client::joinChannel( Channel & chan, std::string key ) {
	std::vector<Channel>::iterator it = std::find(_channels.begin(), _channels.end(), chan);
	if (it != _channels.end()) {
		ft_sendMsg(_fd, chan.getName() + ": you already joined this channel");
		return true;
	}
	if (chan.getUserLimit() && chan.getMaxUsers() == chan.getUsersNbr()) {
		ft_sendMsg(_fd, chan.getName() + ERR_CHANNELISFULL);
		return false;
	}
	if (chan.getIviteOnly() && !chan.userInvited(*this)) {
		ft_sendMsg(_fd, chan.getName() + ERR_INVITEONLYCHAN);
		return false;
	}
	if (chan.getKeyEnable() && chan.getKey() != key) {
		ft_sendMsg(_fd, chan.getName() + ERR_BADCHANNELKEY);
		return false;
	}
	ft_sendMsg(_fd, chan.getName() + ": you joined the channel");
	_channels.push_back(chan);
	chan.addUser(*this);
	return true;
}
