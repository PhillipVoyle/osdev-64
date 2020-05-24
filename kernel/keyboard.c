/*
    keyboard handler and routine.
    There's a lot of FIXME in here, almost all of this belongs in the userland side,
    potentially all of it

    Also, there's a conflation of various concepts here
     * receiving keyboard input
     * translation of keyboard scan codes, control characters to to ascii characters
     * streaming input
     * echoing input to the screen/and or output stream
    
    TODO: rework
*/

#include "keyboard.h"
#include "screen.h"
#include "machine.h"

const char shift_chars[] = {
    '\0','\0', '!', '@',  '#', '$', '%', '^',  '&', '*', '(', ')',  '_', '=','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0', '{', '}', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0', ':', '\"', '~','\0', '|', '\0','\0','\0','\0',
    '\0','\0','\0', '<',  '>', '?','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',

    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0', '|','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
};

const char caps_chars[] = {
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
     'Q', 'W', 'E', 'R',  'T', 'Y', 'U', 'I',  'O', 'P','\0','\0', '\0','\0', 'A', 'S',
     'D', 'F', 'G', 'H',  'J', 'K', 'L','\0', '\0','\0','\0','\0',  'Z', 'X', 'C', 'V',
     'B', 'N', 'M','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
};

const char plain_chars[] = {
    '\0','\e', '1', '2',  '3', '4', '5', '6',  '7', '8', '9', '0',  '-', '+','\b','\t',
     'q', 'w', 'e', 'r',  't', 'y', 'u', 'i',  'o', 'p', '[', ']', '\n','\0', 'a', 's',
     'd', 'f', 'g', 'h',  'j', 'k', 'l', ';', '\'', '`','\0','\\',  'z', 'x', 'c', 'v',
     'b', 'n', 'm', ',',  '.', '/','\0', '*', '\0', ' ','\0','\0', '\0','\0','\0','\0',

    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0', '-','\0', '\0','\0', '+','\0',
    '\0','\0','\0', '.', '\0','\0','\\','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
};

const char numlock_chars[] = {
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',

    '\0','\0','\0','\0', '\0','\0','\0', '7',  '8', '9','\0', '4',  '5', '6','\0', '1',
     '2', '3', '0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
};

void WriteHexDigit(unsigned char c)
{
    if (c < 10)
    {
        VGA_WriteChar(c + '0', VGA_COLOUR_WHITE);
    }
    else
    {
        VGA_WriteChar(c - 10 + 'A', VGA_COLOUR_WHITE);
    }
}

void WriteHex(unsigned char c)
{
    WriteHexDigit((c >> 4) & 0xf);
    WriteHexDigit(c & 0xf);
}

int modifier = 0;
int lshift = 0;
int rshift = 0;
int caps = 0;
int lctrl = 0;
int rctrl = 0;
int lalt = 0;
int ralt = 0;
int numlock = 1;

void On_KeyStroke()
{
    unsigned char byte = inb(0x60);
    //WriteHex(byte);

    if ((byte == 0xE0) || (byte == 0xE1))
    {
        modifier = byte;
        return;
    }

    unsigned scancode = 0x7f & byte;
    if (modifier == 0)
    {
        if ((byte == 0xE0) || (byte == 0xE1))
        {
            modifier = byte;
        }
        else if ((0x80 & byte) == 0)
        {
            if (scancode == 0x2A)
            {
                lshift = 1;
            }
            else if (scancode == 0x36)
            {
                rshift = 1;
            }
            else if (scancode == 0x3a)
            {
                caps = !caps;
            }
            else if (scancode == 0x1D)
            {
                lctrl = 1;
            }
            else if (scancode == 0x38)
            {
                lalt = 1;
            }
            else if (scancode == 0x45)
            {
                numlock = !numlock;
            }
            else if ((lshift || rshift) && (scancode < sizeof(shift_chars)) && (shift_chars[scancode] != 0))
            {
                On_Char(shift_chars[scancode]);
            }
            else if ((lshift || rshift || caps) && (scancode < sizeof(caps_chars)) && (caps_chars[scancode] != 0))
            {
                On_Char(caps_chars[scancode]);
            }
            else if (numlock && (scancode < sizeof(numlock_chars)) && (numlock_chars[scancode] != 0))
            {
                On_Char(numlock_chars[scancode]);
            }
            else if ((scancode < sizeof(plain_chars)) && (plain_chars[scancode] != 0))
            {
                On_Char(plain_chars[scancode]);
            }
        }
        else
        {
            if (scancode == 0x2A)
            {
                lshift = 0;
            }
            else if (scancode == 0x36)
            {
                rshift = 0;
            }
            else if (scancode == 0x1D)
            {
                lctrl = 0;
            }
            else if (scancode == 0x38)
            {
                lalt = 0;
            }
        }
        return;
    }
    else if (modifier == 0xE0)
    {
        modifier = 0;

        if ((0x80 & byte) == 0)
        {
            if (scancode == 0x1D)
            {
                rctrl = 1;
            }
            else if (scancode == 0x38)
            {
                ralt = 1;
            }
            else if (scancode == 0x4B)
            {
                On_Direction(left);
            }
            else if (scancode == 0x48)
            {
                On_Direction(up);
            }
            else if (scancode == 0x4D)
            {
                On_Direction(right);
            }
            else if (scancode == 0x50)
            {
                On_Direction(down);
            }
        }
        else if ((0x80 & byte) == 0)
        {
            if (scancode == 0x1D)
            {
                rctrl = 0;
            }
            else if (scancode == 0x38)
            {
                ralt = 0;
            }
        }
    }
}
