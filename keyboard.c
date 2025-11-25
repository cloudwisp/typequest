#include "keyboard.h"
#include <conio.h>

KeyEvent get_key_event() {
    KeyEvent event;
    int ch = getch();

    if (ch == 0 || ch == 224) {
        event.isAscii = 0;
        event.code = getch();  // extended key code
    } else {
        event.isAscii = 1;
        event.code = ch;       // regular ASCII code
    }

    return event;
}

