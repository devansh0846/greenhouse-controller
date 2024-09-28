#include <curses.h>
//or #include <ncurses.h>
//compile by typing gcc testingcurses.c -lcurses 
//or by typing gcc testingcurses.c -lncurses 
int main()
{	
	initscr();			/* Start curses mode 		  */
	mvprintw(1,1,"Hello World !!!");/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */

	return 0;
}