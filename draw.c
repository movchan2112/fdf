#include "fdf.h"

void fit_map_in_window(t_map *map, int win_width, int win_height)
{
    int min_x, max_x, min_y, max_y;

    get_map_bounds(map, &min_x, &max_x, &min_y, &max_y);

    // while ((max_x - min_x) > win_width || (max_y - min_y) > win_height)
    // {
    //     map->scale *= 0.9;
        
    //     // ✅ Сбрасываем координаты перед пересчётом
    //     for (int i = 0; i < map->width * map->height; i++) {
    //         map->points[i].x_iso = map->points[i].x;
    //         map->points[i].y_iso = map->points[i].y;
    //     }

    //     convert_to_isometric(map);
    //     get_map_bounds(map, &min_x, &max_x, &min_y, &max_y);
    // }

    int shift_x = (win_width / 2) - ((min_x + max_x) / 2);
    int shift_y = (win_height / 2) - ((min_y + max_y) / 2);
	int i = 0;
    while (i < map->width * map->height)
    {
        map->points[i].x_iso += shift_x;
        map->points[i].y_iso += shift_y;
		i++;
    }
}



void put_pixel_to_image(t_img *img, int x, int y, int color)
{
	char *pixel;

	if (x < 0 || x >= 1200 || y < 0 || y >= 800) // Проверяем границы окна
		return;

	pixel = img->addr + (y * img->line_length + x * (img->bpp / 8));
	*(unsigned int *)pixel = color;
}

void draw_line(t_img *img, t_point a, t_point b, int color)
{
	int dx = abs(b.x_iso - a.x_iso);
	int dy = abs(b.y_iso - a.y_iso);
	int sx = (a.x_iso < b.x_iso) ? 1 : -1;
	int sy = (a.y_iso < b.y_iso) ? 1 : -1;
	int err = dx - dy;

	while (1)
	{
		put_pixel_to_image(img, (int)a.x_iso, (int)a.y_iso, color);
		if (a.x_iso == b.x_iso && a.y_iso == b.y_iso)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; a.x_iso += sx; }
		if (e2 < dx) { err += dx; a.y_iso += sy; }
	}
}

void draw_map(void *mlx, void *win, t_map *map)
{
	t_img img;

	img.img = mlx_new_image(mlx, 1200, 800);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_length, &img.endian);

	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; x < map->width; x++)
		{
			int index = y * map->width + x;
			
			if (x < map->width - 1)
				draw_line(&img, map->points[index], map->points[index + 1], map->points[index].color);
			if (y < map->height - 1)
				draw_line(&img, map->points[index], map->points[index + map->width], map->points[index].color);
		}
	}

	mlx_put_image_to_window(mlx, win, img.img, 0, 0); // Выводим весь буфер за 1 вызов
	mlx_destroy_image(mlx, img.img);
}