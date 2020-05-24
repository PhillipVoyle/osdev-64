#ifndef __KEYBOARD_H
#define __KEYBOARD_H

void On_KeyStroke();

void On_Char(char c);

enum Direction {left, up, down, right};
typedef enum Direction Direction;

void On_Direction(Direction d);

#endif//__KEYBOARD_H
