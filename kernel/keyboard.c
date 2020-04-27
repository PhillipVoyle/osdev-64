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
    '\0','\0','\0', '<',  '>', '?','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
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
    '\0','\0','\0', '.', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
};

const char numlock_chars[] = {
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
    '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',

    '\0','\0','\0','\0', '\0','\0','\0', '7',  '8', '9','\0', '4',  '5', '6','\0', '1',
     '2', '3', '0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0'
};

/*
    TODO: Currently Unused

const char* Get_KeyName(unsigned char scancode) {
    switch (scancode)
    {
        case 0x01: return "ESC";
        case 0x02: return "1!";
        case 0x03: return "2@";
        case 0x04: return "3#";
        case 0x05: return "4$";
        case 0x06: return "5%";
        case 0x07: return "6^";
        case 0x08: return "7&";
        case 0x09: return "8*";
        case 0x0A: return "9(";
        case 0x0B: return "0)";
        case 0x0C: return "-_";
        case 0x0D: return "+=";
        case 0x0E: return "BACKSPACE";
        case 0x0F: return "TAB";

        case 0x10: return "qQ";
        case 0x11: return "wW";
        case 0x12: return "eE";
        case 0x13: return "rR";
        case 0x14: return "tT";
        case 0x15: return "yY";
        case 0x16: return "uU";
        case 0x17: return "iI";
        case 0x18: return "oO";
        case 0x19: return "pP";
        case 0x1A: return "[{";
        case 0x1B: return "]}";
        case 0x1C: return "RETURN";
        case 0x1D: return "L-CTRL";
        case 0x1E: return "aA";
        case 0x1F: return "sS";

        case 0x20: return "dD";
        case 0x21: return "fF";
        case 0x22: return "gG";
        case 0x23: return "hH";
        case 0x24: return "jJ";
        case 0x25: return "kK";
        case 0x26: return "lL";
        case 0x27: return ";:";
        case 0x28: return "\'\"";
        case 0x29: return "`~";
        case 0x2A: return "L-SHIFT";
        case 0x2B: return "\\|";
        case 0x2C: return "zZ";
        case 0x2D: return "xX";
        case 0x2E: return "cC";
        case 0x2F: return "vV";

        case 0x30: return "bB";
        case 0x31: return "nN";
        case 0x32: return "mM";
        case 0x33: return ",<";
        case 0x34: return ".>";
        case 0x35: return "/?";
        case 0x36: return "R-SHIFT";
        case 0x37: return "*";
        case 0x38: return "ALT";
        case 0x39: return "SPACE";
        case 0x3A: return "CAPS";
        case 0x3B: return "F1";
        case 0x3C: return "F2";
        case 0x3D: return "F3";
        case 0x3E: return "F4";
        case 0x3F: return "F5";

        case 0x40: return "F6";
        case 0x41: return "F7";
        case 0x42: return "F8";
        case 0x43: return "F9";
        case 0x44: return "F10";
        case 0x45: return "NUMLOCK";
        case 0x46: return "SCROLLLOCK";
        case 0x47: return "7Home";
        case 0x48: return "8Up";
        case 0x49: return "9PgUp";
        case 0x4A: return "-";
        case 0x4B: return "4Left";
        case 0x4C: return "5Center";
        case 0x4D: return "6Right";
        case 0x4E: return "+";
        case 0x4F: return "1end";

        case 0x50: return "2Down";
        case 0x51: return "3PgDn";
        case 0x52: return "0Ins";
        case 0x53: return ".Del";
        case 0x56: return "F11";
        case 0x57: return "F12";

        default: return "unknown";
    }
}

const char* Get_ModAKeyName(unsigned char scancode)
{
    switch(scancode)
    {
        case 0x0B: return "WINDOWS";
        case 0x1D: return "R-CTRL";
        case 0x38: return "R-ALT";
        case 0x47: return "HOME";
        case 0x48: return "UP";
        case 0x49: return "PGUP";
        case 0x4B: return "LEFT";
        case 0x4C: return "CENTER";
        case 0x4D: return "RIGHT";
        case 0x4F: return "END";
        case 0x50: return "DOWN";
        case 0x51: return "PGDOWN";
        case 0x52: return "INS";
        case 0x53: return "DEL";

        default: return "UNKNOWN (MOD-A)";
    }
}

const char* Get_ModBKeyName(unsigned char scancode)
{
    switch(scancode)
    {
        case 0x45: return "PAUSEBREAK";
        default: return "UNKNOWN (MOD-B)";
    }
}

*/

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
                VGA_WriteChar(shift_chars[scancode], VGA_COLOUR_WHITE);
            }
            else if ((lshift || rshift || caps) && (scancode < sizeof(caps_chars)) && (caps_chars[scancode] != 0))
            {
                VGA_WriteChar(caps_chars[scancode], VGA_COLOUR_WHITE);
            }
            else if (numlock && (scancode < sizeof(numlock_chars)) && (numlock_chars[scancode] != 0))
            {
                VGA_WriteChar(numlock_chars[scancode], VGA_COLOUR_WHITE);
            }
            else if ((scancode < sizeof(plain_chars)) && (plain_chars[scancode] != 0))
            {
                VGA_WriteChar(plain_chars[scancode], VGA_COLOUR_WHITE);
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
/*
    TODO: Pause/Break handling here
    else if (modifier == 0xE1)
    {
        modifier = byte;
        return;
    }   
    else
    {
        //VGA_WriteString(Get_ModBKeyName(scancode));
    }
    VGA_WriteString(" (code=0x");
    WriteHex(byte);
    VGA_WriteString(", ");
    VGA_WriteInt((int)byte);
    VGA_WriteString(")\n");
*/
}
