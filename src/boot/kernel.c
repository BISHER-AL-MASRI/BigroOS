#include "vga.h"
#include "keyboard.h"
#include "utils/util.h"
#include "utils/idt.h"
#include "utils/isr.h"
#include "utils/irq.h"
#include "utils/timer.h"
#include "utils/util.h"
#include "shell/shell.h"
#include "fs/ext2/ext2.h"

#define FPS 30

void kernel_main(void) {
    idt_init();
    isr_init();
    irq_init();
    terminal_initialize();
    timer_init();
    keyboard_init();
    
    // Initialize filesystem
    if (ext2_init() != 0) {
        terminal_writestring("Failed to initialize filesystem\n");
    }
    
    shell_initialize();
    
    unsigned int last_frame = 0, last = 0;  // Removed unused last_key_press
    char LastKeys[128] = { 0 };

    while(true) {
        size_t x, y;  // Changed from unsigned int to size_t
        terminal_getcursorposition(&x, &y);
        const unsigned int now = (unsigned int) timer_get();

        if (now != last) {
            last = now;
        }

		if ((now - last_frame) > (TIMER_TPS / FPS)) {
            last_frame = now;

			// Check if the last key pressed was released
			bool Keys[128] = { 0 };
			
			for (int i = 0; i < 128; i++) {
				if (keyboard.chars[i]) {
					Keys[i] = true;
				}else if (keyboard.releasedchars[i]) {
					Keys[i] = false;
				}
			}

			for (int i = 0; i < 128; i++) {
				if (LastKeys[i] != Keys[i]) {
					if (Keys[i]) {
						if (i == 0) continue;

						if ((char) i == KEY_BACKSPACE) {
							user_input('\b');
						}else if ((char) i == KEY_ENTER) {
							user_input('\n');
						}else {
							bool Shift = keyboard.mods & KEY_MOD_SHIFT;
							char c = (char) i;

							if (Shift)
								c = shift_char(c);
							
							user_input(c);
						}
					}
				}

				LastKeys[i] = Keys[i];
			}
		}
		
	}
}