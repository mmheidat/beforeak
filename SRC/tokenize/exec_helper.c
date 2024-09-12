/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:58:15 by mmheidat          #+#    #+#             */
/*   Updated: 2024/09/11 14:58:39 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void	close_pipes(int *pip, t_cmd *cmd)
{
	close(pip[0]);
	if (cmd->next)
		dup2(pip[1], STDOUT_FILENO);
	close(pip[1]);
}

void	exec_cmd_or_find_in_path(t_cmd *cmd, char **envp, t_data *data)
{
	if (execve(cmd->content[0], cmd->content, envp) == -1)
		find_cmd_in_path(cmd, envp, data);
}

void	find_cmd_in_path(t_cmd *cmd, char **envp, t_data *data)
{
	char	**path;
	char	*tmp_cmd;
	char	**tmp;

	path = get_path(data->envp_list);
	tmp = path;
	while (path && *path)
	{
		tmp_cmd = ft_strjoin(ft_strjoin(*path, "/"), cmd->content[0]);
		if (execve(tmp_cmd, cmd->content, envp) != -1)
		{
			free(tmp_cmd);
			break ;
		}
		free(tmp_cmd);
		path++;
	}
	free(tmp);
	handle_exec_error(cmd->content[0], data);
}
