/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chans.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:25:32 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/09 12:33:12 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::join( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2 || input.size() > 3) {
		ft_sendMsg(cli.getFd(), "server: 'JOIN' usage is JOIN <channels> [<keys>]");
		return false;
	}
	input.pop_front();
	std::deque<std::string> chans = ft_split(input.front(), ',');
	input.pop_front();
	std::deque<std::string> key;
	if (!input.empty())
		key = ft_split(input.front(), ',');
	while (key.size() < chans.size())
		key.push_back("");
	std::cout << "debug: " << chans.front() << std::endl;
	while (!chans.empty()) {
		std::vector<Channel>::iterator it = std::find(_channels.begin(), _channels.end(), chans.front());
		if (it != _channels.end())
				cli.joinChannel(*it, key.front());
		else {
			_channels.push_back(Channel(chans.front(), key.front()));
			cli.joinChannel(_channels.back(), key.front());
		}
		chans.pop_front();
		key.pop_front();
	}
	return true;
}
