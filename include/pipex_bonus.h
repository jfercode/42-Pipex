/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 10:39:30 by jaferna2          #+#    #+#             */
/*   Updated: 2025/02/07 12:58:31 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define ERROR_MSG "Expected: ./pipex <infile> <cmd> <cmd> <outfile>\n"

# define IS_CMD_IN 0
# define IS_CMD_OUT 1

# define IS_HEREDOC 1
# define NOT_HEREDOC 0

# include <sys/wait.h>
# include "../source/ft_libft/include/libft.h"
# include "../source/ft_printf/include/ft_printf.h"

typedef struct s_pipe_data
{
	int		fd[2];
	int		fd_in;
	int		fd_out;
	int		prev_fd;
	int		total_pipes;
	int		use_heredoc;
	pid_t	pid;
}			t_pipe_data;

void	ft_execute_cmd(char *argv, char **envp);

int		ft_count_pipes(char **argv, t_pipe_data pipe_data);

char	*ft_find_path(char *cmd, char **envp);
char	*ft_obtain_cmd(char **argv, int indx, int is_in, t_pipe_data pipe_data);

#endif/*PIPEX_H*/
