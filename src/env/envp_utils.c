/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:30:15 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/24 17:37:24 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_add_env_hash(t_hmap **hashmap, char *key, char *value)
{
	t_hmap	*current;

	if (!hashmap || !key)
		return ;
	current = *hashmap;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	add_new_var(hashmap, key, value);
}

void	ft_remove_env_hash(t_hmap **hashmap, char *key)
{
	t_hmap	*prev;
	t_hmap	*current;

	if (!hashmap || !key)
		return ;
	current = *hashmap;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*hashmap = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	free_hashmap(t_hmap *hashmap)
{
	t_hmap	*current;
	t_hmap	*next;

	current = hashmap;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

int	key_exists(t_hmap *v, char *ekey)
{
	while (v)
	{
		if (ft_strcmp(v->key, ekey) == 0)
			return (1);
		v = v->next;
	}
	return (0);
}

char	*return_value_hash(t_hmap *v, char *key)
{
	if (key_exists(v, key) == 1)
	{
		while (v)
		{
			if (ft_strcmp(v->key, key) == 0)
				return (v->value);
			v = v->next;
		}
	}
	return (NULL);
}
