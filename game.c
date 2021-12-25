#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>

#include "game.h"
#include "rooms.h"
#include "scr.h"

WINDOW *g_scr = NULL;

char *text_title[] = {
	"DARK SKY",
	"",
	"Original game by Ben \"Bent\" Ferguson",
	"https://barelyconsciousgames.itch.io/dark-sky",
	"",
	"curses port by Jamie Bainbridge",
	"https://github.com/superjamie/darksky"
};
uint8_t text_title_lines = sizeof(text_title) / sizeof(char*);

char *text_intro1[] = {
	"It is the year 2089.",
	"",
	"Mankind has developed a reliance on satellite technology.",
	"",
	"An unexpected pulse from an unidentified stellar phenomenon",
	"strikes the satellite network.",
	"",
	"A chain reaction causes one explosion, then another.",
	"The gigantic satellite network which serves as Earth's",
	"\"artificial ring\" becomes burning wreckage.",
	"",
	"What isn't destroyed by falling steel is destroyed by",
	"opportunistic bandits.",
};
uint8_t text_intro1_lines = sizeof(text_intro1) / sizeof(char*);

char *text_intro2[] = {
	"You are Kara.",
	"",
	"You are a twin sister. You and Sara have survived by relying",
	"on each other and salvaging everything you can.",
	"",
	"But she is diabetic, and you are out of insulin. She doesn't have long.",
	"",
	"Kara: \"Don't worry. I'll find your medicine and be back before you know it.\"",
	"",
	"You step out of the door to your makeshift home and lock it behind you.",
	"Better get a move on.",
};
uint8_t text_intro2_lines = sizeof(text_intro2) / sizeof(char*);

char *text_intro3[] = {
	"INSTRUCTIONS",
	"",
	"You have 15 turns to complete the game.",
	"",
	"Your objective is to find the medicine that your sister",
	"needs and return it to her before the turns reach zero.",
	"",
	"Sometimes, you can find food to increase the number of turns left.",
	"You can move between areas in the given direction, and search.",
	"Press Q at any time to quit.",
	"",
	"Good luck!",
};
uint8_t text_intro3_lines = sizeof(text_intro3) / sizeof(char*);

char *text_win[] = {
	"CONGRATULATIONS!",
	"",
	"You give your sister her much-needed medicine just in time.",
	"",
	"Hand-in-hand, you head off to find a safe new home.",
	"",
	"THE END",
};
uint8_t text_win_lines = sizeof(text_win) / sizeof(char*);

char *text_lose[] = {
	"You have starved to death...",
	"",
	"GAME OVER",
};
uint8_t text_lose_lines = sizeof(text_lose) / sizeof(char*);

void g_main(void)
{
	g_scr = newwin(SCREEN_Y, SCREEN_X, 0, 0);
	if (g_scr == NULL) {
		printf("g_scr failed\n");
		return;
	}

	g_display_text(text_title, text_title_lines, "- Press any key to start -");
	g_display_text(text_intro1, text_intro1_lines, "- Press any key to continue -");
	g_display_text(text_intro2, text_intro2_lines, "- Press any key to continue -");
	g_display_text(text_intro3, text_intro3_lines, "Press any key to begin your adventure!");

	enum game_result res = g_loop();

	switch (res) {
	case g_res_win:
		g_display_text(text_win, text_win_lines, "- Press any key -");
		break;
	case g_res_lose:
		g_display_text(text_lose, text_lose_lines, "- Press any key -");
		break;
	case g_res_quit:
	case g_res_continue:
		break;
	}

	delwin(g_scr);
}

void g_display_text(char *text[], uint8_t textlines, char *lastline)
{
	werase(g_scr);
	box(g_scr, 0, 0);

	int firstline = (SCREEN_Y - textlines - 1) / 2;

	for (uint8_t i = 0; i < textlines; i++) {
		uint8_t len = strlen(text[i]);
		uint8_t xpos = (SCREEN_X - len) / 2;
		mvwaddstr(g_scr, firstline + i, xpos, text[i]);
	}

	uint8_t lastlen = strlen(lastline);
	mvwaddstr(g_scr, (SCREEN_Y - 3), (SCREEN_X - lastlen) / 2, lastline);

	wrefresh(g_scr);
	wgetch(g_scr);
}

