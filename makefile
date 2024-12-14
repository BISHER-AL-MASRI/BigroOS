BOOT = boot
SRC = src
BIN = bin

C_FILES := $(wildcard $(SRC)/**/*.c $(SRC)/*.c) \
           $(SRC)/shell/bin/amogus/amogus.c \
           $(SRC)/shell/bin/bigroos/bigroos.c \
           $(SRC)/shell/bin/flappy/flappy.c \
           $(SRC)/shell/bin/snake/snake.c \
		   $(SRC)/shell/bin/ls/ls.c \
		   $(SRC)/shell/bin/cat/cat.c \
		   $(SRC)/utils/ext2/ext2.c \

S_FILES := $(wildcard $(SRC)/**/*.s $(SRC)/*.s)
OBJ_FILES := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(C_FILES)) $(patsubst $(SRC)/%.s, $(BIN)/%.o, $(S_FILES))

all: $(BIN)/myos.bin

$(BIN)/%.o: $(SRC)/%.s
	mkdir -p $(dir $@) # Ensure the output directory exists
	i686-elf-as $< -o $@

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@) # Ensure the output directory exists
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)



$(BIN)/myos.bin: $(OBJ_FILES)
	i686-elf-gcc -T $(SRC)/boot/linker.ld -o $@ -ffreestanding -O2 -nostdlib $(OBJ_FILES) -lgcc

disk.img: $(BIN)/myos.bin
	dd if=/dev/zero of=disk.img bs=512 count=93750 # 45MB disk
	dd if=$(BIN)/myos.bin of=disk.img conv=notrunc

run: disk.img
	qemu-system-i386 -drive file=disk.img,format=raw -kernel $<

run64: disk.img
	qemu-system-x86_64 -drive file=disk.img,format=raw -kernel $<

clean:
	rm -f $(BIN)/*.o $(BIN)/myos.bin disk.img
	find $(BIN) -type d -empty -delete
