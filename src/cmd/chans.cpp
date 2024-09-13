/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chans.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:25:32 by gduranti          #+#    #+#             */
/*   Updated: 2024/09/13 12:26:09 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>
#include <msg.hpp>

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
				// ft_sendMsg(cli.getFd(), ":" + cli.getNickname() + "!server@" + inet_ntoa(_addr.sin_addr) + " JOIN :" + chans.front());
				cli.joinChannel(_channels.back(), key.front());
			}
		}
		chans.pop_front();
		key.pop_front();
	}
	return true;
}

bool Server::mode( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
		return false;
	}
	input.pop_front();
	std::vector<Channel>::iterator chan = std::find(_channels.begin(), _channels.end(), input.front());
	if (chan == _channels.end()) {
		ERR_NOSUCHCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (std::find((*chan).getUsers().begin(), (*chan).getUsers().end(), cli) == (*chan).getUsers().end()) {
		ERR_NOTONCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (input.size() == 1) {
		RPL_CHANNELMODEIS(cli.getFd(), cli.getNickname(), *chan);
		return true;
	}
	if (std::find((*chan).getOperators().begin(), (*chan).getOperators().end(), cli) == (*chan).getOperators().end()) {
		ERR_CHANOPRIVSNEEDED(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	input.pop_front();
	switch (modeCases(input.front())) {
	case eInvite:
		(*chan).setInviteOnly(input.front(), cli);
		break;
	case eTopic:
		(*chan).setTopicRestricted(input.front(), cli);
		break;
	case eKey:
		if ((*chan).setKeyEnable(input, cli) == false)
			ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
		break;
	case eOperator:
		if (input.size() < 2) {
			ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
			break;
		}
		if (std::find((*chan).getUsers().begin(), (*chan).getUsers().end(), input[1]) == (*chan).getUsers().end()) {
			ERR_NOSUCHNICK(cli.getFd(), cli.getNickname(), input[1]);
			break;
		}
		(*chan).manageOperator(input[1], input.front(), cli);
		break;
	case eLimit:
		if ((*chan).setUserLimit(input, cli) == false)
			ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "MODE");
		break;
	default:
		ERR_UNKNOWNMODE(cli.getFd(), cli.getNickname(), input.front());
		break;
	}
	return true;
}

bool Server::kick( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 3) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "KICK");
		return false;
	}
	input.pop_front();
	std::vector<Channel>::iterator ch = std::find(_channels.begin(), _channels.end(), input.front());
	if (ch == _channels.end()) {
		ERR_NOSUCHCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	input.pop_front();
	if (cli.getNickname() == input.front()) {
		ft_sendMsg(cli.getFd(), ":" + (*ch).getName() + " PRIVMSG " + (*ch).getName() + " :you cannot kick yourself from a channel. Try PART command instead");
		return false;
	}
	std::vector<Client>::iterator cl = std::find(_clients.begin(), _clients.end(), input.front());
	if (cl == _clients.end()) {
		ERR_NOSUCHNICK(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (std::find((*ch).getUsers().begin(), (*ch).getUsers().end(), cli) == (*ch).getUsers().end()) {
		ERR_NOTONCHANNEL(cli.getFd(), cli.getNickname(), (*ch).getName());
		return false;
	}
	if (std::find((*ch).getOperators().begin(), (*ch).getOperators().end(), cli) == (*ch).getOperators().end()) {
		ERR_CHANOPRIVSNEEDED(cli.getFd(), cli.getNickname(), (*ch).getName());
		return false;
	}
	input.pop_front();
	if (std::find((*ch).getUsers().begin(), (*ch).getUsers().end(), *cl) == (*ch).getUsers().end()) {
		ft_sendMsg(cli.getFd(), ":" + (*ch).getName() + " " + cli.getNickname() + " :user " + (*cl).getNickname() + " not in channel\r\n");
		return false;
	}
	std::string kickMsg = "stfu";
	if (!input.empty())
		kickMsg = input.front();
	for (size_t i = 0; i < (*ch).getUsers().size(); i++)
		ft_sendMsg((*ch).getUsers()[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " KICK " + (*ch).getName() + " " + (*cl).getNickname() + " :" + kickMsg);
	(*ch).removeUser(*cl);
	return true;
}

bool Server::invite( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 3) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "INVITE");
		return false;
	}
	input.pop_front();
	std::vector<Client>::iterator cl = std::find(_clients.begin(), _clients.end(), input.front());
	if (cl == _clients.end()) {
		ERR_NOSUCHNICK(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	input.pop_front();
	std::vector<Channel>::iterator ch = std::find(_channels.begin(), _channels.end(), input.front());
	if (ch == _channels.end()) {
		ERR_NOSUCHCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (std::find((*ch).getUsers().begin(), (*ch).getUsers().end(), cli) == (*ch).getUsers().end()) {
		ERR_NOTONCHANNEL(cli.getFd(), cli.getNickname(), (*ch).getName());
		return false;
	}
	if ((*ch).getIviteOnly() == true) {
		if (std::find((*ch).getOperators().begin(), (*ch).getOperators().end(), cli) == (*ch).getOperators().end()) {
			ERR_CHANOPRIVSNEEDED(cli.getFd(), cli.getNickname(), (*ch).getName());
			return false;
		}
	}
	(*ch).inviteUser(*cl);
	RPL_INVITING(cli.getFd(), cli.getNickname(), (*ch).getName(), (*cl).getNickname());
	for (size_t i = 0; i < (*ch).getUsers().size(); i++)
		ft_sendMsg((*ch).getUsers()[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " INVITE " + (*cl).getNickname() + " " + (*ch).getName());
	ft_sendMsg((*cl).getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " INVITE " + (*cl).getNickname() + " " + (*ch).getName());
	return true;
}

bool Server::topic( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "TOPIC");
		return false;
	}
	input.pop_front();
	std::vector<Channel>::iterator ch = std::find(_channels.begin(), _channels.end(), input.front());
	if (ch == _channels.end()) {
		ERR_NOSUCHCHANNEL(cli.getFd(), cli.getNickname(), input.front());
		return false;
	}
	if (std::find((*ch).getUsers().begin(), (*ch).getUsers().end(), cli) == (*ch).getUsers().end()) {
		ERR_NOTONCHANNEL(cli.getFd(), cli.getNickname(), (*ch).getName());
		return false;
	}
	input.pop_front();
	if (input.empty()) {
		if ((*ch).getTopic().empty())
			RPL_NOTOPIC(cli.getFd(), cli.getNickname(), (*ch).getName());
		else
			RPL_TOPIC(cli.getFd(), cli.getNickname(), (*ch).getName(), (*ch).getTopic());
	}
	else {
		if ((*ch).getTopicRestricted() == true && std::find((*ch).getOperators().begin(), (*ch).getOperators().end(), cli) == (*ch).getOperators().end()) {
			ERR_CHANOPRIVSNEEDED(cli.getFd(), cli.getNickname(), (*ch).getName());
			return false;
		}
		(*ch).setTopic(input.front());
		for (size_t i = 0; i < (*ch).getUsers().size(); i++)
			ft_sendMsg((*ch).getUsers()[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " TOPIC " + (*ch).getName() + " :" + input.front());
	}
	return true;
}

bool Server::part( Client & cli, std::deque<std::string> input ) {
	if (input.size() < 2) {
		ERR_NEEDMOREPARAMS(cli.getFd(), cli.getNickname(), "PART");
		return false;
	}
	input.pop_front();
	std::deque<std::string> qu = ft_split(input.front(), ',');
	do {
		std::vector<Channel>::iterator ch = std::find(_channels.begin(), _channels.end(), qu.front());
		if (ch == _channels.end())
			ERR_NOSUCHCHANNEL(cli.getFd(), cli.getNickname(), qu.front());
		else if (std::find((*ch).getUsers().begin(), (*ch).getUsers().end(), cli) == (*ch).getUsers().end())
			ERR_NOTONCHANNEL(cli.getFd(), cli.getNickname(), (*ch).getName());
		else {
			for (size_t i = 0; i < (*ch).getUsers().size(); i++)
				ft_sendMsg((*ch).getUsers()[i].getFd(), ":" + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostName() + " PART " + (*ch).getName() + " :goodbye!");
			(*ch).removeUser(cli);
		}
		qu.pop_front();
	} while (!qu.empty());
	return true;
}
