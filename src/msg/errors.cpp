/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:59:38 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/01 11:08:57 by gduranti         ###   ########.fr       */
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

void ERR_NICKNAMEINUSE( int fd, std::string nickname ) {
	std::string message = ":server 433 " + nickname + " " + nickname + " :Nickname is already in use\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_ERRONEUSNICKNAME( int fd, std::string nickname ) {
	std::string message = ":server 432 " + nickname + " " + nickname + " :Erroneus nickname\r\n" ;
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
	std::string message = ":server 471 " + nickname + " " + channelName + " :Cannot join channel (+i)\r\n" ;
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

void ERR_NOSUCHNICK( int fd, std::string nickname ) {
	std::string message = ":server 401 " + nickname + " " + nickname + " :No such nick/channel\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}

void ERR_NOTEXTTOSEND( int fd, std::string nickname ) {
	std::string message = ":server 412 " + nickname + " :No text to send\r\n" ;
	send(fd, message.c_str(), message.size(), 0);
}
