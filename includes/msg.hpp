/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gduranti <gduranti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 10:57:24 by gduranti          #+#    #+#             */
/*   Updated: 2024/07/10 11:50:04 by gduranti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSG_HPP
# define MSG_HPP

# include <iostream>

# define ERR_NEEDMOREPARAMS /*<command>*/" :Not enough parameters" //Returned by the server by numerous commands to indicate to the client that it didn’t supply enough parameters.
# define ERR_ALREADYREGISTRED " :You may not reregister" //Returned by the server to any link which tries to change part of the registered details (such as password or user details from second USER message)
# define ERR_NICKNAMEINUSE /*<nick>*/ " :Nickname is already in use" //Returned when a NICK message is processed that results in an attempt to change to a currently existing nickname.
# define ERR_NOTREGISTERED " :You have not registered" //Returned by the server to indicate that the client must be registered before the server will allow it to be parsed in detail.
# define ERR_NONICKNAMEGIVEN " :No nickname given" //Returned when a nickname parameter expected for a command and isn’t found.
# define MSG_QUITDEFAULT " :You disconnected from server" //Default Quit message

// errors.cpp

#endif