# software
PREFIX = ia16-elf

CC = $(PREFIX)-gcc
CCFLAGS = -ffreestanding

LD = $(PREFIX)-ld
LDFLAGS = -Ttext 0x0500 --oformat binary
LDLIBS = $(shell $(CC) -print-libgcc-file-name)

AS = nasm
GDB = gdb
QEMU = qemu-system-i386

# directories and files
SRC_DIR = src
BUILD_DIR = build

EXECUTABLE = gicho.bin

SOURCES = $(shell find $(SRC_DIR) -name *.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# phony
.PHONY: all run clean

# targets
all: $(BUILD_DIR)/$(EXECUTABLE)

run: $(BUILD_DIR)/$(EXECUTABLE)
	$(QEMU) -drive format=raw,file=$<

debug: $(BUILD_DIR)/$(EXECUTABLE)
	$(QEMU) -drive format=raw,file=$< -s -S &
	$(GDB) -ex "target remote localhost:1234"

clean:
	rm -rf build

$(BUILD_DIR)/$(EXECUTABLE): $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/bootloader/main.asm $(wildcard $(SRC_DIR)/bootloader/*.asm) | $(BUILD_DIR)
	$(AS) -f bin $< -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel/entry_point.o $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(AS) -f elf $^ -o $@

$(BUILD_DIR):
	mkdir -p $@/kernel/lib