/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 11:35:45 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel( void ) {
	
}

Channel::Channel( std::string name, std::string key ) : _name(name), _key(key) {
	
}

Channel::Channel( Channel const & other ) : _name(other._name), _key(other._key), _topic(other._topic), _maxUsers(other._maxUsers), _inviteOnly(other._inviteOnly), _topicRestricted(other._topicRestricted), _keyEnable(other._keyEnable), _userLimit(other._userLimit) {
	_users.insert(_users.end(), other._users.begin(), other._users.end());
}

Channel::~Channel() {
	
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
	_users.insert(_users.end(), rhs._users.begin(), rhs._users.end());
	return *this;
}
