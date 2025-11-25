#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEY_ESC        27
#define KEY_ENTER      13
#define KEY_BACKSPACE   8
#define KEY_TAB         9
#define KEY_SPACE      32

#define KEY_UP         72
#define KEY_DOWN       80
#define KEY_LEFT       75
#define KEY_RIGHT      77
#define KEY_HOME       71
#define KEY_END        79
#define KEY_PGUP       73
#define KEY_PGDN       81
#define KEY_INSERT     82
#define KEY_DELETE     83
#define KEY_F1         59
#define KEY_F2         60
#define KEY_F3         61
#define KEY_F4         62
#define KEY_F5         63
#define KEY_F6         64
#define KEY_F7         65
#define KEY_F8         66
#define KEY_F9         67
#define KEY_F10        68
#define KEY_F11        133
#define KEY_F12        134

typedef struct {
    int code;
    int isAscii;
} KeyEvent;

KeyEvent get_key_event();


#endif