/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chans.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 12:25:32 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/08 12:25:56 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Server.hpp>

bool Server::join( Client & cli, std::string str ) {
	size_t i = str.find("JOIN") + 4;
	while (str[i] && std::isspace(str[i]))
		i++;
	std::cout << "Client <" << cli.getNickname() << "joined '" << &str[i] << "'." << std::endl;
	return true;
}
