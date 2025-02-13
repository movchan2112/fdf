#include "fdf.h"
#include <math.h>
#include <stdlib.h>
#include <ctype.h>

// Освобождение памяти для split
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

// Функция для получения ширины строки
int	get_width(char *line)
{
	int		width;
	char	**spl_line;

	if (!line)
		return (0);

	spl_line = ft_split(line, ' ');
	if (!spl_line)
		return (0);

	width = 0;
	while (spl_line[width]) // ✅ Теперь корректно считаем количество слов
		width++;

	free_spl(spl_line);
	return (width);
}

// Функция для очистки памяти и закрытия файла при ошибке
int cleanup(int fd, char *line, int error_code)
{
	if (line)
		free(line);
	if (fd >= 0)
		close(fd);
	return (error_code);
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
		printf("Width: %d, Next: %d\n", width, next_line_width);
		if (width != next_line_width)
			return (cleanup(fd, line, -1)); // ✅ Теперь нет утечки памяти
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


void free_map(t_map *mapa)
{
	if (mapa)
	{
		free(mapa->points);
		free(mapa);
	}
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

t_map	*init_map(int width, int height)
{
	t_map *mapa = malloc(sizeof(t_map));
	if (!mapa)
		return (NULL);

	mapa->width = width;
	mapa->height = height;
	mapa->z_min = 999;
	mapa->z_max = -999;
	mapa->scale = 10.0;  // Базовый масштаб (можно динамически пересчитывать)
	mapa->offset_x = 500; // Центрирование карты по X
	mapa->offset_y = 300; // Центрирование карты по Y
	mapa->color = 0xFFFFFF; // Белый цвет по умолчанию

	mapa->points = malloc(sizeof(t_point) * width * height);
	if (!mapa->points)
	{
		free(mapa);
		return (NULL);
	}

	return (mapa);
}

int my_strtol(const char *str) {
    if (str == NULL) {
        return 0; // Обработка NULL
    }

    int result = 0;
    int sign = 1;
    int i = 0;

    // Пропускаем пробелы
    while (isspace(str[i])) {
        i++;
    }

    // Проверяем знак
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Обрабатываем возможный префикс "0x" или "0X"
    if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X')) {
        i += 2;
    }

    // Парсим шестнадцатеричные цифры
    while (str[i]) {
        char c = str[i];
        int digit;

        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            break; // Останавливаемся, если символ не входит в диапазон шестнадцатеричных
        }

        result = result * 16 + digit;
        i++;
    }

    return result * sign;
}


// int add_points(t_map *newmap, const char *dest)
// {
// 	char    *line;
// 	char    **s_line;
// 	int     y = 0;
// 	int     index;
// 	int     point_index;
// 	int     fd;
// 	char	**colored_line;

// 	fd = open(dest, O_RDONLY);
// 	if (fd == -1)
// 		return (0);

// 	// Инициируем z_min и z_max осмысленными значениями
// 	newmap->z_min = 999999;  
// 	newmap->z_max = -999999;

// 	while ((line = get_next_line(fd)))
// 	{
// 		s_line = ft_split(line, ' ');
// 		free(line);
// 		if (!s_line)
// 			return (close(fd), 0);

// 		index = 0;
// 		while (s_line[index] && index < newmap->width)
// 		{
// 			int z_val;
// 			if(color_check(s_line[index]))
// 			{
// 				colored_line = ft_split(s_line[index], ',');
// 				z_val = atoi(colored_line[0]);
// 				newmap->points[point_index].z = z_val;
// 				newmap->points[point_index].color = my_strtol(colored_line[1]);
// 				free_spl(colored_line);

// 			}
// 			else
// 			{
// 				z_val = atoi(s_line[index]);
// 				newmap->points[point_index].z = z_val;
// 			}
// 			point_index = y * newmap->width + index;
// 			newmap->points[point_index].x = index;
// 			newmap->points[point_index].y = y;
// 			newmap->points[point_index].color = newmap->color;

// 			// Обновляем z_min и z_max
// 			if (z_val < newmap->z_min)
// 				newmap->z_min = z_val;
// 			if (z_val > newmap->z_max)
// 				newmap->z_max = z_val;

