BOOT = boot
SRC = src
BIN = bin

C_FILES := $(wildcard $(SRC)/**/*.c $(SRC)/*.c) \
           $(SRC)/shell/bin/bigroos/bigroos.c \
           $(SRC)/shell/bin/snake/snake.c \
           $(SRC)/fs/ext2/ext2.c
           # Remove the duplicate $(SRC)/utils/disk.c line since it's already included by $(wildcard $(SRC)/**/*.c)

S_FILES := $(wildcard $(SRC)/**/*.s $(SRC)/*.s)

OBJ_FILES := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(C_FILES)) \
             $(patsubst $(SRC)/%.s, $(BIN)/%.o, $(S_FILES)) 

all: disk.img

$(BIN)/%.o: $(SRC)/%.s
	mkdir -p $(dir $@)
	i686-elf-as $< -o $@

$(BIN)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(SRC)

$(BIN)/myos.bin: $(OBJ_FILES)
	i686-elf-gcc -T $(SRC)/boot/linker.ld -o $@ -ffreestanding -O2 -nostdlib $(OBJ_FILES) -lgcc

disk.img: $(BIN)/myos.bin
	dd if=/dev/zero of=disk.img bs=1M count=64
	parted disk.img mklabel msdos
	parted disk.img mkpart primary ext2 1 100%
	dd if=$(BIN)/myos.bin of=disk.img conv=notrunc seek=1

run: disk.img
	qemu-system-i386 -hda disk.img -boot c

run64: disk.img
	qemu-system-x86_64 -hda disk.img -boot c

clean:
	rm -f $(BIN)/*.o $(BIN)/myos.bin disk.img
	find $(BIN) -type d -empty -delete
