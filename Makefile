LD =      cross/bin/i586-elf-ld
CC =      cross/bin/i586-elf-gcc
AS =      nasm
FLAT_FS = /usr/local/bin/flatfs

SRC_DIR =       $(PWD)/src
BUILD_DIR =     $(PWD)/build
IMAGE_DIR =     $(PWD)/image
RESOURCES_DIR = $(PWD)/resources

KERNEL_DIR = $(SRC_DIR)/kernel

NOP =        $(BUILD_DIR)/nop.img
NOP_KERNEL = $(IMAGE_DIR)/boot/nop.bin

TINYBOOT = $(RESOURCES_DIR)/tinyboot.bin
LINKER =   $(RESOURCES_DIR)/linker.ld

KERNEL_C_FILES =  $(shell find $(KERNEL_DIR) -name "*.c")
KERNEL_AS_FILES = $(shell find $(KERNEL_DIR) -name "*.asm")
KERNEL_O_FILES =  $(KERNEL_AS_FILES:%.asm=%.o) $(KERNEL_C_FILES:%.c=%.o)

QEMU =  qemu-system-i386 -serial stdio -m 16
BOCHS = bochs

LD_FLAGS = \
	-pie \
	-Tresources/linker.ld \
	-nostdlib

CC_FLAGS = \
	-nostdlib \
	-ffreestanding \
	-Wall \
	-Wextra \
	-Ofast \
	-I$(KERNEL_DIR)/include \
	-D__iamcu__ \
	-std=gnu99

AS_FLAGS = -felf

all: $(NOP) obj-clean

$(NOP): $(NOP_KERNEL)
	cd image && $(FLAT_FS) $(NOP) -f boot/config.txt boot/nop.bin && cd ..
	dd if=$(TINYBOOT) of=$(NOP) seek=0 skip=0 status=progress conv=notrunc

$(NOP_KERNEL): $(KERNEL_O_FILES)
	$(LD) $(LD_FLAGS) -o $(NOP_KERNEL) $(KERNEL_O_FILES)

obj-clean:
	rm -rf $(KERNEL_O_FILES)

clean:
	rm -rf $(NOP) $(KERNEL_O_FILES) $(NOP_KERNEL) $(OS)

%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(AS_FLAGS) $< -o $@

run:
	$(QEMU) -hda $(NOP)

run-bochs:
	rm -f $(NOP).lock
	$(BOCHS)