void g_draw_room_header(struct player *p)
{
	werase(g_scr);
	box(g_scr, 0, 0);

	wattron(g_scr, A_STANDOUT);
	mvwaddstr(g_scr, 2, 2, "                                                                            ");
	mvwprintw(g_scr, 2, 2, " %s", rooms[p->current_room].name);
	mvwprintw(g_scr, 2, 64, "%2d turns left", p->turns_left);
	wattroff(g_scr, A_STANDOUT);

	for (uint8_t line = 0; line < (ROOM_STR); line++) {
		mvwaddstr(g_scr, 4 + line, 2, rooms[p->current_room].desc[line]);
	}

	mvwaddstr(g_scr, 9, 2, "What will you do?");
	mvwaddstr(g_scr, 10, 4, "M)ove, or S)earch?");

	wrefresh(g_scr);
}

enum game_result g_room_move(struct player *p)
{
	mvwaddstr(g_scr, 13, 2, "MOVE");
	mvwaddstr(g_scr, 15, 2, "Exits:");
	int add_line = 0;
	if (rooms[p->current_room].exit_n) {
		mvwaddstr(g_scr, 16 + add_line, 3, "N)orth");
		add_line++;
	}
	if (rooms[p->current_room].exit_s) {
		mvwaddstr(g_scr, 16 + add_line, 3, "S)outh");
		add_line++;
	}
	if (rooms[p->current_room].exit_e) {
		mvwaddstr(g_scr, 16 + add_line, 3, "E)ast");
		add_line++;
	}
	if (rooms[p->current_room].exit_w) {
		mvwaddstr(g_scr, 16 + add_line, 3, "W)est");
		add_line++;
	}
	mvwaddstr(g_scr, 16 + add_line + 1, 2, "Which direction?");

	wrefresh(g_scr);

	bool moved = false;

	while (!moved) {
		int direction = wgetch(g_scr);

		switch (direction) {
		case 'n':
		case 'N':
			if (rooms[p->current_room].exit_n) {
				p->current_room = rooms[p->current_room].exit_n;
				moved = true;
			}
			break;
		case 's':
		case 'S':
			if (rooms[p->current_room].exit_s) {
				p->current_room = rooms[p->current_room].exit_s;
				moved = true;
			}
			break;
		case 'e':
		case 'E':
			if (rooms[p->current_room].exit_e) {
				p->current_room = rooms[p->current_room].exit_e;
				moved = true;
			}
			break;
		case 'w':
		case 'W':
			if (rooms[p->current_room].exit_w) {
				p->current_room = rooms[p->current_room].exit_w;
				moved = true;
			}
			break;
		case 'q':
		case 'Q':
			return g_res_quit;
			break;
		}
		if (moved) {
			p->turns_left--;
		}
		if (p->turns_left == 0)
			return g_res_lose;
	}
	return g_res_continue;
}

enum game_result g_room_search(struct player *p)
{
	enum game_result res = g_res_continue;

	mvwaddstr(g_scr, 13, 2, "SEARCH");
	
	int final_offset = 2;
	if (rooms[p->current_room].searched) {
		mvwaddstr(g_scr, 15, 2, "You've already thoroughly searched this area.");
	} else {
		for (int line = 0; line < ROOM_STR; line++) {
			mvwaddstr(g_scr, 15 + line, 2, rooms[p->current_room].search[line]);
			final_offset++;

		}
		if (rooms[p->current_room].gives_food == true) {
			mvwaddstr(g_scr, 15 + final_offset - 1, 2, "[You found food. Turns + 3]");
			p->turns_left += 3;
			final_offset++;
		}

		if (p->current_room == room_bandit_hq) {
			res = g_res_quit;
		}

		if (p->current_room == room_power_plant) {
			/* power is now off */
			p->current_room = room_power_plant_dark;
			rooms[room_main_drag_east].exit_e = room_power_plant_dark;
			rooms[room_main_drag_south].exit_s = room_bandit_hq_no_power;
		}

		if (p->current_room == room_cold_storage) {
			p->medicine_found = true;
		}

		rooms[p->current_room].searched = true;
	}
	mvwaddstr(g_scr, 15 + final_offset, 2, "(Press return)");
	wrefresh(g_scr);
	wgetch(g_scr);

	return res;
}

enum game_result g_loop(void)
{
	enum game_result res = g_res_continue;

	struct player p = {
		.current_room = room_office,
		.turns_left = 15,
		.medicine_found = false,
	};

	bool game_loop = true;

	while (game_loop) {
		if (p.medicine_found == true && p.current_room == room_office) {
			res = g_res_win;
			break;
		}

		g_draw_room_header(&p);
		int c = wgetch(g_scr);

		switch(c) {
		case 'm':
		case 'M':
			res = g_room_move(&p);
			break;
		case 's':
		case 'S':
			res = g_room_search(&p);
			break;
		case 'q':
		case 'Q':
			res = g_res_quit;
			break;
		}
		if (res != g_res_continue)
			game_loop = false;
	}
	return res;
}

