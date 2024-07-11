/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:57:24 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 15:55:36 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

# include <iostream>
# include <sys/socket.h>

// errors
// # define 461 ERR_NEEDMOREPARAMS "<command> :Not enough parameters" Returned by the server by numerous commands to indicate to the client that it didn’t supply enough parameters.
// # define ERR_ALREADYREGISTRED " :You may not reregister" //462 Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message)
// # define ERR_NONICKNAMEGIVEN " :No nickname given" //431 Returned when a nickname parameter expected for a command and isn’t found.
// # define ERR_NICKNAMEINUSE /*<nick>*/ " :Nickname is already in use" //433 Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
// # define ERR_ERRONEUSNICKNAME /*<nick>*/ " :Erroneus nickname" //432 Returned after receiving a NICK message which contains characters which do not fall in the defined set. See section x.x.x for details on valid nicknames.
# define ERR_NOTREGISTERED " :You have not registered" //Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOPRIVILEGES ":Permission Denied- You’re not an IRC operator" //Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_KEYSET /*<channel>*/ " :Channel key already set"
// # define ERR_TOOMANYCHANNELS /*<channel name>*/ " :You have joined too many channels" //Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
// # define ERR_CHANNELISFULL /*<channel>*/ " :Cannot join channel (+l)" //Return when a client attempt to join a channel already full.
# define ERR_UNKNOWNMODE /*<char>*/ " :is unknown mode char to me"
// # define ERR_INVITEONLYCHAN /*<channel>*/ " :Cannot join channel (+i)" //Return when a client attempt to join an invite only channel whitout invitation.
# define ERR_BANNEDFROMCHAN /*<channel>*/ " :Cannot join channel (+b)" //Return when a client attempt to join a channel from which he was banned.
// # define ERR_BADCHANNELKEY /*<channel>*/ " :Cannot join channel (+k)" //Return when a client attempt to join a channel using an invalid key.
# define ERR_CHANOPRIVSNEEDED /*<channel>*/ " :You’re not channel operator" //Any command requiring ’chanop’ privileges (such as MODE messages) must return this error if the client making the attempt is not a chanop on the specified channel.
// # define ERR_NORECIPIENT ":No recipient given " //(":No recipient given (<command>)") Return when you miss a target
// # define ERR_CANNOTSENDTOCHAN /*<channel name>*/ " :Cannot send to channel" //Sent to a user who is not on a channel and is trying to send a PRIVMSG message to that channel.
// # define ERR_NOSUCHNICK /*<nick>*/ " :No such nick/channel" //Used to indicate the nickname parameter supplied to a command is currently unused.
// # define ERR_NOTEXTTOSEND ":No text to send"//Return when a user is trying to send a PRIVMSG message and no message is provided

// errors.cpp

/*
	461 ERR_NEEDMOREPARAMS
	"<command> :Not enough parameters"
	- Returned by the server by numerous commands to
	indicate to the client that it didn’t supply enough
	parameters.
 */
void ERR_NEEDMOREPARAMS ( int fd, std::string nickname, std::string command );

/*
	462 ERR_ALREADYREGISTRED
	":You may not reregister"
	- Returned by the server to any link which tries to
	change part of the registered details (such as
	password or user details from second USER message).
 */
void ERR_ALREADYREGISTRED( int fd, std::string nickname );

/*
	431 ERR_NONICKNAMEGIVEN
	":No nickname given"
	- Returned when a nickname parameter expected for a
	command and isn’t found.
*/
void ERR_NONICKNAMEGIVEN( int fd, std::string nickname );

/*
	433 ERR_NICKNAMEINUSE
	"<nick> :Nickname is already in use"
	- Returned when a NICK message is processed that results
	in an attempt to change to a currently existing nickname.
*/
void ERR_NICKNAMEINUSE( int fd, std::string nickname );

/*
	432 ERR_ERRONEUSNICKNAME
	"<nick> :Erroneus nickname"
	- Returned after receiving a NICK message which contains
	characters which do not fall in the defined set. See
	section x.x.x for details on valid nicknames.
*/
void ERR_ERRONEUSNICKNAME( int fd, std::string nickname );

/*
	405 ERR_TOOMANYCHANNELS
	"<channel name> :You have joined too many channels"
	- Sent to a user when they have joined the maximum
	number of allowed channels and they try to join
	another channel.
*/
void ERR_TOOMANYCHANNELS( int fd, std::string nickname, std::string channelName );

/*
	471 ERR_CHANNELISFULL
	"<channel> :Cannot join channel (+l)"
	Return when a client attempt to join a channel
	already full.
*/
void ERR_CHANNELISFULL( int fd, std::string nickname, std::string channelName );

/*
	473 ERR_INVITEONLYCHAN
	"<channel> :Cannot join channel (+i)"
	Return when a client attempt to join an invite
	only channel whitout invitation.
*/
void ERR_INVITEONLYCHAN( int fd, std::string nickname, std::string channelName );

/*
	475 ERR_BADCHANNELKEY
	"<channel> :Cannot join channel (+k)"
	Return when a client attempt to join a channel
	using an invalid key.
*/
void ERR_BADCHANNELKEY( int fd, std::string nickname, std::string channelName );

/*
	411 ERR_NORECIPIENT
	":No recipient given (<command>)"
	Return when you miss a target
*/
void ERR_NORECIPIENT( int fd, std::string nickname, std::string command );

/*
	404 ERR_CANNOTSENDTOCHAN
	"<channel name> :Cannot send to channel"
	- Sent to a user who is not on a channel
	trying to send a PRIVMSG message to that channel.
*/
void ERR_CANNOTSENDTOCHAN( int fd, std::string nickname, std::string channelName );

/*
	401 ERR_NOSUCHNICK
	"<nickname> :No such nick/channel"
	- Used to indicate the nickname parameter supplied
	to a command is currently unused.
*/
void ERR_NOSUCHNICK( int fd, std::string nickname );

/*
	412 ERR_NOTEXTTOSEND
	":No text to send"
	Return when a user is trying to send a PRIVMSG
	message and no message is provided
*/
void ERR_NOTEXTTOSEND( int fd, std::string nickname );



// replyes.cpp

/*
	001	RPL_WELCOME
	":Welcome to the Internet Relay Network <nick>!<user>@<host>"
	- The first message sent after client registration. The text used
	varies widely
*/
void RPL_WELCOME( int fd, std::string nickname );

/*
	002	RPL_YOURHOST
	":Your host is <servername>, running version <version>"
	- Part of the post-registration greeting. Text varies widely
*/
void RPL_YOURHOST( int fd, std::string nickname );

/*
	003	RPL_CREATED
	":This server was created <date>"
	- Part of the post-registration greeting. Text varies widely
*/
void RPL_CREATED( int fd, std::string nickname );

/*
	004	RPL_MYINFO
	"<server_name> <version> <user_modes> <chan_modes>"
	- Part of the post-registration greeting
*/
void RPL_MYINFO( int fd, std::string nickname );

#endif