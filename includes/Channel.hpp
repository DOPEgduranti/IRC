/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:10:26 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/08 12:32:29 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <utils.hpp>
# include <Client.hpp>

class Channel {
private:
	std::string _name;
	std::string _key;
	std::string _topic;
	int _maxUsers;
	std::vector<Client> _users;
	bool _inviteOnly;
	bool _topicRestricted;
	bool _keyEnable;
	bool _userLimit;
public:
	Channel( void ) { }
	Channel( std::string name, std::string key );
	Channel( Channel const & other ) { *this = other; }
	~Channel( void ) { }

	Channel & operator=( Channel const & rhs );

	std::string getName( void ) const { return _name; }
	std::string getKey( void ) const { return _key; }
	std::string getTopic( void ) const { return _topic; }
	int getMaxUsers( void ) const { return _maxUsers; }
	bool getIviteOnly( void ) const { return _inviteOnly; }
	bool getTopicRestricted( void ) const { return _topicRestricted; }
	bool getKeyEnable( void ) const { return _keyEnable; }
	bool getUserLimit( void ) const { return _userLimit; }

	
};

#endif