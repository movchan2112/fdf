#include <stdlib.h>
#include "fdf.h"

#include <stdlib.h>

size_t	ft_strlen1(const char *str)
{
	size_t	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i = 0;
	if (size != 0)
	{
		while (src[i] != '\0' && i < (size - 1))
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (ft_strlen1(src));
}

size_t	ft_wordlen(const char *str, char c)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != c && str[i] != '\n') // Не учитываем \n
		i++;
	return (i);
}

size_t	ft_count_words(char const *s, char c)
{
	size_t	word_count;
	size_t	n;
	int		in_word;

	word_count = 0;
	n = 0;
	in_word = 0;
	while (s[n] != '\0')
	{
		if (s[n] != c && s[n] != '\n' && in_word == 0) // Пропускаем пробелы и \n
		{
			in_word = 1;
			word_count++;
		}
		else if (s[n] == c || s[n] == '\n') // Прерываем слово
			in_word = 0;
		n++;
	}
	return (word_count);
}


void	ft_free_split(char **arr, size_t index)
{
	while (index > 0)
		free(arr[--index]);
	free(arr);
}

char	*ft_mall(const char *src, size_t len)
{
	char *arr = (char *)malloc(sizeof(char) * (len + 1));
	if (!arr)
		return (NULL);
	ft_strlcpy(arr, src, len + 1);
	return (arr);
}

char	**ft_split(const char *s, char c)
{
	size_t	i = 0, word_count = ft_count_words(s, c);
	char	**arr = (char **)malloc(sizeof(char *) * (word_count + 1));

	if (!s || !arr)
		return (NULL);

	for (size_t words = 0; words < word_count; words++)
	{
		while (s[i] == c)
			i++;
		size_t len = ft_wordlen(&s[i], c);
		arr[words] = ft_mall(&s[i], len);
		if (!arr[words])
		{
			ft_free_split(arr, words);
			return (NULL);
		}
		i += len;
	}
	arr[word_count] = NULL;
	return (arr);
}

