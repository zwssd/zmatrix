#include <iostream>

using namespace std;

extern "C" {
#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else				/* Uh oh */
#include <curses.h>
#endif				/* CURSES_H */
}

int main(int argc, char **argv) {
    initscr();
    int update = 5;
    
    std::cout << "Hello, world!" << std::endl;
    
    int a=0;
    
    while(1){
        a++;
        std::cout << a << std::endl;
        napms(update * 1);
        clear();
        refresh();
    }
    
    endwin();
    return 0;
}
