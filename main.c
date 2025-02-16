#include "fdf.h"
#include <math.h>
#include <stdlib.h>
#include <ctype.h>


int key_hook(int keycode, t_data *data)
{
    if (keycode == 53 || keycode == 65307) // 53 - MacOS, 65307 - Linux
        close_window(data);
    return (0);
}


void init_window(t_data *data, int width, int height)
{
    data->mlx = mlx_init();
    if (!data->mlx)
        return;
    data->win = mlx_new_window(data->mlx, width, height, "FdF");
}

// Функция готовит карту и рисует её
void draw(t_map *map)
{
    t_data data;
    init_window(&data, 1200, 800);
    if (!data.win)
        return;
	write(1, "inited\n", 8);
    calculate_scale_and_offset(map, 1200, 800);
	write(1, "calculated offset\n", 19);
    convert_to_isometric(map);
	write(1, "converted to iso\n", 18);
    fit_map_in_window(map, 1200, 800);
    draw_map(data.mlx, data.win, map);
	write(1, "drawed\n", 8);
	free_map(map);

    mlx_hook(data.win, 17, 0, close_window, &data);
    mlx_key_hook(data.win, key_hook, &data);
    mlx_loop(data.mlx);
}

int main(int argc, char **argv)
{
	if(argc != 2)
		return 1;
	const char  *dest = argv[1];
	int         height = ft_map_height(dest);
	int         width = ft_map_width(dest);
	printf("%d // %d ", width, height);
	if (width <= 0)
	{
		printf("Error: Invalid map width %d\n", width);
		return (1);
	}
	if (height <= 0 )
	{
		printf("Error: Invalid map height!\n");
		return (1);
	}
	t_map *mapa = init_map(width, height);
	if (!mapa)
	{
		printf("Error: Memory allocation failed!\n");
		return (1);
	}

	if (!add_points(mapa, dest))
	{
		printf("Error: Failed to load points!\n");
		free_map(mapa);
		return (1);
	}
	draw(mapa);
	return (0);
}

//size_line = 8 //  2x2 == [][][][] [][][][] [][][][] [][][][]  size_line: 2 пикселя * 4 байта/пиксель = 8 байт.
// buffer [y * size_line + x * (bits_per_pixel / 8)]  y=0,x=1 // offset == 0 * 8(size_line) + 1 * 4 = 4 ---> offset/4 == 1;
												//    y=1,x=1 // offset == 1 * 8 + 1 * 4 = 12 ---> offset/4 == 3;