// 			index++;
// 		}
// 		free_spl(s_line);
// 		y++;
// 	}
// 	close(fd);
// 	return (1);
// }

void process_line(t_map *newmap, char **s_line, int y)
{
    int index = 0;
    int point_index;
    while (s_line[index] && index < newmap->width)
    {
        char **colored_line;
        int z_val = atoi(s_line[index]);

        if (color_check(s_line[index]))
        {
            colored_line = ft_split(s_line[index], ',');
            z_val = atoi(colored_line[0]);
            newmap->points[y * newmap->width + index].color = my_strtol(colored_line[1]);
            free_spl(colored_line);
        }

        point_index = y * newmap->width + index;
        newmap->points[point_index] = (t_point){index, y, z_val, newmap->color};

        if (z_val < newmap->z_min) newmap->z_min = z_val;
        if (z_val > newmap->z_max) newmap->z_max = z_val;

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
        s_line = ft_split(line, ' ');
        free(line);
        if (!s_line) return (close(fd), 0);

        process_line(newmap, s_line, y);
        free_spl(s_line);
        y++;
    }
    close(fd);
    return (1);
}


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
	int prev_x = p->x;
	int prev_y = p->y;
	float z_factor = calculate_z_factor(map);

	p->x = (prev_x - prev_y) * 0.866 * map->scale + map->offset_x;  // cos(30°) = 0.866
	p->y = (prev_x + prev_y) * 0.5 * map->scale - (p->z * map->scale * z_factor) + map->offset_y;  // sin(30°) = 0.5
}


void calculate_scale_and_offset(t_map *map, int win_width, int win_height)
{
	float scale_x = win_width / (map->width * 2);
	float scale_y = win_height / (map->height * 2);
	map->scale = (scale_x < scale_y) ? scale_x : scale_y;  // Берём минимальный масштаб

	map->offset_x = win_width / 2;
	map->offset_y = win_height / 3;  // Поднимаем карту чуть выше центра
}



void convert_to_isometric(t_map *map)
{
	int i = 0;
	while (i < map->width * map->height)
	{
		iso_projection(&map->points[i], map);
		i++;
	}
}

// void fit_map_in_window(t_map *map, int win_width, int win_height)
// {
// 	int i;
// 	int min_x = 999999, max_x = -999999;
// 	int min_y = 999999, max_y = -999999;

// 	// Смотрим координаты после iso_projection
// 	for (i = 0; i < map->width * map->height; i++)
// 	{
// 		if (map->points[i].x < min_x) min_x = map->points[i].x;
// 		if (map->points[i].x > max_x) max_x = map->points[i].x;
// 		if (map->points[i].y < min_y) min_y = map->points[i].y;
// 		if (map->points[i].y > max_y) max_y = map->points[i].y;
// 	}

// 	// Проверяем, умещается ли карта
// 	int map_width  = max_x - min_x;
// 	int map_height = max_y - min_y;

// 	// Пример: уменьшаем scale, если карта не влезает
// 	while (map_width > win_width || map_height > win_height)
// 	{
// 		map->scale *= 0.9; // уменьшили масштаб
// 		// Обновляем координаты заново
// 		convert_to_isometric(map);
// 		// Снова пересчитываем min_x...max_y, map_width, map_height
// 		// (Или можно сделать точную формулу, но итеративный подход проще)
// 	}

// 	// Сдвигаем offset, чтобы карта была по центру
// 	// Центр карты = (min_x + max_x) / 2, (min_y + max_y) / 2
// 	// Центр окна = (win_width / 2), (win_height / 2)
// 	int center_map_x = (min_x + max_x) / 2;
// 	int center_map_y = (min_y + max_y) / 2;
// 	int center_win_x = win_width / 2;
// 	int center_win_y = win_height / 2;

// 	int shift_x = center_win_x - center_map_x;
// 	int shift_y = center_win_y - center_map_y;

// 	for (i = 0; i < map->width * map->height; i++)
// 	{
// 		map->points[i].x += shift_x;
// 		map->points[i].y += shift_y;
// 	}
// }

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
        if (map->points[i].x < *min_x) *min_x = map->points[i].x;
        if (map->points[i].x > *max_x) *max_x = map->points[i].x;
        if (map->points[i].y < *min_y) *min_y = map->points[i].y;
        if (map->points[i].y > *max_y) *max_y = map->points[i].y;
    }
}

