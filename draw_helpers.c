#include "fdf.h"

float calculate_z_factor(t_map *map)
{
	float K = 10.0;  // Базовый коэффициент высоты (можно менять)
	int z_range = map->z_max - map->z_min + 1;  // Разница высот
	float z_factor = K / z_range;

	// Ограничиваем z_factor, чтобы карта не была слишком высокой
	if (z_factor > 10000) 
		z_factor = 10000;
	if (z_factor < 0.001) 
		z_factor = 0.001;

	return z_factor;
}


void iso_projection(t_point *p, t_map *map)
{
    float normalized_x = p->x - (map->width / 2);
    float normalized_y = p->y - (map->height / 2);
    float z_factor = calculate_z_factor(map);

    p->x_iso = (normalized_x - normalized_y) * 0.866 * map->scale + map->offset_x;
    p->y_iso = (normalized_x + normalized_y) * 0.5 * map->scale - (p->z * map->scale * z_factor) + map->offset_y;
}


void calculate_scale_and_offset(t_map *map, int win_width, int win_height)
{
    float scale_x = (float)win_width / (map->width * 2);
    float scale_y = (float)win_height / (map->height * 2);

    // Выбираем наименьший масштаб
    map->scale = (scale_x < scale_y) ? scale_x : scale_y;

    // Ограничиваем минимальный масштаб, чтобы избежать очень маленьких значений
    if (map->scale < 1) 
        map->scale = 1.3;  

    // Устанавливаем смещение
    map->offset_x = win_width / 2;
    map->offset_y = win_height / 3;
}


void convert_to_isometric(t_map *map)
{
    for (int i = 0; i < map->width * map->height; i++)
    {
        // Сбрасываем `x_iso` и `y_iso`, чтобы избежать накопления ошибок
        map->points[i].x_iso = map->points[i].x;
        map->points[i].y_iso = map->points[i].y;

        // Теперь делаем преобразование
        iso_projection(&map->points[i], map);
    }
}


// Функция вычисляет границы карты
void get_map_bounds(t_map *map, int *min_x, int *max_x, int *min_y, int *max_y)
{
    int i;
    *min_x = 999999;
    *max_x = -999999;
    *min_y = 999999;
    *max_y = -999999;

    for (i = 0; i < map->width * map->height; i++)
    {
        if (map->points[i].x_iso < *min_x) *min_x = map->points[i].x_iso;
        if (map->points[i].x_iso > *max_x) *max_x = map->points[i].x_iso;
        if (map->points[i].y_iso < *min_y) *min_y = map->points[i].y_iso;
        if (map->points[i].y_iso > *max_y) *max_y = map->points[i].y_iso;
    }
}