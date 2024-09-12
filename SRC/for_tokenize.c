/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   for_tokenize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmheidat <mmheidat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 13:59:40 by mmheidat          #+#    #+#             */
/*   Updated: 2024/09/11 14:49:20 by mmheidat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

void	handle_split_error(char **split, int i)
{
	free_split_memory(split, i);
}

char	**allocate_the_cmd(int size)
{
	char	**the_cmd;

	the_cmd = (char **)malloc(sizeof(char *) * (size + 1));
	return (the_cmd);
}

void	process_command_chunk(char *split_part, t_cmd **cmd)
{
	t_files	*files;
	char	**splitted;
	char	**the_cmd;

	files = NULL;
	splitted = ft_split_without_quotes(split_part, ' ');
	if (!splitted)
		return ;
	the_cmd = allocate_the_cmd(size_of_content(splitted));
	if (!the_cmd)
		return ;
	process_split_part(splitted, &files, the_cmd);
	free_double_pointer(splitted);
	ft_cmd_lstadd_back(cmd, ft_cmd_lstnew(the_cmd, files));
	(*cmd)->pid = 0;
}
