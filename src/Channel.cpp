/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/01 12:17:56 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel( std::string name, std::string key ) : _name(name), _maxUsers(0), _inviteOnly(false), _topicRestricted(false), _userLimit(false) {
	if (key.empty()) {
		_keyEnable = false;
		_key = "";
	}
	else {
		_keyEnable = true;
		_key = key;
	}
}

Channel & Channel::operator=( Channel const & rhs ) {
	_name = rhs._name;
	_key = rhs._key;
	_topic = rhs._topic;
	_maxUsers = rhs._maxUsers;
	_users = rhs._users;
	_invites = rhs._invites;
	_operators = rhs._operators;
	_inviteOnly = rhs._inviteOnly;
	_topicRestricted = rhs._topicRestricted;
	_keyEnable = rhs._keyEnable;
	_userLimit = rhs._userLimit;
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
	if (std::find(_invites.begin(), _invites.end(), cli) != _invites.end())
		_invites.erase(std::find(_invites.begin(), _invites.end(), cli));
	_users.push_back(cli);
	if (_users.size() == 1)
		_operators.push_back(cli);
	if (_topic.empty())
		ft_sendMsg(cli.getFd(), ":server 331 " + cli.getNickname() + " " + _name + " :No topic is set");
	else
		ft_sendMsg(cli.getFd(), ":server 332 " + cli.getNickname() + " " + _name + " :" + _topic);
	std::string mess = ":server 353 " + cli.getNickname() + " = " + _name + " :" ;
	for (size_t i = 0; i < _users.size(); i++) {
		if (std::find(_operators.begin(), _operators.end(), _users[i]) != _operators.end())
			mess += "@";
		mess += _users[i].getNickname();
		if (i + 1 < _users.size())
			mess += " ";
	}
	ft_sendMsg(cli.getFd(), mess);
	ft_sendMsg(cli.getFd(), ":server 366 " + cli.getNickname() + " " + _name + " :End of /NAMES list");
	for (size_t i = 0; i < _users.size(); i++)
		ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + " JOIN :" + _name);
	std::cout << _name << ": Client <" << cli.getFd() << "> joined the channel" << std::endl;
}

void Channel::removeUser( Client & cli ) {
	if (std::find(_users.begin(), _users.end(), cli) != _users.end())
		_users.erase(std::find(_users.begin(), _users.end(), cli));
}

bool Channel::userInvited( Client const & cli ) {
	if (std::find(_invites.begin(), _invites.end(), cli) != _invites.end())
		return true;
	return false;
}
