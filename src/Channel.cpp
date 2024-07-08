/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:28:27 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/08 12:32:42 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Channel.hpp>

Channel::Channel( std::string name, std::string key ) : _name(name), _key(key) {
	
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
