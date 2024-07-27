/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_envp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 16:25:21 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/07/27 08:25:04 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static	t_hmap	*hmap_new(char *akey, char *avalue)
{
	t_hmap	*node;

	if (!akey)
		return (NULL);
	node = (t_hmap *)malloc(sizeof(t_hmap));
	if (!node)
	{
		ft_putstr_fd("Memory allocation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	node->key = ft_strdup(akey);
	if (!avalue)
		node->value = ft_strdup("");
	else
		node->value = ft_strdup(avalue);
	node->next = NULL;
	return (node);
}

t_hmap	**init_hmap(char **env)
{
	char	*hash_key;
	t_hmap	**hashmap;
	int		j;
	int		i;

	hashmap = (t_hmap **)malloc(sizeof(t_hmap *));
	if (!hashmap)
		exit(EXIT_FAILURE);
	*hashmap = NULL;
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] != 0 && env[i][j] != '=')
			j++;
		hash_key = malloc(j + 1);
		if (!hash_key)
			exit(1);
		ft_strncpy(hash_key, env[i], j);
		hash_key[j] = '\0';
		add_new_var(hashmap, hash_key, ft_strchr(env[i], '=') + 1);
		free(hash_key);
		i++;
	}
	return (hashmap);
}

void	add_new_var(t_hmap **v, char *akey, char *avalue)
{
	t_hmap	*node;
	t_hmap	*temp;

	if (akey[0] == ' ')
		node = hmap_new(akey + 1, NULL);
	else
		node = hmap_new(akey, avalue);
	if (!node)
		return ;
	if (*v == NULL)
		*v = node;
	else
	{
		temp = *v;
		while (temp->next)
			temp = temp->next;
		temp->next = node;
	}
}
