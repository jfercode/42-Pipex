/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaferna2 <jaferna2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 10:30:17 by jaferna2          #+#    #+#             */
/*   Updated: 2025/02/07 16:25:19 by jaferna2         ###   ########.fr       */
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

static int	handle_here_doc(char *delimiter)
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
			return(free(line), close(tmp_fd), IS_HEREDOC);
		write(tmp_fd, line, ft_strlen(line));
		write(tmp_fd, "\n", 1);
		free(line);
	}
	close(tmp_fd);
	return (NOT_HEREDOC);
}

/// TO DO: OBTENER EL FD DE ENTRADA (P.E EL PRIMERO PUEDE SER EL HEREDOC U OTRO DONDE SE
/// ENCUENTREN LOS DATOS QUE QUEREMOS MANDAR)
// Child process function
static void child_process(t_pipe_data pipe_data, char **argv, char **envp, int indx)
{
	char	*cmd;
	
	pipe_data.fd_in = open(argv[1], O_RDONLY, 0777);
	if (pipe_data.fd_in == -1)
		return (perror("Error opening input file"), exit (1));
	if (dup2(pipe_data.fd_in, STDIN_FILENO) == -1)
		return (perror("Error fd_in dup2 failed"), exit(1));
	close(pipe_data.fd_in);
	if (dup2(pipe_data.fd[1], STDOUT_FILENO) == -1)
	 	return (perror("Error child dup2 failed"), exit(1));
	close(pipe_data.fd[0]);
	cmd = ft_obtain_cmd(argv, indx, IS_CMD_IN, pipe_data);
	if (!cmd)
		return (perror("Error Invalid command"), exit(1));
	ft_execute_cmd(cmd, envp);
}

/// TO DO: OBTENER EL FD DONDE QUEREMOS MANDAR LOS DATOS (P.E EL ULTIMO SERA EL ARCHIVO Y
/// LOS INTERMEDIARIOS DEBERIAN DE SER EL FD SIGUIENTE)
/// SI ES EL ULTIMO COMANDO EL QUE LE PASAMOS DEBERIA DE EJECUTARSE Y PASARLO DIRECTAMENTE
/// AL ARCHIVO OUTPUT SINO TENDREMOS QUE REDIRECCIONARLOS AL PUNTO MEDIO ENTRE PIPES
// Parent process function
static void parent_process(t_pipe_data pipe_data, char **argv,char **envp, int indx)
{
	int	status;

	waitpid(-1, &status, 0);
	if (indx == pipe_data.total_pipes - 1)
	{
		pipe_data.fd_out = open(argv[pipe_data.total_pipes + 2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipe_data.fd_out == -1)
			return (perror("Error opening output file"), exit(1));
		if (dup2(pipe_data.fd_out, STDOUT_FILENO) == -1)
			return (perror("Error fd_out dup2 failed"), exit(1));
		close(pipe_data.fd_out);
		if (dup2(pipe_data.fd[0], STDIN_FILENO) == -1)
			return (perror("Error parent dup2 failed"), exit(1));
		close(pipe_data.fd[1]);
		ft_execute_cmd(ft_obtain_cmd(argv, indx, IS_CMD_OUT, pipe_data), envp);
	}
}

static void	prepare_for_pipex(char **argv, t_pipe_data **pipe_data)
{
	if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		(*pipe_data)->use_heredoc = IS_HEREDOC;
	else
		(*pipe_data)->use_heredoc = NOT_HEREDOC;
	(*pipe_data)->total_pipes = ft_count_pipes(argv, **pipe_data);
	if ((*pipe_data)->use_heredoc == IS_HEREDOC)
	{
		if (handle_here_doc(argv[2]) == IS_HEREDOC)
			(*pipe_data)->fd_in = open("/tmp/heredoc_tmp.txt", O_RDONLY, 0644);
		else
			return (perror("Error failed to handle heredoc"), exit(1));
	}
}

/// TO DO: TENEMOS QUE MODIFICAR ESTE MAIN EN CUANTO A LA LOGICA DEL BUCLE PARA QUE LOS FD 
/// SE VAYA REDIRECCIONANDO LA INFORMACION (DE UN PIPE AL SIGUIENTE O AL FINAL)
///		IN ->	CMD	|	CMD	| ... | CMD  -> OUT
int	main(int argc, char **argv, char **envp)
{
	int			i;
	t_pipe_data	*pipe_data;

	pipe_data = malloc(sizeof(t_pipe_data));
	if (argc < 5)
		return (perror("Expected: ./pipex "
			"[here_doc][LIMITER] file1 cmd1 ... file2\n"), EXIT_FAILURE);	
	prepare_for_pipex(argv, &pipe_data);
	i = 0;
	while (i < pipe_data->total_pipes)
	{
		if (pipe(pipe_data->fd) == -1)
			return (perror("Error creating pipe"), EXIT_FAILURE);
		pipe_data->pid = fork();
		if (pipe_data->pid == -1)
			return (perror("Error creating the calling process clone"), EXIT_FAILURE);
		if (pipe_data->pid == 0)
			child_process(*pipe_data, argv, envp, i);
		else
			parent_process(*pipe_data, argv,envp, i);
		i++;
	}
	if (pipe_data->use_heredoc == IS_HEREDOC)
		unlink("/tmp/heredoc_tmp.txt");
	return (EXIT_SUCCESS);
}
