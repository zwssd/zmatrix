#include <iostream>

using namespace std;

extern "C" {
#include <pty.h>
#include <signal.h>
#include<stdlib.h>  
#include<unistd.h>
#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else				/* Uh oh */
#include <curses.h>
#endif				/* CURSES_H */
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif				/* HAVE_SYS_IOCTL_H */
}

void handle_sigwinch(int s);

int main(int argc, char **argv) {
    int update = 5;
    
    initscr();
    
    signal(SIGWINCH, handle_sigwinch);
    
    //std::cout << "Hello, world!" << std::endl;
    
    char a='a';
    
    /*while(1){
        clear();
        addch(a);
        addch(' ');
        addch(' ');
        if(a=='a'){
            a = 'z';
        }else{
            a = 'a';
        }
        addch(a);
        
        napms(update * 1);
        refresh();
    }*/
    
    //endwin();
    getchar();//等待按键
    return 0;
}

void handle_sigwinch(int s)
{
    char *tty = NULL;
    int fd = 0;
    int result = 0;
    struct winsize win;

    result = ioctl(fd, TIOCGWINSZ, &win);
    if (result == -1)
	return;
    
    COLS = win.ws_col;
    LINES = win.ws_row;
    
    printf("%d rows, %d columns\n", win.ws_row, win.ws_col);//输出终端大小  

    /* Do these b/c width may have changed... */
    //clear();
    //refresh();

}
