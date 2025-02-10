/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 10:30:17 by jaferna2          #+#    #+#             */
/*   Updated: 2025/02/10 15:27:46 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

/**
 * @brief Removes the trailing newline character from a string.
 *
 * If the string is non-null and ends with a newline ('\n'),
 * this function replaces it with a null terminator.
 *
 * @param line The string to process.
 * @return The modified string without a trailing newline, 
 * or NULL if the input is NULL.
 */
static char	*remove_newline(char *line)
{
	size_t	len;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

/**
 * @brief Processes here-document input.
 *
 * Opens a temporary file and repeatedly prompts the user with "heredoc> "
 *  to read input.
 * Each line (after removing its newline) is compared with the delimiter;
 *  if it matches, the file is closed and IS_HEREDOC is returned. 
 * Otherwise, the line is written to the file.
 *
 * @param delimiter The string that terminates the here-document input.
 * @return IS_HEREDOC when the delimiter is encountered, or 0 if an error occurs.
 */
int	ft_handle_here_doc(char *delimiter)
{
	int		tmp_fd;
	char	*line;

	tmp_fd = open("/tmp/heredoc_tmp.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (perror("Error opening temp here_doc file"), 0);
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 10);
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			return (perror("Error reading here_doc"), close(tmp_fd), 0);
		line = remove_newline(line);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			return (free(line), close(tmp_fd), IS_HEREDOC);
		write(tmp_fd, line, ft_strlen(line));
		write(tmp_fd, "\n", 1);
		free(line);
	}
	close(tmp_fd);
	return (NOT_HEREDOC);
}

/**
 * @brief Manages the child process after fork.
 *
 * For the first command, redirects STDIN from fd_in.
 * Then, it redirects STDOUT to the pipe's write end, closes unused descriptors,
 * obtains the command (with IS_CMD_IN flag), and executes it.
 *
 * @param pipe_data Structure containing pipe descriptors and related data.
 * @param argv      Array of command arguments.
 * @param envp      Array of environment variables.
 * @param indx      Index of the current command.
 */
static void	child_process(t_pipe_data pipe_data, char **argv,
					char **envp, int indx)
{
	char	*cmd;

	if (indx == 0)
	{
		if (dup2(pipe_data.fd_in, STDIN_FILENO) == -1)
			return (perror("Error fd_in dup2 failed"), exit(1));
		close(pipe_data.fd_in);
	}
	if (dup2(pipe_data.fd[1], STDOUT_FILENO) == -1)
		return (perror("Error child dup2 failed"), exit(1));
	close(pipe_data.fd[0]);
	cmd = ft_obtain_cmd(argv, indx, IS_CMD_IN, pipe_data);
	if (!cmd)
		return (perror("Error Invalid command"), exit(1));
	ft_execute_cmd(cmd, envp);
}

/**
 * @brief Manages the parent process after fork.
 *
 * Waits for the child process to finish, closes the write end of the pipe,
 * and redirects the standard input to the read end. 
 * If it's the last command,it also redirects the standard output to fd_out,
 * obtains, and executes the command.
 *
 * @param pipe_data Structure containing pipe descriptors and other related data.
 * @param argv      Array of command arguments.
 * @param envp      Environment variables.
 * @param indx      Index of the current command.
 */
static void	parent_process(t_pipe_data pipe_data, char **argv,
					char **envp, int indx)
{
	int		status;
	char	*cmd;

	waitpid(-1, &status, 0);
	close (pipe_data.fd[1]);
	if (dup2(pipe_data.fd[0], STDIN_FILENO) == -1)
		return (perror("Error parent dup2 failed"), exit(1));
	if (indx == pipe_data.total_pipes - 1)
	{
		if (dup2(pipe_data.fd_out, STDOUT_FILENO) == -1)
			return (perror("Error fd_out dup2 failed"), exit(1));
		close(pipe_data.fd_out);
		cmd = ft_obtain_cmd(argv, indx, IS_CMD_OUT, pipe_data);
		if (!cmd)
			return (perror("Error Invalid command"), exit(1));
		ft_execute_cmd(cmd, envp);
	}
	else
		if (dup2(pipe_data.fd[0], STDIN_FILENO) == -1)
			return (perror("Error child dup2 failed"), exit(1));
}

/**
 * @brief Entry point for the pipex program.
 *
 * Initializes the pipe data structure and sets up the pipeline 
 * for multiple commands.
 * For each command, it creates a pipe and forks a child process:
 * - The child process handles command execution.
 * - The parent process manages redirection and process synchronization.
 *
 * If a here-document is used, the temporary file is deleted after execution.
 *
 * @param argc Argument count.
 * @param argv Argument vector (program name, files, commands, etc.).
 * @param envp Environment variables.
 * @return EXIT_SUCCESS on successful execution, or an error code on failure.
 */
int	main(int argc, char **argv, char **envp)
{
	int			i;
	t_pipe_data	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe_data));
	if (argc < 5)
		return (perror("Expected: ./pipex "
				"[here_doc][LIMITER] file1 cmd1 ... file2\n"), 1);
	ft_prepare_for_pipex(argc, argv, &pipe_data);
	i = 0;
	while (i < pipe_data->total_pipes)
	{
		if (pipe(pipe_data->fd) == -1)
			return (perror("Error creating pipe"), EXIT_FAILURE);
		pipe_data->pid = fork();
		if (pipe_data->pid == -1)
			return (perror("Error creating the calling process clone"), 1);
		if (pipe_data->pid == 0)
			child_process(*pipe_data, argv, envp, i);
		else
			parent_process(*pipe_data, argv, envp, i);
		i++;
	}
	if (pipe_data->use_heredoc == IS_HEREDOC)
		unlink("/tmp/heredoc_tmp.txt");
	return (EXIT_SUCCESS);
}
