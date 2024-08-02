/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chans.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:25:32 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/02 11:48:42 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::join( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "JOIN");
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
	while (!chans.empty()) {
		if (channelSintax(chans.front())) {
			if (cli.getChannelNbr() >= MAX_JOINABLECHANNELS) {
				ERR_TOOMANYCHANNELS(cli.getFd(), cli.getNickname(), chans.front());
				return false;
			}
			std::vector<Channel>::iterator it = std::find(_channels.begin(), _channels.end(), chans.front());
			if (it != _channels.end())
					cli.joinChannel(*it, key.front());
			else {
				_channels.push_back(Channel(chans.front(), key.front()));
				cli.joinChannel(_channels.back(), key.front());
			}
		}
		chans.pop_front();
		key.pop_front();
	}
	return true;
}

bool Server::mode( Client & cli, std::deque<std::string> input ) {
	input.pop_front();
	if (std::find(cli.getChannels().begin(), cli.getChannels().end(), input.front()) == cli.getChannels().end())
		return false;
	std::vector<Channel>::iterator chan = std::find(_channels.begin(), _channels.end(), input.front());
	if (std::find((*chan).getOperators().begin(), (*chan).getOperators().end(), cli) == (*chan).getOperators().end())
		return false;
	input.pop_front();
	std::vector<Client>::iterator tmp;
	switch (modeCases(input.front())) {
	case eInvite:
		(*chan).setInviteOnly();
		break;
	case eTopic:
		(*chan).setTopicRestricted();
		break;
	case eKey:
		input.pop_front();
		if (input.empty())
			break;
		(*chan).setKeyEnable(input.front());
		break;
	case eOperator:
		input.pop_front();
		if (input.empty())
			break;
		tmp = std::find(_clients.begin(), _clients.end(), input.front());
		if (tmp == _clients.end())
			break;
		(*chan).addOperator(*tmp);
		break;
	case eLimit:
		input.pop_front();
		if (input.empty())
			break;
		(*chan).setUserLimit(validPositiveInteger(input.front()));
		break;
	default:
		break;
	}
	return true;
}

bool Server::kick( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true;
}

bool Server::invite( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true;
}

bool Server::topic( Client & cli, std::deque<std::string> input ) {
	(void)cli;
	(void)input;
	return true;
}
