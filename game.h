#pragma once

#include <stdbool.h>

#include <curses.h>

#include "rooms.h"

extern WINDOW *g_scr;
extern char *text_title[];
extern uint8_t text_title_lines;
extern char *text_intro1[];
extern uint8_t text_intro1_lines;
extern char *text_intro2[];
extern uint8_t text_intro2_lines;
extern char *text_intro3[];
extern uint8_t text_intro3_lines;
extern char *text_win[];
extern uint8_t text_win_lines;
extern char *text_lose[];
extern uint8_t text_lose_lines;

struct player {
	enum room_number current_room;
	uint8_t turns_left;
	bool medicine_found;
};

enum game_result {
	g_res_quit,
	g_res_continue,
	g_res_win,
	g_res_lose,
};

void g_main(void);
void g_display_text(char *text[], uint8_t textlines, char *lastline);

void g_draw_room_header(struct player *p);
enum game_result g_loop(void);
enum game_result g_room_search(struct player *p);
enum game_result g_room_move(struct player *p);

