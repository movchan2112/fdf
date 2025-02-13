#ifndef FDF_H
# define FDF_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# include "minilibx-linux/mlx.h"


typedef struct s_point{
	int x;
	int y;
	int z;
	int color;
} t_point;

typedef struct s_img
{
	void    *img;
	char    *addr;
	int     bpp;
	int     line_length;
	int     endian;
}   t_img;

typedef struct s_data
{
	void	*mlx;
	void	*win;
}	t_data;

typedef struct s_map
{
	int width;       // Количество колонок (X)
	int height;      // Количество строк (Y)
	t_point *points; // 1D массив точек (размер: width * height)

	int z_min;       // Минимальная высота (используется для нормализации)
	int z_max;       // Максимальная высота (используется для нормализации)

	float scale;     // Масштаб карты
	int offset_x;    // Смещение по X (центрирование)
	int offset_y;    // Смещение по Y (центрирование)

	int color;       // Цвет точек (по умолчанию белый: 0xFFFFFF)

}   t_map;




char	*get_next_line(int fd);
char	**ft_split(char const *s, char c);


#endif