/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:10:26 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/10 16:11:39 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <utils.hpp>
# include <Client.hpp>

class Client;
class Channel {
private:
	std::string _name;
	std::string _key;
	std::string _topic;
	int _maxUsers;
	std::vector<Client> _users;
	std::vector<Client> _invites;
	std::vector<Client> _operators;
	bool _inviteOnly;
	bool _topicRestricted;
	bool _keyEnable;
	bool _userLimit;
	Channel( void ) { }
public:
	Channel( std::string name, std::string key );
	Channel( Channel const & other ) { *this = other; }
	~Channel( void ) { }

	Channel & operator=( Channel const & rhs );
	bool operator==( Channel const & other ) const;
	bool operator==( std::string const & str ) const;

	std::string getName( void ) const { return _name; }
	std::string getKey( void ) const { return _key; }
	std::string getTopic( void ) const { return _topic; }
	int getMaxUsers( void ) const { return _maxUsers; }
	bool getIviteOnly( void ) const { return _inviteOnly; }
	bool getTopicRestricted( void ) const { return _topicRestricted; }
	bool getKeyEnable( void ) const { return _keyEnable; }
	bool getUserLimit( void ) const { return _userLimit; }

	void addUser( Client & cli );
	void addOperator( Client & cli );
	void removeUser( Client & cli );
	bool userInvited( Client const & cli );
};

#endif