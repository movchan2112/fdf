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
	int x_iso;
	int y_iso;
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
float calculate_z_factor(t_map *map);
void iso_projection(t_point *p, t_map *map);
void calculate_scale_and_offset(t_map *map, int win_width, int win_height);
void convert_to_isometric(t_map *map);
void get_map_bounds(t_map *map, int *min_x, int *max_x, int *min_y, int *max_y);
void fit_map_in_window(t_map *map, int win_width, int win_height);
void put_pixel_to_image(t_img *img, int x, int y, int color);
void draw_line(t_img *img, t_point a, t_point b, int color);
void draw_map(void *mlx, void *win, t_map *map);
void	free_spl(char **line);
int cleanup(int fd, char *line, int error_code);
void free_map(t_map *mapa);
int close_window(t_data *data);
int		color_check(char *s);
int	get_width(char *line);
int	ft_map_width(const char *dest);
int	ft_map_height(const char *dest);
t_map	*init_map(int width, int height);
int ft_isspace(char c);
int ft_hex_digit(char c);
int	my_strtol_process(const char *str);
int	my_strtol(const char *str);
void	set_point(t_map *map, int x, int y, int z_val, int color);
void	process_line(t_map *newmap, char **s_line, int y);
int add_points(t_map *newmap, const char *dest);
size_t	ft_wordlen(const char *str, char c);
size_t	ft_count_words(const char *s, char c);
void	ft_free_split(char **arr, size_t index);
char	*ft_allocate_word(const char *s, size_t len);
char	**ft_split_1(const char *s, char c);



#endif