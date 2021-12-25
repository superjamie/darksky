#include <curses.h>

#include "main.h"

int main(void)
{
	curses_on();

	g_main();

	curses_off();

	printf("Thank you for playing Dark Sky!\n");
	/**
	 * if you got this reference, you are very cool
	 * https://www.reddit.com/r/shittyprogramming/comments/3bmszo/thank_you_for_playing_wing_commander/
	 */

	return 0;
}

void curses_on(void)
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
}

void curses_off(void)
{
	erase();
	refresh();
	endwin();
}

