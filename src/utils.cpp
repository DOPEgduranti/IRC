/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:51:47 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/10 16:27:33 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <utils.hpp>

bool portKeyCheck( std::string port, std::string key ) {
	for (size_t i = 0; i < port.size(); i++) {
		if (!std::isdigit(port[i]))
			return false;
	}
	if (std::strtol(port.c_str(), NULL, 10) < 1024 || std::strtol(port.c_str(), 0, 10) > 49151)
		return false;
	if (key.empty())
		return false;
	return true;
}

int ft_sendMsg( int fd, std::string msg ) {
	msg.append("\r\n");
	return send(fd, msg.c_str(), msg.size(), 0);
}

std::string getFirstString( std::string & str ) {
	size_t i = 0;
	while (str[i] && std::isspace(str[i]))
		i++;
	size_t j = i;
	while (str[j] && !std::isspace(str[j]))
		j++;
	if (j <= i || j > str.size())
		return "";
	return str.substr(i, j - i);
}

std::string ft_erase( std::string & str, char c ) {
	if (c <= 0 || c > 127)
		return str;
	std::string::iterator it = std::find(str.begin(), str.end(), c);
	while (it != str.end()) {
		str.erase(it);
		it = std::find(str.begin(), str.end(), c);
	}
	return str;
}

std::string ft_erase( std::string & str, std::string set ) {
	for (size_t i = 0; i < set.size(); i++) {
		std::string::iterator it = std::find(str.begin(), str.end(), set[i]);
		while (it != str.end()) {
			str.erase(it);
			it = std::find(str.begin(), str.end(), set[i]);
		}
	}
	return str;
}

std::deque<std::string> ft_split( std::string & str, char c ) {
	std::deque<std::string> deq;
	if (str.find(c) == std::string::npos) {
		deq.push_back(ft_erase(str, "\r\n"));
		return deq;
	}
	std::string tmp;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == ':') {
			c = '\0';
			continue ;
		}
		if (str[i] == c && !tmp.empty()) {
			deq.push_back(tmp);
			tmp.clear();
		}
		else if (str[i] != c && str[i] != '\r' && str[i] != '\n')
			tmp += str[i];
	}
	if (!tmp.empty())
		deq.push_back(tmp);
	return deq;
}

bool channelSintax( std::string & str ) {
	if (str.empty() || str.size() < 2)
		return false;
	if (str[0] != '#')
		return false;
}
