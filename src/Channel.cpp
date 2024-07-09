/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/09 12:35:33 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel( std::string name, std::string key ) : _name(name) {
	if (key.empty())
		_key = "";
	else
		_key = key;
}

Channel & Channel::operator=( Channel const & rhs ) {
	_name = rhs._name;
	_key = rhs._key;
	_topic = rhs._topic;
	_maxUsers = rhs._maxUsers;
	_inviteOnly = rhs._inviteOnly;
	_topicRestricted = rhs._topicRestricted;
	_keyEnable = rhs._keyEnable;
	_userLimit = rhs._userLimit;
	_users = rhs._users;
	return *this;
}

bool Channel::operator==( Channel const & other ) const {
	if (_name == other.getName())
		return true;
	return false;
}

bool Channel::operator==( std::string const & str ) const {
	if (_name == str)
		return true;
	return false;
}

void Channel::addUser( Client & cli ) {
	std::vector<Client>::iterator it = find(_users.begin(), _users.end(), cli);
	if (it != _users.end())
		return ;
	_users.push_back(cli);
	std::cout << _name << ": Client <" << cli.getFd() << "> joined the channel" << std::endl;
}
