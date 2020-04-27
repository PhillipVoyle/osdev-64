#include <machine.h>
#include <screen.h>

#define VIDEO_START 0xB8000

static unsigned char text_colour = VGA_COLOUR_WHITE;

static void EnableCursor(uint8_t cursor_start, uint8_t cursor_end)
{
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void UpdateCursor_Raw(uint16_t pos)
{
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

    unsigned char *video = ((unsigned char *)VIDEO_START);
    video[pos * 2 + 1] = text_colour;
}

static void UpdateCursor_XY(int x, int y)
{
    uint16_t pos = y * VGA_WIDTH + x;
    UpdateCursor_Raw(pos);
}

static uint16_t GetCursorPosition(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}

static void ClearScreen()
{
    int width = 80 * VGA_HEIGHT * 2;
    unsigned char *video = ((unsigned char *)VIDEO_START);
    for (int a = 0; a < width; a++)
    {
        *(video + a * 2) = '\0';
        *(video + a * 2 + 1) = text_colour;
    }
}

void VGA_Scroll()
{
    unsigned char *video = ((unsigned char *)VIDEO_START);
    for (int a = 0; a < VGA_WIDTH * (VGA_HEIGHT - 1) * 2; a++)
    {
        video[a] = video[VGA_WIDTH * 2 + a];
    }

    for (int a = 0; a < VGA_WIDTH; a++)
    {
        video[(VGA_WIDTH * (VGA_HEIGHT - 1) + a) * 2] = '\0';
        video[(VGA_WIDTH * (VGA_HEIGHT - 1) + a) * 2 + 1] = text_colour;
    }
}

void VGA_ClearScreen()
{
    ClearScreen();
    EnableCursor(8, 15);
    UpdateCursor_XY(0, 0);
}

void VGA_SetTextColour(unsigned char colour)
{
    unsigned char *video = ((unsigned char *)VIDEO_START);
    uint16_t pos = GetCursorPosition();
    text_colour = colour;
    video[pos * 2 + 1] = text_colour;
}
void VGA_SetTerminalPosition(int row, int col);

void VGA_WriteChar(char c, unsigned char colour)
{
    outb(0x3F8, c); //debug serial

    int cursorPos = GetCursorPosition();
    unsigned char *video = ((unsigned char *)VIDEO_START);

    if (c == '\n')
    {
        cursorPos = ((cursorPos / VGA_WIDTH) + 1) * VGA_WIDTH;
    }
    else
    {
        *(video + cursorPos * 2) = c;
        *(video + cursorPos * 2 + 1) = colour;
        cursorPos = cursorPos + 1;
    }

    if (cursorPos >= (VGA_WIDTH * VGA_HEIGHT))
    {
        VGA_Scroll();
        UpdateCursor_XY(0, 24);
    }
    else
    {
        UpdateCursor_Raw(cursorPos);
    }
}
void VGA_WriteString(const char *s)
{
    while (*s)
        VGA_WriteChar(*s++, text_colour);
}

static unsigned char *WriteIntPart(int remaining, unsigned char *video)
{
    int remainder = remaining % 10;
    int rest = remaining / 10;
    if (rest != 0)
    {
        video = WriteIntPart(rest, video);
    }
    VGA_WriteChar((unsigned char)remainder + '0', text_colour);
    return video;
}

void VGA_WriteInt(int n)
{
    if (n < 0)
    {
        n = -n;
        VGA_WriteChar('-', text_colour);
    }
    WriteIntPart(n, (unsigned char *)VIDEO_START);
}
