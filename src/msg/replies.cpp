/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:10:36 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/16 12:14:09 by gduranti         ###   ########.fr       */
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

void RPL_NAMREPLY( int fd, std::string nickname, Channel const & chan ) {
	std::string message = ":server 353 " + nickname + " = " + chan.getName() + " :";
	for (size_t i = 0; i < chan.getUsers().size(); i++) {
		if (std::find(chan.getOperators().begin(), chan.getOperators().end(), chan.getUsers()[i]) != chan.getOperators().end())
			message += "@";
		message += chan.getUsers()[i].getNickname();
		if (i + 1 < chan.getUsers().size())
			message += " ";
	}
	message += "\r\n";
	std::cout << message;
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_ENDOFNAMES( int fd, std::string nickname, std::string chan ) {
	std::string message = ":server 366 " + nickname + " " + chan + " :End of /NAMES list\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_CHANNELMODEIS( int fd, std::string nickname, Channel const & chan ) {
	std::string message = ":server 324 " + nickname + " " + chan.getName() + " +";
	if (chan.getKeyEnable())
		message += "k";
	if (chan.getUserLimit())
		message += "l";
	if (chan.getIviteOnly())
		message += "i";
	if (chan.getTopicRestricted())
		message += "t";
	if (chan.getKeyEnable())
		message += " " + chan.getKey();
	if (chan.getUserLimit())
		message += " " + numberToString(chan.getMaxUsers());
	if (message == ":server 324 " + nickname + " " + chan.getName() + " +")
		message = ":server 324 " + nickname + " " + chan.getName() + " none";
	message += "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_WHOREPLY( int fd, std::string userNick, Channel const & chan ) {
	for (size_t i = 0; i < chan.getUsers().size(); i++)
		RPL_WHOREPLY(fd, userNick, chan.getName(), chan.getUsers()[i]);
}

void RPL_WHOREPLY( int fd, std::string userNick, std::string channel, Client const & cli ) {
	std::string message = ":server 352 " + userNick + " " + channel + " " + cli.getUsername() + " " + cli.getHostName() + " server " + cli.getNickname() + " H";
	if (cli.getRealName() == "Hi, I am a bot!")
		message += 'b';
	message += " :0 " + cli.getRealName() + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_ENDOFWHO( int fd, std::string userNick, std::string name ) {
	std::string message = ":server 315 " + userNick + " " + name + " ::End of /WHO list\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_LISTSTART( int fd, std::string nickname) {
	std::string message = ":server 321 " + nickname + " :Channel :Users Name\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_LIST( int fd, std::string nickname, std::string channel, size_t visible, std::string topic) {
	std::string message = ":server 322 " + nickname + " " + channel + " " + numberToString(visible) + " :" + topic + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_LISTEND( int fd, std::string nickname ) {
	std::string message = ":server 323 " + nickname + " :End of /LIST\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_WHOISUSER( int fd, std::string userNick, Client const & cli ) {
	std::string message = ":server 311 " + userNick + " " + cli.getNickname() + " " + cli.getUsername() + " " + cli.getHostName() + " * :" + cli.getRealName() + "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_WHOISSERVER( int fd, std::string userNick, std::string nickname ) {
	std::string message = ":server 312 " + userNick + " " + nickname + " server :sono un server IRC\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_WHOISCHANNELS( int fd, std::string userNick, Client const & cli ) {
	std::string message = ":server 319 " + userNick + " " + cli.getNickname() + " :";
	for (size_t i = 0; i < cli.getChannels().size(); i++) {
		if (std::find(cli.getChannels()[i].getOperators().begin(), cli.getChannels()[i].getOperators().end(), cli) != cli.getChannels()[i].getOperators().end())
			message += '@';
		message += cli.getChannels()[i].getName();
		if (i + 1 < cli.getChannels().size())
			message += " ";
	}
	message += "\r\n";
	send(fd, message.c_str(), message.size(), 0);
}

void RPL_ENDOFWHOIS( int fd, std::string userNick, std::string nickname ) {
	std::string message = ":server 318 " + userNick + " " + nickname + " :End of /WHOIS list\r\n";
	send(fd, message.c_str(), message.size(), 0);
}