#include "minishell.h"

void ft_add_env_hash(t_hmap **hashmap, char *key, char *value)
{
    if (!hashmap || !key)
        return;

    t_hmap *current =*hashmap;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = ft_strdup(value);
            return;
        }
        current = current->next;
    }
    add_new_var(hashmap, key, value);
}

void ft_remove_env_hash(t_hmap **hashmap, char *key)
{
    if (!hashmap || !key)
        return;

    t_hmap *current = *hashmap;
    t_hmap *prev = NULL;

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
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_hashmap(t_hmap *hashmap)
{
    t_hmap *current;
    t_hmap *next;

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

int key_exists(t_hmap *v, char *ekey)
{
    while (v)
    {
        if (ft_strcmp(v->key, ekey) == 0)
            return (1);
        v = v->next;
    }
    return (0);
}

char *return_value_hash(t_hmap *v, char *key)
{
    if (key_exists(v, key) == 1)
    {
        while (v)
        {
            if (ft_strcmp(v->key, key) == 0)
            {
                printf("Found key: %s, value: %s\n", key, v->value);
                return (v->value);
            }
            v = v->next;
        }
    }
    printf("Key not found: %s\n", key);
    return (NULL);
}

static t_hmap *hmap_new(char *akey, char *avalue)
{
	t_hmap *node;

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

t_hmap **init_hmap(char **env)
{
	char *hash_key;
	t_hmap **hashmap;
	int j;
	int i;
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
		strncpy(hash_key, env[i], j);
		hash_key[j] = '\0';
		add_new_var(hashmap, hash_key, ft_strchr(env[i], '=') + 1);
		free(hash_key);
		i++;
	}
	return (hashmap);
}

void add_new_var(t_hmap **v, char *akey, char *avalue)
{
	t_hmap *node;
	t_hmap *temp;

	if (akey[0] == ' ')
		node = hmap_new(akey + 1, NULL);
	else
		node = hmap_new(akey, avalue);
	if (!node)
		return;
	if (*v == NULL)
		*v = node;
	else
	{
		temp = *v;
		while (temp->next)
			temp = temp->next;
		temp->next = node;
	}
	printf("Added key: %s, value: %s\n", akey, avalue);
}


// char	**dup_envp(char **envp)
// {
// 	char	**env;
// 	int		len;
// 	int		i = 0;

// 	len = ft_size((void **)envp);
// 	env = ft_calloc(len + 1, sizeof(char *));
// 	if (!env)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		env[i] = ft_strdup(envp[i]);
// 		if (!env[i])
// 		{
// 			free_arr_2d((void *)env);
// 			return (NULL);
// 		}
// 		i++;
// 	}
// 	i = 0;
// // !
// 	// while(i < len && env[i] != NULL)
// 	// {
// 	// 	printf("%s\n", env[i]);
// 	// 	i++;
// 	// }
// 	return (env);
// }
