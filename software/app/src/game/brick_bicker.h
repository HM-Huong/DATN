#pragma once

const int BRICK_BRICKER_WIDTH = 480;
const int BRICK_BRICKER_HEIGHT = 480;

enum BrickBrickerCMD {
	NO_CMD,
	START,
	PLAY,
	QUIT,

	LEN
};

void control_brick_bricker(BrickBrickerCMD cmd);
