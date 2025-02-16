#include "fdf.h"

void	set_point(t_map *map, int x, int y, int z_val, int color)
{
	int	point_index;

	point_index = y * map->width + x;
	map->points[point_index] = (t_point){x, y, z_val, color, 0, 0};
	if (z_val < map->z_min)
		map->z_min = z_val;
	if (z_val > map->z_max)
		map->z_max = z_val;
}

void	process_line(t_map *newmap, char **s_line, int y)
{
	int		index;
	int		z_val;
	int		actual_color;
	char	**colored_line;

	index = 0;
	actual_color = newmap->color;
	while (s_line[index] && index < newmap->width)
	{
		colored_line = NULL;
		z_val = atoi(s_line[index]);
		if (color_check(s_line[index]))
		{
			colored_line = ft_split_1(s_line[index], ',');
			z_val = atoi(colored_line[0]);
			actual_color = my_strtol(colored_line[1]);
			free_spl(colored_line);
		}
		set_point(newmap, index, y, z_val, actual_color);
		index++;
	}
}


int add_points(t_map *newmap, const char *dest)
{
    char *line;
    char **s_line;
    int y = 0;
    int fd = open(dest, O_RDONLY);
    if (fd == -1) return (0);

    newmap->z_min = 999999;
    newmap->z_max = -999999;

    while ((line = get_next_line(fd)))
    {
        s_line = ft_split_1(line, ' ');
        free(line);
        if (!s_line)
			return (close(fd), 0);

        process_line(newmap, s_line, y);
        free_spl(s_line);
        y++;
    }
    close(fd);
    return (1);
}