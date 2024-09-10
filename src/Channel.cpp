/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/10 12:54:20 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>
#include <msg.hpp>

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
	if (find(_users.begin(), _users.end(), cli) != _users.end())
		return ;
	if (std::find(_invites.begin(), _invites.end(), cli) != _invites.end())
		_invites.erase(std::find(_invites.begin(), _invites.end(), cli));
	_users.push_back(cli);
	if (_operators.empty())
		_operators.push_back(cli);
	for (size_t i = 0; i < _users.size(); i++)
		ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " JOIN " + _name);
	if (_topic.empty())
		RPL_NOTOPIC(cli.getFd(), cli.getNickname(), _name);
	else
		RPL_TOPIC(cli.getFd(), cli.getNickname(), _name, _topic);
	RPL_NAMREPLY(cli.getFd(), cli.getNickname(), *this);
	RPL_ENDOFNAMES(cli.getFd(), cli.getNickname(), _name);
	std::cout << _name << ": Client <" << cli.getFd() << "> joined the channel" << std::endl;
}

void Channel::manageOperator( std::string & nickname, std::string & str ) {
	if (str == "-o" && std::find(_operators.begin(), _operators.end(), nickname) != _operators.end())
		_operators.erase(std::find(_operators.begin(), _operators.end(), nickname));
	else if (str == "+o" && std::find(_users.begin(), _users.end(), nickname) != _users.end())
		_operators.push_back(*std::find(_users.begin(), _users.end(), nickname));
}

void Channel::removeUser( Client & cli ) {
	if (std::find(_users.begin(), _users.end(), cli) != _users.end()) {
		cli.quitChannel(*this);
		_users.erase(std::find(_users.begin(), _users.end(), cli));
	}
	if (std::find(_operators.begin(), _operators.end(), cli) != _operators.end())
		_operators.erase(std::find(_operators.begin(), _operators.end(), cli));
	if (_operators.empty() && !_users.empty())
		_operators.push_back(_users.front());
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
