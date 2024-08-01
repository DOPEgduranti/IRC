/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   communication.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 11:36:09 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/01 12:55:09 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::privmsg( Client & cli, std::deque<std::string> input ) {
	std::string message = input.back();
	input.pop_back();
	input.pop_front();
	while (!input.empty()) {
		std::vector<Client>::iterator tmp1 = std::find(_clients.begin(), _clients.end(), input.front());
		std::vector<Channel>::iterator tmp2 = std::find(_channels.begin(), _channels.end(), input.front());
		if (tmp1 != _clients.end())
			ft_sendMsg((*tmp1).getFd(), ":" + cli.getNickname() + " PRIVMSG " + input.front() + " :" + message);
		else if (tmp2 != _channels.end())
			(*tmp2).broadcastMsg(cli, message);
		input.pop_front();
	}
	return true;
}
