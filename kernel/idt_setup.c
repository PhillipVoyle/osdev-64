#include "machine.h"
#include "idt_setup.h"


unsigned char IDT[16 * 256];

static inline void load_idt(void* base, uint16_t size)
{   // This function works in 32 and 64bit mode
    struct {
        uint16_t length;
        void*    base;
    } __attribute__((packed)) IDTR = { size, base };
 
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

void set_idt_row(int row, void* address)
{
	unsigned long long irq_addr = (unsigned long long)address; 
	unsigned char * descriptor = IDT + 16 * row;
	descriptor[0] = irq_addr & 0xff;
	descriptor[1] = (irq_addr >> 8) & 0xff;
	descriptor[2]= 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
	descriptor[3] = 0;
	descriptor[4] = 0;
	descriptor[5] = 0x8e; /* INTERRUPT_GATE */
	descriptor[6] = (irq_addr >> 16) & 0xff;
	descriptor[7] = (irq_addr >> 24) & 0xff;
	descriptor[8] = (irq_addr >> 32) & 0xff;;
	descriptor[9] = (irq_addr >> 40) & 0xff;
	descriptor[10] = (irq_addr >> 48) & 0xff;
	descriptor[11] = (irq_addr >> 56) & 0xff;
	descriptor[12] = 0;
	descriptor[13] = 0;
	descriptor[14] = 0;
	descriptor[15] = 0;
}

void idt_init(void) {
    extern int irq0();
    extern int irq1();
    extern int irq2();
    extern int irq3();
    extern int irq4();
    extern int irq5();
    extern int irq6();
    extern int irq7();
    extern int irq8();
    extern int irq9();
    extern int irq10();
    extern int irq11();
    extern int irq12();
    extern int irq13();
    extern int irq14();
    extern int irq15();

    extern int int0();
    extern int int1();
    extern int int2();
    extern int int3();
    extern int int4();
    extern int int5();
    extern int int6();
    extern int int7();
    extern int int8();
    extern int int9();
    extern int int10();
    extern int int11();
    extern int int12();
    extern int int13();
    extern int int14();
    extern int int15();	
 

    /* remapping the PIC */
	outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 40);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

	set_idt_row(0, int0);
 	set_idt_row(1, int1);
 	set_idt_row(2, int2);
 	set_idt_row(3, int3);
 	set_idt_row(4, int4);
 	set_idt_row(5, int5);
 	set_idt_row(6, int6);
 	set_idt_row(7, int7);
 	set_idt_row(8, int8);
 	set_idt_row(9, int9);
 	set_idt_row(10, int10);
 	set_idt_row(11, int11);
 	set_idt_row(12, int12);
 	set_idt_row(13, int13);
 	set_idt_row(14, int14);
 	set_idt_row(15, int15);

    set_idt_row(32, irq0);
 	set_idt_row(33, irq1);
 	set_idt_row(34, irq2);
 	set_idt_row(35, irq3);
 	set_idt_row(36, irq4);
 	set_idt_row(37, irq5);
 	set_idt_row(38, irq6);
 	set_idt_row(39, irq7);
 	set_idt_row(40, irq8);
 	set_idt_row(41, irq9);
 	set_idt_row(42, irq10);
 	set_idt_row(43, irq11);
 	set_idt_row(44, irq12);
 	set_idt_row(45, irq13);
 	set_idt_row(46, irq14);
 	set_idt_row(47, irq15);
  
	load_idt(IDT, 16 * 256);
}
