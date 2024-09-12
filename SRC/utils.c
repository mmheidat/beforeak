/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbamatra <mbamatra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 03:43:08 by mbamatra          #+#    #+#             */
/*   Updated: 2024/09/11 21:51:41 by mbamatra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"

static	int	count_without_quotes(char *s)
{
	int			count;
	int			i;
	t_quotes	inside_quotes;

	initialize_quote(&inside_quotes);
	i = 0;
	count = 0;
	if (s[i] == '\0')
		return (0);
	count = ft_strlen(s);
	while (s[i])
	{
		update_quotes(&inside_quotes, s[i]);
		if (s[i] == '\'' && inside_quotes.double_quote == 0)
			count--;
		else if (s[i] == '\"' && inside_quotes.single_quote == 0)
			count--;
		i++;
	}
	return (count);
}

void	remove_quotes_while_updating(int *index, char *str,
		t_quotes *quote_state, char **new_str)
{
	int	nstr_index;

	nstr_index = 0;
	while (str[*index] && update_quotes(quote_state, str[*index]))
	{
		if ((str[*index] == '\"' && quote_state->single_quote == 0)
			|| (str[*index] == '\'' && quote_state->double_quote == 0))
		{
			(*index)++;
			continue ;
		}
		(*new_str)[nstr_index] = str[*index];
		nstr_index++;
		(*index)++;
	}
	(*new_str)[nstr_index] = '\0';
}

char	*ft_exclude_quotes(char *str)
{
	int				index;
	char			*new_str;
	t_quotes		quote_state;

	if (!str)
		return (NULL);
	index = 0;
	new_str = malloc(count_without_quotes(str) + 1);
	initialize_quote(&quote_state);
	if (!new_str)
		return (NULL);
	remove_quotes_while_updating(&index, str, &quote_state, &new_str);
	return (new_str);
}

void	do_exclude_quotes(t_cmd *command, t_data *data)
{
	int		i;
	char	*temp;
	t_files	*files;

	(void)data;
	while (command)
	{
		files = command->files;
		i = -1;
		while (command && command->content && command->content[++i])
		{
			temp = command->content[i];
			command->content[i] = ft_exclude_quotes(temp);
			free(temp);
		}
		while (files && files->file_name)
		{
			temp = files->file_name;
			files->file_name = ft_exclude_quotes(temp);
			free(temp);
			files = files->next;
		}
		if (command)
			command = command->next;
	}
}

void	ft_split_helper2(const char *s, char c, t_quotes inside_quotes)
{
	while (*s && ((*s != c) || inside_quotes.single_quote
			|| inside_quotes.double_quote))
	{
		update_quotes(&inside_quotes, *s);
		s++;
	}
	return ;
}
