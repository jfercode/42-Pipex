/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:20:24 by jaferna2          #+#    #+#             */
/*   Updated: 2024/12/27 11:55:47 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

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
		return (perror("Invalid command"), exit(1));
	path = ft_find_path(cmd[0], envp);
	if (!path)
	{
		while (cmd[i])
			free (cmd[i++]);
		free (cmd);
		return (perror("Command not found"), exit(1));
	}
	if (execve(path, cmd, envp) == -1)
	{
		free(path);
		while (cmd[i])
			free(cmd[i++]);
		free(cmd);
			return (perror("Error executing command"), exit(1));
    }
}
