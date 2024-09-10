# software
PREFIX = ia16-elf

CC = $(PREFIX)-gcc
CCFLAGS = -ffreestanding

LD = $(PREFIX)-ld
LDFLAGS = -Ttext 0x0500 --oformat binary
LDLIBS = $(shell $(CC) -print-libgcc-file-name)

AS = nasm
QEMU = qemu-system-i386

# directories and files
SRC_DIR = src
BUILD_DIR = build

EXECUTABLE = gicho.img

OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(shell find $(SRC_DIR) -name *.c))
SAMPLES = $(patsubst $(SRC_DIR)/%.asm, $(BUILD_DIR)/%.bin, $(wildcard $(SRC_DIR)/samples/*.asm))

# phony
.PHONY: all run clean

# targets
all: $(BUILD_DIR)/$(EXECUTABLE)

run: $(BUILD_DIR)/$(EXECUTABLE)
	$(QEMU) -drive format=raw,file=$<

clean:
	$(RM) -r build

$(BUILD_DIR)/$(EXECUTABLE): $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin $(SAMPLES)
	cat $^ > $@

$(BUILD_DIR)/bootloader.bin: $(SRC_DIR)/bootloader/main.asm $(wildcard $(SRC_DIR)/bootloader/*.asm) $(BUILD_DIR)/kernel.bin | $(BUILD_DIR)
	$(AS) -f bin -DKERNEL_SIZE=$$(($(shell stat -c %s $(BUILD_DIR)/kernel.bin) / 512)) $< -o $@

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel/entry_point.o $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	truncate -s %512 $@

# rules
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CCFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(AS) -f elf $^ -o $@

$(BUILD_DIR)/%.bin: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(AS) -f bin $^ -o $@

$(BUILD_DIR):
	mkdir -p $@/kernel/lib $@/samples