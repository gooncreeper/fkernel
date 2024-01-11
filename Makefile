CC := clang --target=i686-pc-none-elf -march=i686
ARCH_GENERIC := x86
ARCH_VERSION := IA-32

#UFLAGS = -O2 -flto
UFLAGS := -ggdb -Wall -Wextra -Wpedantic #-DDEBUG_OUTPUT
DEFINE := -DELF -DMULTIBOOT -DDEBUG_OUTPUT
IMAGE  := fkernel.bin

BUILD_ROOT := build
DEPFLAGS   := -MP -MD

KERNEL_ROOT    := kernel
KERNEL_SOURCE  := $(KERNEL_ROOT)
KERNEL_INCLUDE := $(KERNEL_ROOT)/include

         ALL_SUFFIX := all
ARCH_GENERIC_SUFFIX := arch/$(ARCH_GENERIC)/all
ARCH_VERSION_SUFFIX := arch/$(ARCH_GENERIC)/$(ARCH_VERSION)

 SUFFIX_DIRS := $(ALL_SUFFIX) $(ARCH_GENERIC_SUFFIX) $(ARCH_VERSION_SUFFIX)
 SOURCE_TOPS := $(foreach suffix, $(SUFFIX_DIRS),$(KERNEL_SOURCE)/$(suffix))
INCLUDE_DIRS := $(foreach suffix, $(SUFFIX_DIRS),$(KERNEL_INCLUDE)/$(suffix))

LINKER_SCRIPT := $(KERNEL_SOURCE)/$(ARCH_GENERIC_SUFFIX)/linker.ld
INCLUDE := $(foreach include_dir, $(INCLUDE_DIRS),-I$(include_dir))

CFLAGS := -std=gnu11 -fPIE \
          -ffreestanding -nostdlib -nostdinc -fno-builtin \
          -DARCH_FAMILY=$(ARCH_GENERIC) -DARCH_VERSION=$(ARCH_VERSION) \
          $(DEPFLAGS) $(UFLAGS) $(DEFINE) $(INCLUDE)

SOURCE_FILES := $(foreach source_top, $(SOURCE_TOPS), $(shell find $(source_top) -name "*.c" -o -name "*.S"))
OBJECT_FILES := $(foreach f, $(filter %.o, $(SOURCE_FILES:.c=.o) $(SOURCE_FILES:.S=.o)),$(BUILD_ROOT)/$(f))
DEPEND_FILES := $(foreach f, $(filter %.d, $(SOURCE_FILES:.c=.d) $(SOURCE_FILES:.S=.d)),$(BUILD_ROOT)/$(f))


all: $(IMAGE)

$(IMAGE): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -T $(LINKER_SCRIPT) -o $@ $^

$(BUILD_ROOT)/%.o:%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_ROOT)/%.o:%.S
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -R $(BUILD_ROOT)/*

-include $(DEPEND_FILES)

.PHONY: all clean
