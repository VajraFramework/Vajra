#include "ExampleGame/GameConstants/GameConstants.h"
#include "Vajra/Utilities/Utilities.h"

std::map<std::string, GameConstantId_type> gameConstantNameToId;
//
std::map<GameConstantId_type, float>       gameConstants_float;
std::map<GameConstantId_type, int>         gameConstants_int;
std::map<GameConstantId_type, std::string> gameConstants_string;
std::map<GameConstantId_type, bool>        gameConstants_bool;

void InitGameConstants() {

	gameConstantNameToId["camSpeed"] = GAME_CONSTANT_camSpeed;
	gameConstantNameToId["door_close_time"] = GAME_CONSTANT_door_close_time;
	gameConstantNameToId["ghostName"] = GAME_CONSTANT_ghostName;
	gameConstantNameToId["someBoolVariable"] = GAME_CONSTANT_someBoolVariable;
	gameConstantNameToId["someIntVariable"] = GAME_CONSTANT_someIntVariable;
	gameConstantNameToId["allowed_finger_movement_in_press"] = GAME_CONSTANT_allowed_finger_movement_in_press;
	gameConstantNameToId["long_press_length_in_seconds"] = GAME_CONSTANT_long_press_length_in_seconds;
	gameConstantNameToId["assassin_attack_speed"] = GAME_CONSTANT_assassin_attack_speed;
	gameConstantNameToId["swipe_dist_in_pixels"] = GAME_CONSTANT_swipe_dist_in_pixels;
	gameConstantNameToId["swipe_duration_in_seconds"] = GAME_CONSTANT_swipe_duration_in_seconds;
	gameConstantNameToId["dash_distance_in_units"] = GAME_CONSTANT_dash_distance_in_units;
	gameConstantNameToId["jump_distance_in_units"] = GAME_CONSTANT_jump_distance_in_units;
	gameConstantNameToId["jump_elevation_multiplier"] = GAME_CONSTANT_jump_elevation_multiplier;
	gameConstantNameToId["jump_speed_in_units_per_second"] = GAME_CONSTANT_jump_speed_in_units_per_second;
	gameConstantNameToId["target_tween_time"] = GAME_CONSTANT_target_tween_time;
	gameConstantNameToId["target_indicator_offset"] = GAME_CONSTANT_target_indicator_offset;
	gameConstantNameToId["target_indicator_scale"] = GAME_CONSTANT_target_indicator_scale;
	gameConstantNameToId["target_indicator_scale_hover"] = GAME_CONSTANT_target_indicator_scale_hover;

	ReadInGameConstants();
}

float GetFloatGameConstant(GameConstantId_type gameConstantId) {
	ASSERT(gameConstants_float.find(gameConstantId) != gameConstants_float.end(), "Looking for game constant of id %d in float constants, are you sure the constant is declared and that the datatype by which you are trying to get it is correct?", gameConstantId);
	return gameConstants_float[gameConstantId];
}

int GetIntGameConstant(GameConstantId_type gameConstantId) {
	ASSERT(gameConstants_int.find(gameConstantId) != gameConstants_int.end(), "Looking for game constant of id %d in int constants, are you sure the constant is declared and that the datatype by which you are trying to get it is correct?", gameConstantId);
	return gameConstants_int[gameConstantId];
}

std::string GetStringGameConstant(GameConstantId_type gameConstantId) {
	ASSERT(gameConstants_string.find(gameConstantId) != gameConstants_string.end(), "Looking for game constant of id %d in string constants, are you sure the constant is declared and that the datatype by which you are trying to get it is correct?", gameConstantId);
	return gameConstants_string[gameConstantId];
}

bool GetBoolGameConstant(GameConstantId_type gameConstantId) {
	ASSERT(gameConstants_bool.find(gameConstantId) != gameConstants_bool.end(), "Looking for game constant of id %d in bool constants, are you sure the constant is declared and that the datatype by which you are trying to get it is correct?", gameConstantId);
	return gameConstants_bool[gameConstantId];
}

