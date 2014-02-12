#include "ExampleGame/GameConstants/GameConstants.h"
#include "Vajra/Utilities/Utilities.h"

std::map<GameConstantId_type, float>       gameConstants_float;
std::map<GameConstantId_type, int>         gameConstants_int;
std::map<GameConstantId_type, std::string> gameConstants_string;
std::map<GameConstantId_type, bool>        gameConstants_bool;

void InitGameConstants() {

	gameConstants_float[GAME_CONSTANT_camSpeed] = 15.0f;
	gameConstants_float[GAME_CONSTANT_door_close_time] = 2.0f;
	gameConstants_string[GAME_CONSTANT_ghostName] = "annabelle";
	gameConstants_bool[GAME_CONSTANT_someBoolVariable] = true;
	gameConstants_int[GAME_CONSTANT_someIntVariable] = 88;
	gameConstants_float[GAME_CONSTANT_allowed_finger_movement_in_press] = 10.0f;
	gameConstants_float[GAME_CONSTANT_long_press_length_in_seconds] = 0.5f;
	gameConstants_float[GAME_CONSTANT_assassin_attack_speed] = 10.0f;
	gameConstants_float[GAME_CONSTANT_swipe_dist_in_pixels] = 50.0f;
	gameConstants_float[GAME_CONSTANT_swipe_duration_in_seconds] = 1.0f;
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

