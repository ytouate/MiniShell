/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execSimpleCommand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:39:51 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 15:41:38 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char	*get_path(t_list *env_list, char *cmd)
{
	char	*path;
	char	**command_path;
	int		i;

	path = ft_get_env_val(env_list, "PATH");
	if (path == NULL || cmd == NULL)
		return (NULL);
	i = 0;
	command_path = ft_split(path, ':');
	while (command_path[i])
	{
		command_path[i] = ft_strjoin(command_path[i], "/");
		command_path[i] = ft_strjoin(command_path[i], cmd);
		if (access(command_path[i], F_OK) == 0)
			return (command_path[i]);
		i++;
	}
	return (NULL);
}

void	ft_error(char *arg, char *msg, int exit_code)
{
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	set_exit_code(exit_code);
}

void	ft_execute(t_command *command, t_vars *vars, t_contex contex)
{
	char	*command_path;

	if (command->flags[0] == NULL)
		return ;
	command_path = get_path(vars->env_list, command->flags[0]);
	if (command->flags[0][0] == '/' || command->flags[0][0] == '.')
		check_cmd(command, vars, contex);
	else if (command_path == NULL)
		ft_error(command->flags[0], " :command not found", COMMAND_NOT_FOUND);
	else
	{
		if (command->flags[0][0])
		{
			if (fork() == 0)
			{
				dup2(contex.fd_in, STDIN_FILENO);
				dup2(contex.fd_out, STDOUT_FILENO);
				execve(command_path, command->flags, vars->env);
				perror(command->flags[0]);
				exit(0);
			}
			wait(NULL);
		}
		else
			ft_error(command->flags[0], " :command not found",
				COMMAND_NOT_FOUND);
	}
}

void	run_excutable(t_command *command, t_vars *vars, t_contex contex)
{
	if (access(command->flags[0], F_OK | X_OK) == 0)
	{
		if (fork() == 0)
		{
			if (access(command->flags[0], F_OK | X_OK) == 0)
			{
				dup2(contex.fd_out, STDOUT_FILENO);
				dup2(contex.fd_in, STDIN_FILENO);
				execve(command->flags[0], command->flags, vars->env);
			}
			exit(EXIT_SUCCESS);
		}
		wait(NULL);
	}
	else
	{
		set_exit_code(PERMISSION_DENIED);
		perror(command->flags[0]);
	}
}

void	check_cmd(t_command *command, t_vars *vars, t_contex contex)
{
	int	fd[2];
	int	stats;

	if (command->flags[0][0] != '/')
		run_excutable(command, vars, contex);
	else if (access(command->flags[0], F_OK) == 0)
	{
		pipe(fd);
		if (fork() == 0)
		{
			dup2(contex.fd_out, STDOUT_FILENO);
			dup2(contex.fd_in, STDIN_FILENO);
			execve(command->flags[0], command->flags, vars->env);
			perror("execve");
			exit(PERMISSION_DENIED);
		}
		wait(&stats);
		set_exit_code(WEXITSTATUS(stats));
		return ;
	}
	set_exit_code(127);
	perror(command->flags[0]);
}
