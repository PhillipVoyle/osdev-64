#ifndef __SCREEN_H
#define __SCREEN_H

#define VGA_COLOUR_BLACK            0x0
#define VGA_COLOUR_BLUE             0x1
#define VGA_COLOUR_GREEN            0x2
#define VGA_COLOUR_CYAN             0x3
#define VGA_COLOUR_RED              0x4
#define VGA_COLOUR_MAGENTA          0x5
#define VGA_COLOUR_BROWN            0x6
#define VGA_COLOUR_GRAY             0x7

#define VGA_COLOUR_DARK_GRAY        0x8
#define VGA_COLOUR_BRIGHT_BLUE      0x9
#define VGA_COLOUR_BRIGHT_GREEN     0xA
#define VGA_COLOUR_BRIGHT_CYAN      0xB
#define VGA_COLOUR_BRIGHT_RED       0xC
#define VGA_COLOUR_BRIGHT_MAGENTA   0xD
#define VGA_COLOUR_YELLOW           0xE
#define VGA_COLOUR_WHITE            0xF

#define VGA_COLOUR_MODIFIER_INTENSE 0x8

#define VGA_WIDTH   80
#define VGA_HEIGHT  25

void VGA_ClearScreen();
void VGA_WriteChar(char c, unsigned char colour);
void VGA_Scroll();

void VGA_SetTextColour(unsigned char colour);
void VGA_SetTerminalPosition(int row, int col);

void VGA_WriteString(const char* string);
void VGA_WriteInt(int i);

#endif//__SCREEN_H
