#include "fdf.h"

size_t	ft_wordlen(const char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != c && str[i] != '\n')
		i++;
	return (i);
}

size_t	ft_count_words(const char *s, char c)
{
	size_t	word_count;
	int		in_word;
	size_t	i;

	word_count = 0;
	in_word = 0;
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && s[i] != '\n' && in_word == 0)
		{
			in_word = 1;
			word_count++;
		}
		else if (s[i] == c || s[i] == '\n')
			in_word = 0;
		i++;
	}
	return (word_count);
}

void	ft_free_split(char **arr, size_t index)
{
	while (index > 0)
	{
		index--;
		free(arr[index]);
	}
	free(arr);
}

char	*ft_allocate_word(const char *s, size_t len)
{
	char	*word;
	size_t	i;

	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split_1(const char *s, char c)
{
	char	**arr;
	size_t	i;
	size_t	words;
	size_t	len;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	words = 0;
	while (words < ft_count_words(s, c))
	{
		while (s[i] == c)
			i++;
		len = ft_wordlen(&s[i], c);
		arr[words] = ft_allocate_word(&s[i], len);
		if (!arr[words])
			return (ft_free_split(arr, words), NULL);
		i += len;
		words++;
	}
	arr[words] = NULL;
	return (arr);
}
