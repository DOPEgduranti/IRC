/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:10:36 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/21 10:35:01 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <msg.hpp>

void RPL_WELCOME( int fd, std::string nickname ) {
	std::string message = ":server 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_YOURHOST( int fd, std::string nickname ) {
	std::string message = ":server 002 " + nickname + " :Your host is server, running version 0.1\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_CREATED( int fd, std::string nickname ) {
	std::string message = ":server 003 " + nickname + " :This server was created 2024 Jul 11\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_MYINFO( int fd, std::string nickname ) {
	std::string message = ":server 004 " + nickname + " :server 0.1 -o -o\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_INVITING( int fd, std::string userNick, std::string channel, std::string nickname ) {
	std::string message = ":server 341 " + userNick + " " + channel + " " + nickname + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_NOTOPIC( int fd, std::string nickname, std::string channel ) {
	std::string message = ":server 331 " + nickname + " " + channel + " :No topic is set\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_TOPIC( int fd, std::string nickname, std::string channel, std::string topic ) {
	std::string message = ":server 332 " + nickname + " " + channel + " :" + topic + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}