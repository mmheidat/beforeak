/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:26:05 by mbamatra          #+#    #+#             */
/*   Updated: 2024/09/11 21:24:07 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

int	check_current(t_cmd *cmd, t_data data)
{
	open_heredoc_first(cmd->files, &data);
	handle_redirection(cmd->files);
	if (ft_strcmp(cmd->content[0], "exit") == 0)
		(printf("exit\n"), exit(0));
	else if (ft_strcmp(cmd->content[0], "export") == 0)
		return (export(&data, cmd), 0);
	else if (ft_strcmp(cmd->content[0], "clear") == 0)
		return (system("clear"), 0);
	else if (ft_strcmp(cmd->content[0], "env") == 0)
		return (add_temp_env(&data, cmd), 0);
	else if (ft_strcmp(cmd->content[0], "cd") == 0)
		return (cd(cmd, &data), 0);
	else if (ft_strcmp(cmd->content[0], "echo") == 0)
		return (ft_echo(cmd->content), 0);
	else if (ft_strcmp(cmd->content[0], "pwd") == 0)
		return (ft_pwd(), 0);
	else if (ft_strcmp(cmd->content[0], "unset") == 0)
		return (remove_env(&data.export_list, cmd),
			remove_env(&data.envp_list, cmd), 0);
	return (1);
}

void	handle_command_execution(t_cmd *cmd, char **envp,
	t_data *data, int *tmp_fd)
{
	int	i;

	i = 1;
	if (!cmd->next)
	{
		if (check_built(cmd, *data) == 0)
			i = check_current(cmd, *data);
	}
	if (i == 1)
		ft_execute(cmd, envp, data, tmp_fd);
}

void	process_command(t_data *data, char **envp, int *tmp_fd)
{
	data->line = join_spaces(data->line);
	if (validate_line(data))
		return ;
	execute_splitted_commands(data, envp, tmp_fd);
}

// void	process_command(t_data *data, char **envp, int *tmp_fd)
// {
// 	char		**split;

// 	data->line = join_spaces(data->line);
// 	if (!data->line)
// 		return ;
// 	if (check_if_all_spaces(data->line))
// 	{
// 		free(data->line);
// 		return ;
// 	}
// 	split = ft_split_without_quotes(data->line, '|');
// 	if (!split)
// 	{
// 		free(data->line);
// 		if (data->cmd)
// 			free_cmd_memory(&data->cmd);
// 		return ;
// 	}
// 	data->cmd = fill_command(split);
// 	if (!data->cmd)
// 	{
// 		free_split_memory(split, size_of_content(split));
// 		free(data->line);
// 		return ;
// 	}
// 	free_double_pointer(split);
// 	expand_variables(&data->cmd, data);
// 	do_exclude_quotes(data->cmd, data);
// 	handle_command_execution(data->cmd, envp, data, tmp_fd);
// 	free_cmd_memory(&data->cmd);
// 	free(data->line);
// }

void	setup_and_process(t_data *data, char **envp, int *tmp_fd)
{
	tmp_fd[0] = dup(STDIN_FILENO);
	tmp_fd[1] = dup(STDOUT_FILENO);
	data->line = readline("minishell$ ");
	if (!data->line)
	{
		printf("exit\n");
		free_data(data);
		exit(0);
	}
	add_history(data->line);
	if (*data->line)
	{
		initialize_quote(&data->quotes);
		if (parsing(data->line) == 1)
		{
			handle_error(data);
			free(data->line);
		}
		else
			process_command(data, envp, tmp_fd);
	}
	else
		free(data->line);
	dup2(tmp_fd[0], STDIN_FILENO);
	dup2(tmp_fd[1], STDOUT_FILENO);
}

int	main(int ac, char **av, char **envp)
{
	int			tmp_fd[2];
	t_data		data;

	(void)ac;
	(void)av;
	setup_signal_handlers();
	data_init(&data);
	init_env(&data, envp);
	while (1)
		setup_and_process(&data, envp, tmp_fd);
	free_data(&data);
	return (0);
}
