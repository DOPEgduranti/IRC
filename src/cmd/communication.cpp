/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:36:09 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/10 11:20:01 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::privmsg( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 3) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "PRIVMSG");
		return false;
	}
	std::string message = input.back();
	input.pop_back();
	input.pop_front();
	while (!input.empty()) {
		std::vector<Client>::iterator tmp1 = std::find(_clients.begin(), _clients.end(), input.front());
		std::vector<Channel>::iterator tmp2 = std::find(_channels.begin(), _channels.end(), input.front());
		if (tmp1 != _clients.end())
			ft_sendMsg((*tmp1).getFd(), ":" + cli.getNickname() + " PRIVMSG " + input.front() + " :" + message);
		else if (tmp2 != _channels.end() && std::find(cli.getChannels().begin(), cli.getChannels().end(), *tmp2) != cli.getChannels().end())
			(*tmp2).broadcastMsg(cli, message);
		else
			ERR_NOSUCHNICK(cli.getFd(), cli.getNickname(), input.front());
		input.pop_front();
	}
	return true;
}

bool Server::names( Client & cli, std::deque<std::string> input ) {
	if (input.size() == 1) {
		for (size_t i = 0; i < _channels.size(); i++) {
			RPL_NAMREPLY(cli.getFd(), cli.getNickname(), _channels[i]);
			RPL_ENDOFNAMES(cli.getFd(), cli.getNickname(), _channels[i].getName());
		}
		std::string message = ":server 353 " + cli.getNickname() + " " + cli.getNickname() + " :";
		for (size_t i = 0; i < _clients.size(); i++) {
			if (_clients[i].getChannels().empty())
				message += _clients[i].getNickname();
			if (i + 1 < _clients.size())
				message += " ";
		}
		ft_sendMsg(cli.getFd(), message);
		RPL_ENDOFNAMES(cli.getFd(), cli.getNickname(), cli.getNickname());
	}
	else {
		input.pop_front();
		std::deque<std::string> chans = ft_split(input.front(), ',');
		while (!chans.empty()) {
			std::vector<Channel>::iterator it = std::find(_channels.begin(), _channels.end(), chans.front());
			if (it != _channels.end()) {
				RPL_NAMREPLY(cli.getFd(), cli.getNickname(), *it);
				RPL_ENDOFNAMES(cli.getFd(), cli.getNickname(), (*it).getName());
			}
			chans.pop_front();
		}
	}
	return true;
}
