#include "fdf.h"


int ft_len_of_map(int fd)
{
	int len = 0;
	char *line;
	int i;

	line = get_next_line(fd);
	while (line)
	{
		i = 0;
		while (line[i])
		{
			while (line[i] == ' ')
				i++;

			if (line[i] && line[i] != ' ')
			{
				len++; // Count a point
				while (line[i] && line[i] != ' ')
					i++;
			}
		}
		free(line);
		line = get_next_line(fd);
	}
	return (len);
}

void add_points_1(t_map *newmap, int fd)
{
	char	*line;
	char	**s_line;
	int		y = 0;
	int		index;
	int		point_index;

	line = get_next_line(fd);
	while (line)
	{
		s_line = ft_split(line, ' ');
		if (!s_line)
		{
			free(line);
			break;
		}

		index = 0;
		while (s_line[index])
		{
			point_index = y * newmap->width + index; // Correct indexing
			newmap->points[point_index].x = index;
			newmap->points[point_index].y = y;
			newmap->points[point_index].z = atoi(s_line[index]);
			index++;
		}

		// Free memory
		free(line);

		// Read next line
		y++;
		line = get_next_line(fd);
	}
}

// int main()
// {
// 	int		fd;
// 	int		len;
// 	t_map	newmap;
// 	newmap.width = 19;
// 	newmap.height = 11;
// 	newmap.points = malloc(sizeof(t_point) * 209);

// 	fd = open("maps/test_maps/42.fdf", O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	add_points(&newmap,fd);
// 	int i;
// 	i = 0;
// 	while (i < 209)
// 	{
// 		printf("z = %d \n",newmap.points[i].z);
// 		i++;
// 	}
	
	
// }