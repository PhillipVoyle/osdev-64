CC=gcc
SHARED_FLAGS = -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
               -Werror -I. -MMD -mno-red-zone -mcmodel=kernel -fno-pie
CFLAGS = $(SHARED_FLAGS)
ASFLAGS = $(SHARED_FLAGS) -Wa,--divide

OBJS := boot.o
OBJS += kernel.o
OBJS += machine.o
OBJS += screen.o
OBJS += idt_asm.o
OBJS += idt_setup.o
OBJS += keyboard.o
OBJS += timer.o

DFILES = $(patsubst %.o,%.d,$(OBJS))

all: kernel

kernel: $(OBJS) kernel.ld Makefile
	$(CC) $(CFLAGS) -no-pie -Wl,--build-id=none -T kernel.ld -o $@ $(OBJS)

clean:
	find -name "*~" -delete
	rm -rf $(OBJS) $(DFILES) kernel

$(OBJS): Makefile
-include $(DFILES)
