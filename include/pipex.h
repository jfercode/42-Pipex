/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 10:23:17 by jaferna2          #+#    #+#             */
/*   Updated: 2024/12/27 11:30:30 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define ERROR_MSG "Expected: ./pipex <infile> <cmd> <cmd> <outfile>\n"

# include <sys/wait.h>
# include "../source/ft_libft/include/libft.h"
# include "../source/ft_printf/include/ft_printf.h"

void	ft_execute_cmd(char *argv, char **envp);

char	*ft_find_path(char *cmd, char **envp);

#endif/*PIPEX_H*/