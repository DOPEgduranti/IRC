/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:59:38 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/16 12:50:17 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msg.hpp>

void ERR_NEEDMOREPARAMS ( int fd, std::string nickname, std::string command ) {
	std::string message = ":server 461 " + nickname + " " + command + "  :Not enough parameters\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_ALREADYREGISTRED ( int fd, std::string nickname ) {
	std::string message = ":server 462 " + nickname + " :Not enough parameters\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NONICKNAMEGIVEN( int fd, std::string nickname ) {
	std::string message = ":server 431 " + nickname + " :No nickname given\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NICKNAMEINUSE( int fd, std::string userNick, std::string nickname ) {
	std::string message = ":server 433 " + userNick + " " + nickname + " :Nickname is already in use\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_ERRONEUSNICKNAME( int fd, std::string userNick, std::string nickname ) {
	std::string message = ":server 432 " + userNick + " " + nickname + " :Erroneus nickname\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_TOOMANYCHANNELS( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 405 " + nickname + " " + channelName + " :You have joined too many channels\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_CHANNELISFULL( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 471 " + nickname + " " + channelName + " :Cannot join channel (+l)\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_INVITEONLYCHAN( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 473 " + nickname + " " + channelName + " :Cannot join channel (+i)\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_BADCHANNELKEY( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 475 " + nickname + " " + channelName + " :Cannot join channel (+k)\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NORECIPIENT( int fd, std::string nickname, std::string command ) {
	std::string message = ":server 411 " + nickname + " :No recipient given (" + command + ")\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_CANNOTSENDTOCHAN( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 404 " + nickname + " " + channelName + " :Cannot send to channel\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NOSUCHNICK( int fd, std::string userNick, std::string nickname ) {
	std::string message = ":server 401 " + userNick + " " + nickname + " :No such nick/channel\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NOTEXTTOSEND( int fd, std::string nickname ) {
	std::string message = ":server 412 " + nickname + " :No text to send\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NOTONCHANNEL (int fd, std::string nickname, std::string channelName) {
	std::string message = ":server 442 " + nickname + " " + channelName + " :You’re not on that channel\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_UNKNOWNMODE( int fd, std::string nickname, std::string c ) {
	std::string message = ":server 472 " + nickname + " " + c + " :is unknown mode char to me\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NOSUCHCHANNEL( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 403 " + nickname + " " + channelName + " :No such channel\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_CHANOPRIVSNEEDED( int fd, std::string nickname, std::string channelName ) {
	std::string message = ":server 482 " + nickname + " " + channelName + " :You’re not channel operator\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}
