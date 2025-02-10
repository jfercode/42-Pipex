/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 11:20:24 by jaferna2          #+#    #+#             */
/*   Updated: 2025/02/10 15:26:25 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * @brief Finds the full path of a command using the system's PATH 
 * environment variable.
 *
 * Searches through each directory listed in the PATH environment variable,
 * appending the command name to each path, and checks if the command exists 
 * using `access()`. 
 * 
 * Returns the first valid path found.
 *
 * Memory allocated for temporary paths is freed to prevent leaks.
 *
 * @param cmd  The command to find (e.g., "ls", "grep").
 * @param envp The environment variables containing the PATH.
 * @return The full path to the executable if found, or NULL if not found.
 */
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

/**
 * @brief Executes a command using the provided environment variables.
 *
 * Splits the command string into arguments, locates the executable path,
 * and attempts to run the command with `execve`. 
 * Handles errors related to:
 * - Invalid commands
 * - Command not found
 * - Execution failures
 *
 * Frees allocated memory in case of errors to prevent memory leaks.
 *
 * @param argv The command string to execute.
 * @param envp The environment variables used by the new process.
 */
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

/**
 * @brief Counts the number of pipes needed based on the provided commands.
 *
 * Calculates the number of commands that require piping. 
 * The starting index is adjusted depending on whether
 * here-document mode is active:
 * - Starts from index 3 if using here-document (skipping "here_doc",
 *  LIMITER, and file).
 * - Starts from index 2 otherwise (skipping input and output files).
 *
 * The total number of pipes is the number of commands minus 1, 
 * as each pipe connects two commands.
 *
 * @param argv       Argument vector containing commands and files.
 * @param pipe_data  Pipe data structure with here-document information.
 * @return The total number of pipes required.
 */
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

/**
 * @brief Retrieves the command to be executed based on 
 * the current index and mode.
 *
 * Determines the correct command from the argument 
 * vector considering whether here-document mode is active. 
 * The offset is adjusted accordingly:
 * - Offset = 3 if using here-document 
 * (accounts for "here_doc", LIMITER, and file).
 * - Offset = 2 otherwise (input file and output file).
 * 
 * The function distinguishes between input and output commands:
 * - For input commands (IS_CMD_IN), it checks the current index.
 * - For output commands (IS_CMD_OUT), it checks the next index.
 *
 * @param argv       Argument vector containing commands and files.
 * @param indx       Index of the current command.
 * @param is_in      Flag indicating if the command is input 
 * (IS_CMD_IN) or output (IS_CMD_OUT).
 * @param pipe_data  Pipe data structure with here-document info.
 * @return The command string if valid, or NULL if the command 
 * is empty or invalid.
 */
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
			return (argv[offset + indx]);
	}
	else if (is_in == IS_CMD_OUT)
	{
		if (argv[offset + indx + 1][0] != '\0')
			return (argv[offset + indx + 1]);
	}
	printf("%s\n", argv[offset + indx]);
	return (NULL);
}

/**
 * @brief Prepares the pipe data structure for the pipex program.
 *
 * Determines if here-document mode is active based on the first argument.
 * Sets the total number of pipes and handles the opening of input/output files:
 * - If here-document is used, it processes the input and opens 
 * the temporary file.
 * - Otherwise, it opens the specified input file in read-only mode.
 * The output file is opened in append mode for here-document, 
 * or truncated otherwise.
 *
 * @param argc       Argument count.
 * @param argv       Argument vector containing file names, 
 * commands, and options.
 * @param pipe_data  Pointer to the pipe data structure to be initialized.
 */
void	ft_prepare_for_pipex(int argc, char **argv, t_pipe_data **pipe_data)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		(*pipe_data)->use_heredoc = IS_HEREDOC;
	else
		(*pipe_data)->use_heredoc = NOT_HEREDOC;
	(*pipe_data)->total_pipes = ft_count_pipes(argv, **pipe_data);
	if ((*pipe_data)->use_heredoc == IS_HEREDOC)
	{
		if (ft_handle_here_doc(argv[2]))
			(*pipe_data)->fd_in = open("/tmp/heredoc_tmp.txt", O_RDONLY, 0644);
		else
			return (perror("Error failed to handle heredoc"), exit(1));
	}
	else
		(*pipe_data)->fd_in = open(argv[1], O_RDONLY, 0644);
	if ((*pipe_data)->fd_in == -1)
		return (perror("Error opening input file"), exit(1));
	if ((*pipe_data)->use_heredoc == IS_HEREDOC)
		(*pipe_data)->fd_out = open(argv[argc -1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		(*pipe_data)->fd_out = open(argv[argc -1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if ((*pipe_data)->fd_out == -1)
		return (perror("Error opening output file"), exit(1));
}
