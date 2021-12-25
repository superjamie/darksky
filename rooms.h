#pragma once

#include <stdint.h>

#include "scr.h"

#define ROOM_STR  4

struct room {
	const char name[SCREEN_X];
	char desc[ROOM_STR][SCREEN_X];
	const char search[ROOM_STR][SCREEN_X];
	uint8_t exit_n;
	uint8_t exit_s;
	uint8_t exit_e;
	uint8_t exit_w;
	const bool gives_food;
	bool searched;
};

enum room_number {
	room_zero,
	room_office,
	room_office_alley,
	room_office_annex,
	room_plaza_side_road,
	room_chickn_shack,
	room_main_drag_north,
	room_pet_store,
	room_main_drag,
	room_garbage_alley,
	room_main_drag_east,
	room_power_plant,
	room_power_plant_dark,
	room_main_drag_south,
	room_dirt_road,
	room_old_house,
	room_bandit_hq,
	room_bandit_hq_no_power,
	room_cold_storage,
	ROOM_MAX
};

extern struct room rooms[ROOM_MAX];

