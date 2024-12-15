#include "vga.h"
#include "keyboard.h"
#include "utils/util.h"
#include "utils/timer.h"

#define SNAKE_MAX_LENGTH 100
#define GAME_SPEED 10
#define GAME_WIDTH VGA_WIDTH
#define GAME_HEIGHT VGA_HEIGHT

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point segments[SNAKE_MAX_LENGTH];
    int length;
    int direction_x;
    int direction_y;
} Snake;

typedef struct {
    Point position;
} Food;

Snake snake;
Food food;
bool game_over;
unsigned int last_update;

void init_snake() {
    snake.length = 3;
    snake.direction_x = 1;
    snake.direction_y = 0;
    
    // Initialize snake in the middle of the screen
    for (int i = 0; i < snake.length; i++) {
        snake.segments[i].x = GAME_WIDTH / 4 - i;
        snake.segments[i].y = GAME_HEIGHT / 2;
    }
}

void spawn_food() {
    // Simple random position for food
    food.position.x = (timer_get() % (GAME_WIDTH - 2)) + 1;
    food.position.y = (timer_get() % (GAME_HEIGHT - 2)) + 1;
}

void draw_game() {
    // Clear screen
    terminal_initialize();
    
    // Draw snake
    for (int i = 0; i < snake.length; i++) {
        terminal_putentryat('O', vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK), 
                          snake.segments[i].x, snake.segments[i].y);
    }
    
    // Draw food
    terminal_putentryat('*', vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK), 
                       food.position.x, food.position.y);
    
    // Draw border
    for (int x = 0; x < GAME_WIDTH; x++) {
        terminal_putentryat('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), x, 0);
        terminal_putentryat('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), x, GAME_HEIGHT-1);
    }
    for (int y = 0; y < GAME_HEIGHT; y++) {
        terminal_putentryat('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), 0, y);
        terminal_putentryat('#', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK), GAME_WIDTH-1, y);
    }
}

bool check_collision() {
    Point head = snake.segments[0];
    
    // Check wall collision
    if (head.x <= 0 || head.x >= GAME_WIDTH-1 || head.y <= 0 || head.y >= GAME_HEIGHT-1) {
        return true;
    }
    
    // Check self collision
    for (int i = 1; i < snake.length; i++) {
        if (head.x == snake.segments[i].x && head.y == snake.segments[i].y) {
            return true;
        }
    }
    
    return false;
}

void update_snake() {
    // Move body
    for (int i = snake.length - 1; i > 0; i--) {
        snake.segments[i] = snake.segments[i-1];
    }
    
    // Move head
    snake.segments[0].x += snake.direction_x;
    snake.segments[0].y += snake.direction_y;
    
    // Check food collision
    if (snake.segments[0].x == food.position.x && snake.segments[0].y == food.position.y) {
        if (snake.length < SNAKE_MAX_LENGTH) {
            snake.length++;
            snake.segments[snake.length-1] = snake.segments[snake.length-2];
        }
        spawn_food();
    }
    
    // Check if game over
    if (check_collision()) {
        game_over = true;
    }
}

void handle_input() {
    if (keyboard.chars['w'] && snake.direction_y != 1) {
        snake.direction_x = 0;
        snake.direction_y = -1;
    }
    if (keyboard.chars['s'] && snake.direction_y != -1) {
        snake.direction_x = 0;
        snake.direction_y = 1;
    }
    if (keyboard.chars['a'] && snake.direction_x != 1) {
        snake.direction_x = -1;
        snake.direction_y = 0;
    }
    if (keyboard.chars['d'] && snake.direction_x != -1) {
        snake.direction_x = 1;
        snake.direction_y = 0;
    }
}

void snake_game() {
    // Initialize game
    game_over = false;
    init_snake();
    spawn_food();
    last_update = timer_get();
    
    // Game loop
    while (!game_over) {
        unsigned int current_time = timer_get();
        
        // Update game state at fixed intervals
        if (current_time - last_update > TIMER_TPS / GAME_SPEED) {
            handle_input();
            update_snake();
            draw_game();
            last_update = current_time;
        }
    }
    
    // Game over screen
    terminal_initialize();
    terminal_writestring("\n\n     Game Over!\n");
    terminal_writestring("     Score: ");
    char score[10];
    itoa(snake.length - 3, score, 10);
    terminal_writestring(score);

}