// Функция подгоняет карту под окно
void fit_map_in_window(t_map *map, int win_width, int win_height)
{
    int min_x, max_x, min_y, max_y;
    get_map_bounds(map, &min_x, &max_x, &min_y, &max_y);

    while ((max_x - min_x) > win_width || (max_y - min_y) > win_height)
    {
        map->scale *= 0.9; // Уменьшаем масштаб
        convert_to_isometric(map); // Пересчитываем координаты
        get_map_bounds(map, &min_x, &max_x, &min_y, &max_y);
    }

    int shift_x = (win_width / 2) - ((min_x + max_x) / 2);
    int shift_y = (win_height / 2) - ((min_y + max_y) / 2);

    for (int i = 0; i < map->width * map->height; i++)
    {
        map->points[i].x += shift_x;
        map->points[i].y += shift_y;
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
	int dx = abs(b.x - a.x);
	int dy = abs(b.y - a.y);
	int sx = (a.x < b.x) ? 1 : -1;
	int sy = (a.y < b.y) ? 1 : -1;
	int err = dx - dy;

	while (1)
	{
		put_pixel_to_image(img, a.x, a.y, color);
		if (a.x == b.x && a.y == b.y)
			break;
		int e2 = 2 * err;
		if (e2 > -dy) { err -= dy; a.x += sx; }
		if (e2 < dx) { err += dx; a.y += sy; }
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
int close_window(t_data *data)
{
    mlx_destroy_window(data->mlx, data->win);
    exit(0); // Завершаем программу
    return (0);
}


int key_hook(int keycode, t_data *data)
{
    if (keycode == 53 || keycode == 65307) // 53 - MacOS, 65307 - Linux
        close_window(data);
    return (0);
}

// void draw(t_map *map)
// {
// 	t_data	data;
// 	data.mlx = mlx_init();
// 	data.win = mlx_new_window(data.mlx, 1200,800, "FdF");

// 	// 1. Вычисляем масштаб и смещение (чтобы карта вписалась в окно)
// 	calculate_scale_and_offset(map, 1200,800);

// 	// 2. Преобразуем карту в изометрию (только после масштабирования!)
// 	convert_to_isometric(map);
// 	fit_map_in_window(map,1200,800);

// 	int i = 0;
// 	while (i < map->width * map->height)
// 	{
// 		printf("Point[%d]: (x=%d, y=%d, z=%d) AFTER ISO\n", i,
// 			map->points[i].x, map->points[i].y, map->points[i].z);
// 		i++;
// 	}
// 	printf("z_min = %d, z_max = %d\n", map->z_min, map->z_max);


// 	// 3. Рисуем карту
// 	draw_map(data.mlx, data.win, map);


// 	mlx_hook(data.win,17,0,close_window,&data);
// 	mlx_key_hook(data.win,key_hook,&data);

// 	// 4. Запускаем главный цикл MiniLibX
// 	mlx_loop(data.mlx);
// }

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

    calculate_scale_and_offset(map, 1200, 800);
    convert_to_isometric(map);
    fit_map_in_window(map, 1200, 800);

    for (int i = 0; i < map->width * map->height; i++)
        printf("Point[%d]: (x=%d, y=%d, z=%d) AFTER ISO\n",
               i, map->points[i].x, map->points[i].y, map->points[i].z);

    printf("z_min = %d, z_max = %d\n", map->z_min, map->z_max);
    draw_map(data.mlx, data.win, map);

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
	free_map(mapa);
	return (0);
}

//size_line = 8 //  2x2 == [][][][] [][][][] [][][][] [][][][]  size_line: 2 пикселя * 4 байта/пиксель = 8 байт.
// buffer [y * size_line + x * (bits_per_pixel / 8)]  y=0,x=1 // offset == 0 * 8(size_line) + 1 * 4 = 4 ---> offset/4 == 1;
												//    y=1,x=1 // offset == 1 * 8 + 1 * 4 = 12 ---> offset/4 == 3;