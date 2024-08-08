/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/08 10:28:45 by gduranti         ###   ########.fr       */
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

void Channel::setInviteOnly( std::string & str ) {
	if (str == "-i")
		_inviteOnly = false;
	else if (str == "+i")
		_inviteOnly = true;
}

void Channel::setTopicRestricted( std::string & str ) {
	if (str == "-t")
		_topicRestricted = false;
	else if (str == "+t")
		_topicRestricted = true;
}

bool Channel::setKeyEnable( std::deque<std::string> input ) {
	if (input.front() == "-k") {
		_key.clear();
		_keyEnable = false;
	}
	else if (input.front() == "+k") {
		input.pop_front();
		if (input.empty())
			return false;
		_key = input.front();
		_keyEnable = true;
	}
	return true;
}

bool Channel::setUserLimit( std::deque<std::string> input ) {
	if (input.front() == "-l") {
		_maxUsers = 0;
		_userLimit = false;
	}
	else if (input.front() == "+l") {
		input.pop_front();
		if (input.empty() || validPositiveInteger(input.front()) < 1)
			return false;
		_userLimit = validPositiveInteger(input.front());
		_userLimit = true;
	}
	return true;
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

void Channel::manageOperator( Client & cli, std::string & str ) {
	if (str == "-o" && std::find(_operators.begin(), _operators.end(), cli) != _operators.end())
		_operators.erase(std::find(_operators.begin(), _operators.end(), cli));
	else if (str == "+o" && std::find(_users.begin(), _users.end(), cli) != _users.end())
		_operators.push_back(cli);
}

void Channel::removeUser( Client & cli ) {
	if (std::find(_users.begin(), _users.end(), cli) != _users.end())
		_users.erase(std::find(_users.begin(), _users.end(), cli));
	if (std::find(_operators.begin(), _operators.end(), cli) != _operators.end())
		_operators.erase(std::find(_operators.begin(), _operators.end(), cli));
}

bool Channel::userInvited( Client const & cli ) {
	if (std::find(_invites.begin(), _invites.end(), cli) != _invites.end())
		return true;
	return false;
}

void Channel::inviteUser( Client & cli ) {
	if (std::find(_invites.begin(), _invites.end(), cli) != _invites.end())
		return ;
	_invites.push_back(cli);
}

void Channel::broadcastMsg( Client & cli, std::string message ) {
	for (size_t i = 0; i < _users.size(); i++) {
		if (cli.getFd() != _users[i].getFd())
			ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + " PRIVMSG " + _name + " :" + message);
	}
}
