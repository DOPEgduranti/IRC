/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chans.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:25:32 by gduranti          #+#    #+#             */
/*   Updated: 2024/08/02 15:49:05 by gduranti         ###   ########.fr       */
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
	if (input.size() < 3) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
		return false;
	}
	input.pop_front();
	std::vector<Channel>::iterator chan = std::find(_channels.begin(), _channels.end(), input.front());
	if (chan == _channels.end()) {
		ERR_NOSUCHCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (std::find((*chan).getUsers().begin(), (*chan).getUsers().end(), input.front()) == (*chan).getUsers().end()) {
		ERR_NOTONCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (std::find((*chan).getOperators().begin(), (*chan).getOperators().end(), cli) == (*chan).getOperators().end()) {
		ERR_CHANOPRIVSNEEDED(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	input.pop_front();
	std::vector<Client>::iterator tmp;
	switch (modeCases(input.front())) {
	case eInvite:
		(*chan).setInviteOnly(input.front());
		break;
	case eTopic:
		(*chan).setTopicRestricted(input.front());
		break;
	case eKey:
		if ((*chan).setKeyEnable(input) == false)
			ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
		break;
	case eOperator:
		if (input.size() < 2) {
			ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
			break;
		}
		tmp = std::find((*chan).getUsers().begin(), (*chan).getUsers().end(), input[1]);
		if (tmp == (*chan).getUsers().end()) {
			ERR_NOSUCHNICK(cli.getFd(), input[1]);
			break;
		}
		(*chan).manageOperator(*tmp, input.front());
		break;
	case eLimit:
		if ((*chan).setUserLimit(input) == false)
			ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
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
