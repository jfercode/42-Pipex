/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:20:24 by jaferna2          #+#    #+#             */
/*   Updated: 2025/02/07 14:20:14 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

// Function to find the path of the command
char	*ft_find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == NULL)
		i++;
	paths = ft_split(envp[i], ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (NULL);
}

// Function to execute the command
void	ft_execute_cmd(char *argv, char **envp)
{
	char	**cmd;
	char	*path;
	int		i;

	i = 0;
	cmd = ft_split(argv, ' ');
	if (!envp || !cmd[0])
		return (perror("Invalid command"), free(cmd), exit(127));
	path = ft_find_path(cmd[0], envp);
	if (!path)
	{
		while (cmd[i])
			free (cmd[i++]);
		free (cmd);
		return (perror("Command not found"), exit(127));
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
		return (perror("Error executing command"), exit(127));
	}
}

// Function that returns the number of pipes for the program
int	ft_count_pipes(char **argv, t_pipe_data pipe_data)
{
	int	i;
	int	pipes;

	i = 2;
	pipes = 0;
	if (!argv || !argv[i])
		return (0);
	if (pipe_data.use_heredoc == IS_HEREDOC)
		i = 3;
	while (argv[i] != NULL)
	{
		pipes++;
		i++;
	}
	return (pipes - 2);
}

// Function that returns the cmd to be executed in the pipes (child/parent)
char	*ft_obtain_cmd(char **argv, int indx, int is_in, t_pipe_data pipe_data)
{
	int	offset;

	if (pipe_data.use_heredoc == IS_HEREDOC)
		offset = 3;
	else
		offset = 2;
	if (is_in == IS_CMD_IN)
	{
		if (argv[offset + indx][0] != '\0')
			return(argv[offset + indx]);
	}
	else if (is_in == IS_CMD_OUT)
	{
		if (argv[offset + indx + 1][0] != '\0')
			return(argv[offset + indx + 1]);
	}
	printf("%s\n", argv[offset + indx]);
	return (NULL);
}
