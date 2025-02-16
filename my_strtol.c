#include "fdf.h"

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

int ft_hex_digit(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;  // Invalid character
}

int	my_strtol_process(const char *str)
{
	int	result;
	int	digit;
	int	i;

	result = 0;
	i = 0;
	while (str[i])
	{
		digit = ft_hex_digit(str[i]);
		if (digit == -1)
			break ;
		result = result * 16 + digit;
		i++;
	}
	return (result);
}


int	my_strtol(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	if (!str)
		return (0);
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
		i += 2;
	result = my_strtol_process(str + i);
	return (result * sign);
}