#include "fdf.h"

int	get_width(char *line)
{
	int		width;
	char	**spl_line;

	if (!line)
		return (0);

	spl_line = ft_split_1(line, ' ');
	if (!spl_line)
		return (0);

	width = 0;
	while (spl_line[width])
		width++;

	free_spl(spl_line);
	return (width);
}

// Функция для проверки ширины карты
int	ft_map_width(const char *dest)
{
	int		fd;
	int		width;
	int		next_line_width;
	char	*line;

	fd = open(dest, O_RDONLY);
	if (fd == -1)
		return (perror("Error reading file"), 0);

	line = get_next_line(fd);
	if (!line)
		return (cleanup(fd, NULL, 0));

	width = get_width(line);
	free(line);

	while ((line = get_next_line(fd)))
	{
		next_line_width = get_width(line);
		if (width != next_line_width)
			return (cleanup(fd, line, -1));
		free(line);
	}
	return (cleanup(fd, NULL, width));
}

int	ft_map_height(const char *dest)
{
	int		fd;
	int		height;
	char	*line;

	fd = open(dest, O_RDONLY);
	if (fd == -1)
		return (perror("Error reading file"), 0);

	height = 0;
	while ((line = get_next_line(fd)))
	{
		height++;
		free(line);
	}
	
	return (close(fd), height);
}

t_map	*init_map(int width, int height)
{
	t_map *mapa = malloc(sizeof(t_map));
	if (!mapa)
		return (NULL);

	mapa->width = width;
	mapa->height = height;
	mapa->z_min = 999;
	mapa->z_max = -999;
	mapa->scale = 10.0;
	mapa->offset_x = 500;
	mapa->offset_y = 300;
	mapa->color = 0xFFFFFF;

	mapa->points = malloc(sizeof(t_point) * width * height);
	if (!mapa->points)
	{
		free(mapa);
		return (NULL);
	}

	return (mapa);
}