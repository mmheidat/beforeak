/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:05:40 by mmheidat          #+#    #+#             */
/*   Updated: 2024/09/11 21:46:13 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	validate_line(t_data *data)
{
	if (!data->line || check_if_all_spaces(data->line))
	{
		free(data->line);
		return (1);
	}
	return (0);
}

void	execute_splitted_commands(t_data *data, char **envp, int *tmp_fd)
{
	char	**split;

	split = ft_split_without_quotes(data->line, '|');
	if (!split)
	{
		handle_splitted_error(data);
		return ;
	}
	data->cmd = fill_command(split);
	if (!data->cmd)
	{
		free_split_memory(split, size_of_content(split));
		free(data->line);
		return ;
	}
	handle_execution_and_cleanup(data, envp, tmp_fd, split);
}

void	handle_splitted_error(t_data *data)
{
	free(data->line);
	if (data->cmd)
		free_cmd_memory(&data->cmd);
}

void	handle_execution_and_cleanup(t_data *data, char **envp,
			int *tmp_fd, char **split)
{
	free_double_pointer(split);
	expand_variables(&data->cmd, data);
	do_exclude_quotes(data->cmd, data);
	handle_command_execution(data->cmd, envp, data, tmp_fd);
	free_cmd_memory(&data->cmd);
	free(data->line);
}
