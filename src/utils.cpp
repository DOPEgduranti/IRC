/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 10:51:47 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/05 16:04:53 by gduranti         ###   ########.fr       */
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
	return send(fd, msg.c_str(), msg.size(), 0);
}
