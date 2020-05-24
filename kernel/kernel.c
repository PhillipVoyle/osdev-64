#include "machine.h"
#include "screen.h"
#include "idt_setup.h"
#include "timer.h"
#include "keyboard.h"

void Kernel_Hang()
{
    for(;;) {
        //VGA_WriteString("IDLE\n");
        asm ("hlt");
    }
}

void On_Char(char c)
{
    VGA_WriteChar(c, VGA_COLOUR_WHITE);
}

void On_Direction(Direction d)
{
    if (d == left)
    {
        VGA_WriteString("left\n");
    }
    else if (d == right)
    {
        VGA_WriteString("right\n");
    }
    else if (d == up)
    {
        VGA_WriteString("up\n");
    }
    else if (d == down)
    {
        VGA_WriteString("down\n");
    }
}

void Kernel_Main(void)
{
    VGA_WriteString("Phillip Voyle OSDEV Project.\n");
    VGA_ClearScreen();
    idt_init();

    Init_Timer();

    asm("sti");

    VGA_WriteString("Enabled Interrupts\n");
    Kernel_Hang();
}

void AnnounceInterrupt(int n) {
    VGA_WriteString("INT ");
    VGA_WriteInt(n);
    VGA_WriteString("\n");
}

void AnnounceIrq(int n) {
    VGA_WriteString("IRQ ");
    VGA_WriteInt(n);
    VGA_WriteString("\n");
}

void Panic(const char* reason)
{
    VGA_WriteString(reason);
    asm ("hlt");
}

void irq0_handler(void)
{
    On_Timer_Tick();
    outb(0x20, 0x20); //EOI
}

void irq1_handler(void)
{
    On_KeyStroke();
    outb(0x20, 0x20); //EOI
}

void irq2_handler(void)
{
    AnnounceIrq(2);
    outb(0x20, 0x20); //EOI
}

void irq3_handler(void)
{
    AnnounceIrq(3);
    outb(0x20, 0x20); //EOI
}

void irq4_handler(void)
{
    AnnounceIrq(4);
    outb(0x20, 0x20); //EOI
}

void irq5_handler(void)
{
    AnnounceIrq(5);
    outb(0x20, 0x20); //EOI
}

void irq6_handler(void)
{
    AnnounceIrq(6);
    outb(0x20, 0x20); //EOI
}

void irq7_handler(void)
{
    AnnounceIrq(7);
    outb(0x20, 0x20); //EOI
}

void irq8_handler(void)
{
    AnnounceIrq(8);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq9_handler(void)
{
    AnnounceIrq(9);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq10_handler(void)
{
    AnnounceIrq(10);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq11_handler(void)
{
    AnnounceIrq(11);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq12_handler(void)
{
    AnnounceIrq(12);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq13_handler(void)
{
    AnnounceIrq(13);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq14_handler(void)
{
    AnnounceIrq(14);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}

void irq15_handler(void)
{
    AnnounceIrq(15);
    outb(0xA0, 0x20);
    outb(0x20, 0x20); //EOI
}


void int0_handler()  {
    Panic("Divide by zero");
}
void int1_handler()  {
    AnnounceInterrupt(1);
}
void int2_handler()  {
    Panic("Non Maskable Interrupt");
}
void int3_handler()  {
    Panic("Breakpoint");
}
void int4_handler()  {
    AnnounceInterrupt(4);
}
void int5_handler()  {
    AnnounceInterrupt(5);
}
void int6_handler()  {
    Panic("Invalid Opcode");
}
void int7_handler()  {
    AnnounceInterrupt(7);
}
void int8_handler()  {
    AnnounceInterrupt(8);
}
void int9_handler()  {
    AnnounceInterrupt(9);
}
void int10_handler() {
    AnnounceInterrupt(10);
}
void int11_handler() {
    Panic("Segment not present");
}
void int12_handler() {
    Panic("Stack segment fault");
}
void int13_handler() {
    Panic("General Protection Failure");
}
void int14_handler() {
    Panic("Page fault");
}
void int15_handler() {
    AnnounceInterrupt(15);
}
