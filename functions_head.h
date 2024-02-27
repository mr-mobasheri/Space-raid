#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define Reset "\e[0m "
#define BRED "\e[1;31m"
#define WHTB "\e[47m"
#define REDB "\e[41m"
#define HIDE_cursor "\e[?25l"
#define enable_cursor "\e[?25h"

typedef struct st1{
    short health, x, y, score, fire;
    short bullet_worth;
} spacecraft_info;

typedef struct st2{
    short x, y;
    struct st2* next;
    struct st2* prev;
} bullet;

typedef struct st3{
    short x, y, side, bomb, x_bomb, y_bomb, health, damage;
    struct st3 * next;
    struct st3 * prev;
} bomber;

typedef struct st4{
    short x, y, health, damage;
    struct st4 * next;
    struct st4 * prev;
} min;

typedef struct st5{
    short x, y, health;
} fighter;

typedef struct st6{
    short x, y, damage;
    struct st6* next;
    struct st6* prev;
} fighter_bullet;

typedef struct st7{
    short x, y, health, color, damage;
} drone;

typedef struct st8{
    short x, y, health, load, health_load;
}  cargo_ship;

typedef struct st9{
    short bomb_speed, mine_speed, fighter_speed, drone_speed, cargo_ship_spawn, bomber_spawn, mine_spawn, fighter_bullet_damage,
    bomb_damage, mine_damage, drone_damage;
}  level_info;

void clear_spacecraft(spacecraft_info *);
void print_spacecraft(spacecraft_info *);
void start_game(int, int *);
void menu(int *);
void print_border();
void print_border_score(spacecraft_info *);
void print_bullets(bullet **);
void print_bomber(bomber **, spacecraft_info *);
void print_bomber_bomb(bomber **, spacecraft_info *);
void hit_bullet(spacecraft_info *, bomber **, bullet **, min **, fighter *, drone *, cargo_ship *);
long currentMillis();
void set_level(int, level_info *);
void print_min(min **, spacecraft_info *);
void print_fighter(fighter *, spacecraft_info *);
void print_fighter_bullet(fighter_bullet **, spacecraft_info *);
void print_drobe(drone *, spacecraft_info *);
void print_cargo_ship(cargo_ship *, spacecraft_info *);
void game_over(int *, int, int);
void print_logs();
void choose_level(int *level);
void print_bye();