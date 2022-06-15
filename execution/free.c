/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:38:58 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/14 12:11:17 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/Users/ilefhail/Desktop/MiniShell/MiniShell.h"

void	free_2d_array(char **a)
{
	int	i;

	i = 0;
	while (a[i])
		free(a[i++]);
	free(a);
}

void	free_list(t_list *list)
{
	t_list	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->content);
		free(temp);
		temp->content = NULL;
		temp = NULL;
	}
}
