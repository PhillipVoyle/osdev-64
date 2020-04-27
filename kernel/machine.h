#ifndef __MACHINE_H
#define __MACHINE_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

void outb(uint16_t port, uint8_t v);
uint8_t inb(uint16_t port);

#endif//__MACHINE_H
