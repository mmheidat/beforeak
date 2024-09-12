/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 10:59:03 by mmheidat          #+#    #+#             */
/*   Updated: 2024/09/11 21:41:56 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

char	**get_path(t_env *envp_list)
{
	char	*path;
	char	**path_split;
	t_env	*tmp;

	path = NULL;
	tmp = envp_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "PATH") == 0)
		{
			path = ft_strdup(tmp->value);
			break ;
		}
		tmp = tmp->next;
	}
	if (path)
	{
		path_split = ft_split_without_quotes(path, ':');
		free(path);
		return (path_split);
	}
	return (NULL);
}

void	handle_exec_error(char *cmd, t_data *data)
{
	(void)data;
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", 21);
	free_data(data);
	exit(127);
}

void	exec_command(t_cmd *cmd, char **envp, t_data *data, int *pip)
{
	close_pipes(pip, cmd);
	if (check_built(cmd, *data) == 0)
	{
		check_current(cmd, *data);
		exit(0);
	}
	handle_redirection(cmd->files);
	if (!cmd->content || !cmd->content[0])
		exit(0);
	exec_cmd_or_find_in_path(cmd, envp, data);
}

void	command_do(t_cmd *cmd, char **envp, t_data *data, int *tmd_fd)
{
	int		pip[2];
	t_cmd	*tmp;

	tmp = cmd;
	while (tmp)
	{
		pipe(pip);
		open_heredoc_first(tmp->files, data);
		tmp->pid = fork();
		if (tmp->pid == -1)
			exit(1);
		if (tmp->pid == 0)
		{
			close(tmd_fd[0]);
			close(tmd_fd[1]);
			exec_command(tmp, envp, data, pip);
		}
		close(pip[1]);
		dup2(pip[0], STDIN_FILENO);
		close(pip[0]);
		tmp = tmp->next;
	}
}

void	ft_execute(t_cmd *cmd, char **envp, t_data *data, int *tmd_fd)
{
	t_cmd	*full_cmd;

	full_cmd = cmd;
	command_do(cmd, envp, data, tmd_fd);
	while (full_cmd)
	{
		waitpid(full_cmd->pid, &data->exit_status, 0);
		full_cmd = full_cmd->next;
		data->exit_status = WEXITSTATUS(data->exit_status);
	}
}
