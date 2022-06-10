/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 15:50:39 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		//rl_replace_line("", 0);
		rl_redisplay();
		set_exit_code(130);
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *cmd;

	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	vars->env = env;
	vars->env_list = get_env_list(vars->env);
	vars->export_list = get_env_list(vars->env);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler);
	while (true)
	{
		cmd = get_promt();
		if (cmd == NULL)
			exit(130);
		cmd = ft_strtrim(cmd, " ");
		if (*cmd)
		{
			vars->head = ft_get_for_exec(cmd, vars->env_list);
			if (vars->head != NULL)
			{
				vars->command = vars->head->first_c;
				vars->num_of_commands = get_len(vars->command);
				if (vars->command != NULL)
				{
					replace_symbol_by_val(vars->head->first_c->flags, vars->env_list);
					ft_pipe(vars);
				}
			}
		}
	}
}