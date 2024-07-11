/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:57:24 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/11 10:59:47 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

# include <iostream>

// errors
# define ERR_NEEDMOREPARAMS /*<command>*/" :Not enough parameters" //Returned by the server by numerous commands to indicate to the client that it didn’t supply enough parameters.
# define ERR_ALREADYREGISTRED " :You may not reregister" //Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message)
# define ERR_NONICKNAMEGIVEN ":No nickname given" //Returned when a nickname parameter expected for a command and isn’t found.
# define ERR_NICKNAMEINUSE /*<nick>*/ " :Nickname is already in use" //Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
# define ERR_ERRONEUSNICKNAME /*<nick>*/ " :Erroneus nickname" //Returned after receiving a NICK message which contains characters which do not fall in the defined set. See section x.x.x for details on valid nicknames.
# define ERR_NOTREGISTERED " :You have not registered" //Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NOPRIVILEGES ":Permission Denied- You’re not an IRC operator" //Any command requiring operator privileges to operate must return this error to indicate the attempt was unsuccessful.
# define ERR_KEYSET /*<channel>*/ " :Channel key already set"
# define ERR_TOOMANYCHANNELS /*<channel name>*/ " :You have joined too many channels" //Sent to a user when they have joined the maximum number of allowed channels and they try to join another channel.
# define ERR_CHANNELISFULL /*<channel>*/ " :Cannot join channel (+l)" //Return when a client attempt to join a channel already full.
# define ERR_UNKNOWNMODE /*<char>*/ " :is unknown mode char to me"
# define ERR_INVITEONLYCHAN /*<channel>*/ " :Cannot join channel (+i)" //Return when a client attempt to join an invite only channel whitout invitation.
# define ERR_BANNEDFROMCHAN /*<channel>*/ " :Cannot join channel (+b)" //Return when a client attempt to join a channel from which he was banned.
# define ERR_BADCHANNELKEY /*<channel>*/ " :Cannot join channel (+k)" //Return when a client attempt to join a channel using an invalid key.
# define ERR_CHANOPRIVSNEEDED /*<channel>*/ " :You’re not channel operator" //Any command requiring ’chanop’ privileges (such as MODE messages) must return this error if the client making the attempt is not a chanop on the specified channel.

// messages
# define MSG_QUITDEFAULT " :You disconnected from server" //Default Quit message
#endif