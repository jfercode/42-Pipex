/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 10:30:17 by jaferna2          #+#    #+#             */
/*   Updated: 2025/02/10 15:04:05 by jaferna2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

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

///		IN ->	CMD	|	CMD	| ... | CMD  -> OUT
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
