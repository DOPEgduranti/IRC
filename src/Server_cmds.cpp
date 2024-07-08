/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_cmds.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:58:56 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 16:19:06 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <Server.hpp>

bool Server::help( Client & cli, std::string str ) {
	(void)str;
	std::ifstream fin("src/help.txt");
	if (fin.fail()) {
		std::cerr << "Server opens 'help.txt': FAILURE" << std::endl;
		return false;
	}
	std::string buffer;
	std::getline(fin, buffer, '\0');
	ft_sendMsg(cli.getFd(), buffer);
	fin.close();
	return true;
}

bool Server::pass( Client & cli, std::string str ) {
	if (cli.getLogged() == true) {
		ft_sendMsg(cli.getFd(), "You are already logged in");
		return true;
	}
	size_t i = str.find("PASS") + 4;
	while (str[i] && std::isspace(str[i]))
		i++;
	if (str.substr(i, str.size() - i - 1) == _key) {
		cli.login();
		ft_sendMsg(cli.getFd(), "login: SUCCESS\n");
		return true;
	}
	ft_sendMsg(cli.getFd(), "login: FAILURE\n");
	return false;
}

bool Server::nick( Client & cli, std::string str ) {
	size_t i = str.find("NICK") + 4;
	while (str[i] && std::isspace(str[i]))
		i++;
	size_t j = i;
	while (str[j] && !std::isspace(str[j]))
		j++;
	if (!str[i] || str[j] != '\n') {
		ft_sendMsg(cli.getFd(), "NICK usage is: NICK <nickname>\n");
		return false;
	}
	cli.setNick(str.substr(i, str.size() - i - 1));
	ft_sendMsg(cli.getFd(), "nickname change: SUCCESS\n");
	return true;
}

bool Server::join( Client & cli, std::string str ) {
	size_t i = str.find("JOIN") + 4;
	while (str[i] && std::isspace(str[i]))
		i++;
	std::cout << "Client <" << cli.getNick() << "joined '" << &str[i] << "'." << std::endl;
	return true;
}

