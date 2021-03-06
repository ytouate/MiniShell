/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellparse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:52:46 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/15 22:05:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

int	ft_check_pipe(t_lexer *lexer, t_token *token, int k, t_head_c *head)
{
	free(token);
	ft_skip_spaces(lexer);
	if (lexer->content[lexer->i] == '\0' || k == 0)
	{
		ft_free_all(head);
		printf("minishell:syntax error\n");
		set_exit_code(SYNTAX_ERROR_EXIT);
		return (1);
	}
	return (0);
}

int	ft_check_token(t_token *token, t_command *re, int *i, t_head_c *head)
{
	if (token->token == 0)
	{
		if (ft_syntax(token->value, token, head) == 1)
			return (1);
		re->flags = ft_replace(re->flags, *i, token->value);
		*i += 1;
		free(token->value);
		free(token);
	}
	else if (token->token >= 1 && token->token <= 4)
	{
		if (ft_rederictions(re, token, head) == 1)
			return (1);
	}
	return (0);
}

int	ft_fill_node(t_command *re, t_lexer *lexer, \
	t_list *env_list, t_head_c *head)
{
	int			k;
	int			i;
	t_token		*token;

	token = ft_get_next_token(lexer, env_list);
	k = 0;
	i = 1;
	while (token)
	{
		if (token->token < 5)
		{
			if (ft_check_token(token, re, &i, head) == 1)
				return (1);
		}
		else if (token->token == 5)
		{
			if (ft_check_pipe(lexer, token, k, head) == 1)
				return (1);
			break ;
		}
		k++;
		token = ft_get_next_token(lexer, env_list);
	}
	return (0);
}

int	ft_add_commande(t_head_c *head, t_lexer *lexer, t_list *env_list)
{
	t_command	*re;
	int			i;

	re = malloc(sizeof(t_command));
	re->redi = malloc(sizeof(t_token_head));
	re->herdoc = malloc(sizeof(t_token_head));
	re->herdoc->first_token = NULL;
	re->redi->first_token = NULL;
	re->flags = malloc(sizeof(char *));
	re->flags[0] = NULL;
	if (ft_fill_node(re, lexer, env_list, head) == 1)
	{
		free(re->redi);
		free(re->herdoc);
		i = -1;
		while (re->flags[++i])
			free(re->flags[i]);
		free(re->flags);
		free(re);
		return (1);
	}
	ft_add_node(head, re);
	return (0);
}

t_head_c	*ft_get_for_exec(char *content, t_list *env_list)
{
	t_head_c	*head_of_command;
	t_lexer		*lexer;
	int			s;

	head_of_command = malloc(sizeof(t_head_c));
	ft_init_head(head_of_command);
	lexer = ft_init_lexer(content);
	while (lexer->content[lexer->i])
	{
		s = ft_add_commande(head_of_command, lexer, env_list);
		if (s == 1)
		{
			free(lexer);
			return (NULL);
		}
	}
	free(lexer);
	return (head_of_command);
}
