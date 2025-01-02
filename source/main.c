/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 10:30:17 by jaferna2          #+#    #+#             */
/*   Updated: 2024/12/27 11:51:17 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

// Child process function
static void	child_process(int *fd, char **argv, char **envp)
{
	int	fd_in;

	fd_in = open(argv[1], O_RDONLY, 0777);
	if (fd_in == -1)
		return (perror("Error opening input file"), exit(1));
	if (dup2(fd[1], STDOUT_FILENO) == -1 || dup2(fd_in, STDIN_FILENO) == -1)
		return (perror("Error dup2 failed"), exit(1));
	close(fd[0]);
	if (argv[2] == NULL || argv[2][0] == '\0')
		return (perror("Error Invalid command"), exit(1));
	ft_execute_cmd(argv[2], envp);
}

// Parent process function
static void	parent_process(int *fd, char **argv, char **envp)
{
	int	fd_out;
	int	status;

	fd_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd_out == -1)
		return (perror("Error opening output file"), exit(1));
	if (dup2(fd[0], STDIN_FILENO) == -1 || dup2(fd_out, STDOUT_FILENO) == -1)
		return (perror("Error dup2 failed"), exit(1));
	close(fd[1]);
	waitpid(-1, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		ft_execute_cmd(argv[3], envp);
	else
		exit(1);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (argc != 5)
		return (perror(ERROR_MSG), -1);
	if (pipe(fd) == -1)
		return (perror("Error creating pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("Error creating the calling process clone"), -1);
	if (pid == 0)
		child_process(fd, argv, envp);
	parent_process(fd, argv, envp);
	return (0);
}
