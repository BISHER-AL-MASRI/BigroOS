#ifndef SNAKE_H
#define SNAKE_H
extern void init_snake();
extern void spawn_food();
extern void draw_game();
extern void check_collision();
extern void update_snake();
extern void handle_input();
extern void snake_game();
extern int game_over;
extern int last_update;
#endif