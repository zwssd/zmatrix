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

/* Matrix typedef */
typedef struct zmatrix {
    int val;
    int bold;
} zmatrix;

zmatrix **matrix = (zmatrix **) NULL;   /* The matrix has you */

int *length = NULL;			/* Length of cols in each line */
int *spaces = NULL;			/* spaces left to fill */
int *updates = NULL;			/* What does this do again? :) */


void sig_winch(int s);
void *nmalloc(size_t howmuch);
void matrix_init(void);
int va_system(char *str, ...);
void finish(int sigage);
void c_die(char *msg, ...);
void usage(void);
void version(void);




int main(int argc, char **argv) {
    int update = 5, bold = -1, oldstyle = 0;
    int optchr, keypress;
    
    /* Many thanks to zwssd- (349705141@qq.com) for this getopt patch */
    opterr = 0;
    while ((optchr = getopt(argc, argv, "bBohV")) != EOF) {
	switch (optchr) {
	case 'b':
	    if (bold != 2 && bold != 0)
		bold = 1;
	    break;
	case 'B':
	    if (bold != 0)
		bold = 2;
	    break;
	case 'h':
	case '?':
	    usage();
	    exit(0);
	case 'o':
	    oldstyle = 1;
	    break;
	case 'V':
	    version();
	    exit(0);
	}
    }
    
    initscr();
    signal(SIGWINCH, sig_winch);
    
    /* Set up values for random number generation */
    if (console || xwindow) {
	randnum = 51;
	randmin = 166;
	highnum = 217;
    } else {
	randnum = 93;
	randmin = 33;
	highnum = 123;
    }
    
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

void sig_winch(int s)
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

    matrix_init();
    /* Do these b/c width may have changed... */
    clear();
    refresh();

}

/* nmalloc from nano by Big Gaute */
void *nmalloc(size_t howmuch)
{
    void *r;

    /* Panic save? */

    if (!(r = malloc(howmuch)))
	c_die("ZMatrix: malloc: out of memory!");

    return r;
}

/* Initialize the global variables */
void matrix_init(void)
{
    int i, j;

    if (matrix != NULL)
	free(matrix);

    matrix = (zmatrix **) nmalloc(sizeof(zmatrix) * (LINES + 1));
    for (i = 0; i <= LINES; i++)
	matrix[i] = (zmatrix *) nmalloc(sizeof(zmatrix) * COLS);

    if (length != NULL)
	free(length);
    length = (int *) nmalloc(COLS * sizeof(int));

    if (spaces != NULL)
	free(spaces);
    spaces = (int *) nmalloc(COLS* sizeof(int));

    if (updates != NULL)
	free(updates);
    updates = (int *) nmalloc(COLS * sizeof(int));

    /* Make the matrix */
    for (i = 0; i <= LINES; i++)
	for (j = 0; j <= COLS - 1; j += 2)
	    matrix[i][j].val = -1;

    for (j = 0; j <= COLS - 1; j += 2) {
	/* Set up spaces[] array of how many spaces to skip */
	spaces[j] = (int) rand() % LINES + 1;

	/* And length of the stream */
	length[j] = (int) rand() % (LINES - 3) + 3;

	/* Sentinel value for creation of new objects */
	matrix[1][j].val = ' ';

	/* And set updates[] array for update speed. */
	updates[j] = (int) rand() % 3 + 1;
    }
}

int va_system(char *str, ...)
{

    va_list ap;
    char foo[133];

    va_start(ap, str);
    vsnprintf(foo, 132, str, ap);
    va_end(ap);
    return system(foo);
}

/* What we do when we're all set to exit */
void finish(int sigage)
{
    curs_set(1);
    clear();
    refresh();
    resetty();
    endwin();
#ifdef HAVE_CONSOLECHARS
    if (console)
	va_system("consolechars -d");
#elif defined(HAVE_SETFONT)
    if (console)
	va_system("setfont");
#endif
    exit(0);
}

/* What we do when we're all set to exit */
void c_die(char *msg, ...)
{
    va_list ap;

    curs_set(1);
    clear();
    refresh();
    resetty();
    endwin();
#ifdef HAVE_CONSOLECHARS
    if (console)
	va_system("consolechars -d");
#elif defined(HAVE_SETFONT)
    if (console)
	va_system("setfont");
#endif

    va_start(ap, msg);
    vfprintf(stderr, "%s", ap);
    va_end(ap);
    exit(0);
}

void usage(void)
{
    printf(" Usage: zmatrix -[bBohV]\n");
    printf(" -b: Bold characters on\n");
    printf(" -B: All bold characters (overrides -b)\n");
    printf(" -o: Use old-style scrolling\n");
    printf(" -h: Print usage and exit\n");
    printf(" -V: Print version information and exit\n");
}

void version(void)
{
    printf(" ZMatrix version %s by zwssd (compiled %s, %s)\n",
	   "0.000001", __TIME__, __DATE__);
    printf(" Email: 349705141@qq.com  Web: https://github.com/zwssd/zmatrix\n");
}
