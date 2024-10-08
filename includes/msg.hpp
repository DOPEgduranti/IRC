/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:57:24 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/16 12:48:36 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

# include <iostream>
# include <Channel.hpp>

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
void ERR_NICKNAMEINUSE( int fd, std::string userNick, std::string nickname );

/*
	432 ERR_ERRONEUSNICKNAME
	"<nick> :Erroneus nickname"
	- Returned after receiving a NICK message which contains
	characters which do not fall in the defined set. See
	section x.x.x for details on valid nicknames.
*/
void ERR_ERRONEUSNICKNAME( int fd, std::string userNick, std::string nickname );

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
void ERR_NOSUCHNICK( int fd, std::string userNick, std::string nickname );

/*
	412 ERR_NOTEXTTOSEND
	":No text to send"
	- Return when a user is trying to send a PRIVMSG
	message and no message is provided
*/
void ERR_NOTEXTTOSEND( int fd, std::string nickname );

/*
	442 ERR_NOTONCHANNEL
	"<channel> :You’re not on that channel"
	- Returned by the server whenever a client tries to
	perform a channel effecting command for which the
	client isn’t a member.
*/
void ERR_NOTONCHANNEL( int fd, std::string nickname, std::string channelName );

/*
	472 ERR_UNKNOWNMODE
	"<char> :is unknown mode char to me"
	- Returned when a client tries to use MODE command
	passing an unknown mode character
*/
void ERR_UNKNOWNMODE( int fd, std::string nickname, std::string c );

/*
	403 ERR_NOSUCHCHANNEL
	"<channel name> :No such channel"
	- Used to indicate the given channel name is invalid.
*/
void ERR_NOSUCHCHANNEL( int fd, std::string nickname, std::string channelName );

/*
	482 ERR_CHANOPRIVSNEEDED
	"<channel> :You’re not channel operator"
	- Any command requiring ’chanop’ privileges (such as
	MODE messages) must return this error if the client
	making the attempt is not a chanop on the specified channel.
*/
void ERR_CHANOPRIVSNEEDED( int fd, std::string nickname, std::string channelName );




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

/*
	331 RPL_NOTOPIC
	"<channel> :No topic is set"
*/
void RPL_NOTOPIC( int fd, std::string nickname, std::string channel );

/*
	332 RPL_TOPIC
	"<channel> :<topic>"
	- When sending a TOPIC message to determine the
	channel topic, one of two replies is sent. If the
	topic is set, RPL_TOPIC is sent back else RPL_NOTOPIC.
*/
void RPL_TOPIC( int fd, std::string nickname, std::string channel, std::string topic );

/*
	341 RPL_INVITING
	"<channel> <nick>"
	- Returned by the server to indicate that the
	attempted INVITE message was successful and is
	being passed onto the end client.
*/
void RPL_INVITING( int fd, std::string userNick, std::string channel, std::string nickname );

/*
	352 RPL_WHOREPLY
    "<channel> <user> <host> <server> <nick> \
    <H|G>[*][@|+] :<hopcount> <real name>"
*/
void RPL_WHOREPLY( int fd, std::string userNick, Channel const & chan ); void RPL_WHOREPLY( int fd, std::string userNick, std::string channel, Client const & cli );

/*
    315 RPL_ENDOFWHO
    "<name> :End of /WHO list"
    - The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
    to answer a WHO message.  The RPL_WHOREPLY is only
    sent if there is an appropriate match to the WHO
    query.  If there is a list of parameters supplied
    with a WHO message, a RPL_ENDOFWHO must be sent
    after processing each list item with <name> being
    the item.
*/
void RPL_ENDOFWHO( int fd, std::string userNick, std::string name );

/*
	353 RPL_NAMREPLY
    "<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	- Return a list of users and operators in a channel
*/
void RPL_NAMREPLY( int fd, std::string nickname, Channel const & chan );

/*
	366 RPL_ENDOFNAMES
    "<channel> :End of /NAMES list"
    - To reply to a NAMES message, a reply pair consisting
    of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
    server back to the client.  If there is no channel
    found as in the query, then only RPL_ENDOFNAMES is
    returned.  The exception to this is when a NAMES
    message is sent with no parameters and all visible
    channels and contents are sent back in a series of
    RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
    the end.
*/
void RPL_ENDOFNAMES( int fd, std::string nickname, std::string chan );

/*
	324 RPL_CHANNELMODEIS
    "<channel> <mode> <mode params>"
*/
void RPL_CHANNELMODEIS( int fd, std::string nickname, Channel const & chan );

/*
	321 RPL_LISTSTART
    "Channel :Users  Name"
*/
void RPL_LISTSTART( int fd, std::string nickname);

/*
    322 RPL_LIST
    "<channel> <# visible> :<topic>"
*/
void RPL_LIST( int fd, std::string nickname, std::string channel, size_t visible, std::string topic);

/*
    323 RPL_LISTEND
    ":End of /LIST"
    - Replies RPL_LISTSTART, RPL_LIST, RPL_LISTEND mark
    the start, actual replies with data and end of the
    server’s response to a LIST command.  If there are
    no channels available to return, only the start
    and end reply must be sent.
*/
void RPL_LISTEND( int fd, std::string nickname );

/*
	311 RPL_WHOISUSER
    "<nick> <user> <host> * :<real name>"
	- Replies 311 - 313, 317 - 319 are all replies
    generated in response to a WHOIS message.  Given that
    there are enough parameters present, the answering
    server must either formulate a reply out of the above
    numerics (if the query nick is found) or return an
    error reply.  The ’*’ in RPL_WHOISUSER is there as
    the literal character and not as a wild card.  For
    each reply set, only RPL_WHOISCHANNELS may appear
    more than once (for long lists of channel names).
    The ’@’ and ’+’ characters next to the channel name
    indicate whether a client is a channel operator or
    has been granted permission to speak on a moderated
    channel.  The RPL_ENDOFWHOIS reply is used to mark
    the end of processing a WHOIS message.
*/
void RPL_WHOISUSER( int fd, std::string userNick, Client const & cli );

/*
	312 RPL_WHOISSERVER
    "<nick> <server> :<server info>"
*/
void RPL_WHOISSERVER( int fd, std::string userNick, std::string nickname );

/*
	319 RPL_WHOISCHANNELS
    "<nick> :{[@|+]<channel><space>}"
	- Sent as a reply to the WHOIS command, this numeric lists 
	the channels that the client with the nickname <nick> is joined 
	to and their status in these channels. <prefix> is the highest 
	channel membership prefix that the client has in that channel, 
	if the client has one. <channel> is the name of a channel that 
	the client is joined to. The last parameter of this numeric is a 
	list of [prefix]<channel> pairs, delimited by a SPACE character (' ', 0x20).
*/
void RPL_WHOISCHANNELS( int fd, std::string userNick, Client const & cli );

/*
	318 RPL_ENDOFWHOIS
    "<nick> :End of /WHOIS list"
*/
void RPL_ENDOFWHOIS( int fd, std::string userNick, std::string nickname );

#endif