# constants
PREFIX = ia16-elf

CC = $(PREFIX)-gcc
CCFLAGS = -Wall -ffreestanding

LD = $(PREFIX)-ld
LDFLAGS = -Ttext 0x0500 --oformat binary
LDLIBS = $(shell $(CC) -print-libgcc-file-name)

AS = nasm
#ASFLAGS =

QEMU = qemu-system-i386

SRC_DIR = src
BUILD_DIR = build

EXECUTABLE = gicho.img

SOURCES = $(wildcard $(SRC_DIR)/kernel/*.c) $(wildcard $(SRC_DIR)/kernel/**/*.c)
OBJECTS = $(BUILD_DIR)/kernel/entry_point.o $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# phony
.PHONY: all run clean

# targets
all: $(BUILD_DIR)/$(EXECUTABLE)

run: $(BUILD_DIR)/$(EXECUTABLE)
	$(QEMU) -drive format=raw,file=$<

clean:
	$(RM) -r build

$(BUILD_DIR)/$(EXECUTABLE): $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

$(BUILD_DIR)/bootloader.bin: $(wildcard $(SRC_DIR)/bootloader/*.asm) | $(BUILD_DIR)/kernel.bin $(BUILD_DIR)
	$(AS) $(ASFLAGS) -DKERNEL_SIZE=$$(($(shell stat -c %s $(BUILD_DIR)/kernel.bin) / 512)) $< -o $@

$(BUILD_DIR)/kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	truncate -s %512 $@

# rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(AS) -f elf $(ASFLAGS) $^ -o $@

$(BUILD_DIR):
	mkdir -p $@/kernel/lib $@/kernel/shell