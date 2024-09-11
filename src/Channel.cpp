/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/11 12:35:14 by gduranti         ###   ########.fr       */
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

void Channel::setInviteOnly( std::string & str, Client const & cli ) {
	if (str == "-i")
		_inviteOnly = false;
	else if (str == "+i")
		_inviteOnly = true;
	for (size_t i = 0; i < _users.size(); i++)
			ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " " + str);
}

void Channel::setTopicRestricted( std::string & str, Client const & cli ) {
	if (str == "-t")
		_topicRestricted = false;
	else if (str == "+t")
		_topicRestricted = true;
	for (size_t i = 0; i < _users.size(); i++)
			ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " " + str);
}

bool Channel::setKeyEnable( std::deque<std::string> input, Client const & cli ) {
	std::string flag = input.front();
	std::string message = ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " " + flag;
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
		message += " " + input.front();
	}
	for (size_t i = 0; i < _users.size(); i++)
		ft_sendMsg(_users[i].getFd(), message);
	return true;
}

bool Channel::setUserLimit( std::deque<std::string> input, Client const & cli ) {
	std::string flag = input.front();
	std::string message = ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " " + flag;
	if (input.front() == "-l") {
		_maxUsers = 0;
		_userLimit = false;
	}
	else if (input.front() == "+l") {
		input.pop_front();
		if (input.empty() || validPositiveInteger(input.front()) < 1)
			return false;
		_maxUsers = validPositiveInteger(input.front());
		_userLimit = true;
		message += " " + input.front();
	}
	for (size_t i = 0; i < _users.size(); i++)
		ft_sendMsg(_users[i].getFd(), message);
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
	RPL_CHANNELMODEIS(cli.getFd(), cli.getNickname(), *this);
	std::cout << _name << ": Client <" << cli.getFd() << "> joined the channel" << std::endl;
}

void Channel::manageOperator( std::string & nickname, std::string & str, Client const & cli ) {
	if (str == "-o" && std::find(_operators.begin(), _operators.end(), nickname) != _operators.end())
		_operators.erase(std::find(_operators.begin(), _operators.end(), nickname));
	else if (str == "+o" && std::find(_users.begin(), _users.end(), nickname) != _users.end())
		_operators.push_back(*std::find(_users.begin(), _users.end(), nickname));
	for (size_t i = 0; i < _users.size(); i++)
		ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " " + str + " " + nickname);
	if (_operators.empty()) {
		_operators.push_back(_users.front());
		for (size_t i = 0; i < _users.size(); i++)
			ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " +o " + nickname);
	}
}

void Channel::removeUser( Client & cli ) {
	if (std::find(_users.begin(), _users.end(), cli) == _users.end())
		return ;
	_users.erase(std::find(_users.begin(), _users.end(), cli));
	if (std::find(_operators.begin(), _operators.end(), cli) != _operators.end())
		_operators.erase(std::find(_operators.begin(), _operators.end(), cli));
	cli.quitChannel(*this);
	if (_operators.empty() && !_users.empty()) {
		_operators.push_back(_users.front());
		for (size_t i = 0; i < _users.size(); i++)
			ft_sendMsg(_users[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " MODE " + _name + " +o " + _users.front().getNickname());
	}
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
