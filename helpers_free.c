#include "fdf.h"

void	free_spl(char **line)
{
	int i;

	if (!line)
		return;
	i = 0;
	while (line[i])
		free(line[i++]);
	free(line);
}

int cleanup(int fd, char *line, int error_code)
{
	if (line)
		free(line);
	if (fd >= 0)
		close(fd);
	return (error_code);
}

void free_map(t_map *mapa)
{
	if (mapa)
	{
		free(mapa->points);
		free(mapa);
	}
}

int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx, data->win);
	free(data->mlx);
	
    exit(0); // Завершаем программу
    return (0);
}

int		color_check(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if(s[i] == ',')
			return(1);
		i++;
	}
	return(0);
}