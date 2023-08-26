# constants
CC = ia16-elf-gcc
CCFLAGS = -c -ffreestanding

LD = ia16-elf-ld
LDFLAGS = -Ttext 0x1000

AS = nasm
ASFLAGS = -f bin

QEMU = qemu-system-i386
QEMUFLAGS = -accel kvm

SRC_DIR = src
BUILD_DIR = build

EXECUTABLE = gicho.bin

SOURCES = $(shell find $(SRC_DIR) -name '*.c')
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# phony
.PHONY: all run clean

# targets
all: $(BUILD_DIR) $(BUILD_DIR)/$(EXECUTABLE)

run: all
	$(QEMU) $(QEMUFLAGS) -drive format=raw,file=$<

clean:
	rm -rf build

# main rules (technically targets)
$(BUILD_DIR)/$(EXECUTABLE): $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

$(BUILD_DIR)/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/bootloader/main.asm
	$(AS) $(ASFLAGS) -o $@ $<

# general rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)

	$(CC) $(CCFLAGS) -o $@ $<