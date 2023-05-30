#include <iostream>
#include "display.h"

int main() {
    Display screen(540, 810, "Game");

    screen.render_loop();

    return 0;
}
