#include "brick_bicker.h"
#include "../drivers/platform/bar_test_core.h"
#include "../drivers/platform/frame_core.h"
#include "../drivers/platform/gpio_cores.h"
#include "../drivers/platform/init.h"
#include "../drivers/platform/sprite_core.h"
#include "../drivers/platform/text_core.h"

#include <cmath>
#include <cstdlib>

extern FrameCore frame;
extern TextCore osd;
extern GpiCore input;

SpriteCore ball(get_sprite_addr(BRIDGE_BASE, V3_BALL), 0);
SpriteCore paddle(get_sprite_addr(BRIDGE_BASE, V2_PADDLE), 0);

enum GameState {
	NOT_PLAYING,
	PLAYING,
	GAME_OVER,
};

GameState gameState = NOT_PLAYING;
int lives, score, scoreFactor = 1;

const int BALL_SIZE = 16;
float ballX, ballY, ballDX, ballDY;
unsigned long ballRefreshUs = 4000;
bool movingBall;

const int PADDLE_WIDTH = 128;
const int PADDLE_HEIGHT = 16;
int paddleX, paddleY;
unsigned long paddleRefreshUs = 2000;

const int TOP_GAP = 48;
const int BRICK_GAP = 22;
const int BRICK_WIDTH = 16;
const int BRICK_HEIGHT = 16;
const int BRICK_COL = (BRICK_BRICKER_WIDTH - BRICK_GAP) / (BRICK_WIDTH + BRICK_GAP);
const int BRICK_ROW = (BRICK_BRICKER_HEIGHT / 1.5 - BRICK_GAP) / (BRICK_HEIGHT + BRICK_GAP);
bool brick[BRICK_ROW][BRICK_COL];
int brickNum;

int topScore[10] = {
	-1,
	1400,
	800,
	770,
	750,
	700,
	650,
	550,
	500,
	450,
};

static void draw_bricks() {
	for (int i = 0; i < BRICK_ROW; i++) {
		for (int j = 0; j < BRICK_COL; j++) {
			const int x = j * (BRICK_WIDTH + BRICK_GAP) + BRICK_GAP;
			const int y = i * (BRICK_HEIGHT + BRICK_GAP) + TOP_GAP + BRICK_GAP;
			frame.fill_rect(x, y, BRICK_WIDTH, BRICK_HEIGHT, brick[i][j]);
			if (brick[i][j]) {
				++brickNum;
			}
		}
	}
}

static void init_paddle() {
	paddleX = (BRICK_BRICKER_WIDTH - PADDLE_WIDTH) / 2;
	paddleY = BRICK_BRICKER_HEIGHT - PADDLE_HEIGHT * 2;
	paddle.move_xy(paddleX, paddleY);
}

static void init_ball() {
	movingBall = false;
	// ball on top of paddle
	ballX = (paddleX + PADDLE_WIDTH / 2) - BALL_SIZE / 2;
	ballY = paddleY - BALL_SIZE + 2;
	// random direction from 30-70 and 110-150 degrees
	const int angle = rand() % 50 + (rand() % 2 ? 30 : 110);
	ballDX = std::cos(angle * M_PI / 180.0);
	ballDY = std::sin(angle * M_PI / 180.0);
	ball.move_xy(ballX, ballY);
}

static void show_top_score(const int x = 64, const int y = 16) {
	osd.wr_str(x, y, 0, "Top Score");
	for (int i = 1; i <= 9; i++) {
		osd.wr_str(x, y + i + 1, 0, "#%d: %d    ", i, topScore[i]);
	}
}

// topSore[0] is the index of current score
static void update_top_score(int score) {
	if (score <= topScore[9]) {
		return;
	}

	// update the current score
	if (topScore[0] == -1) {
		topScore[9] = score;
	} else {
		topScore[topScore[0]] = score;
	}

	// sort & update the index of current score
	for (int i = 1; i <= 9; i++) {
		for (int j = i + 1; j <= 9; j++) {
			if (topScore[i] < topScore[j]) {
				int tmp = topScore[i];
				topScore[i] = topScore[j];
				topScore[j] = tmp;
			}
		}
		if (topScore[i] == score) {
			topScore[0] = i;
		}
	}

	show_top_score(64, 14);
}

static void init_brick_bricker() {
	lives = 3;
	score = 0;
	scoreFactor = 0;
	brickNum = 0;

	srand(now_us());
	frame.set_bg_color(0);					// black
	frame.set_fg_color(rand() % 0xfff + 1); // random color
	frame.clr_screen();						// dark
	frame.draw_rect(0, 0, 639, 479, 1);		// Screen border
	frame.draw_rect(0, 0, BRICK_BRICKER_WIDTH - 1, BRICK_BRICKER_HEIGHT - 1, 1);
	frame.draw_rect(0, 0, BRICK_BRICKER_WIDTH + 1, BRICK_BRICKER_HEIGHT + 1, 1);
	frame.plot_line(0, TOP_GAP, BRICK_BRICKER_WIDTH - 1, TOP_GAP, 1);
	osd.clr_screen();
	osd.wr_str(64, 1, 1, "BRICK BRICKER");

	osd.wr_str(66, 6, 0, "Move the");
	osd.wr_str(66, 7, 0, "paddle to");
	osd.wr_str(66, 8, 0, "start the");
	osd.wr_str(66, 9, 0, "game!!!");

	topScore[0] = -1;
	show_top_score(64, 14);

	osd.wr_str(64, 28, 0, "Author: ");
	osd.wr_str(64 + 8, 28, 0, "HMHuong");

	init_paddle();
	init_ball();

	for (int i = 0; i < BRICK_ROW; i++) {
		for (int j = 0; j < BRICK_COL; j++) {
			brick[i][j] = true;
		}
	}
	draw_bricks();
}

static void update_paddle() {
	uint8_t btn10 = input.read() & 0x3;
	if (btn10 == 2 && paddleX > 0) {
		paddleX -= 1;
		movingBall = true;
	}
	if (btn10 == 1 && paddleX + PADDLE_WIDTH < BRICK_BRICKER_WIDTH) {
		paddleX += 1;
		movingBall = true;
	}
	paddle.move_xy(paddleX, paddleY);
}

static void game_over() {
	gameState = GAME_OVER;
	score += lives * 100;
	osd.wr_str(1, 1, 0, "Game Over, Score: %d. Press BTN3 to restart!      ", score);
	update_top_score(score);
}

static void update_ball() {
	if (!movingBall) {
		return;
	}

	// Paddle collision detection and handling
	if (ballY + BALL_SIZE >= paddleY &&
		ballX + BALL_SIZE >= paddleX &&
		ballX <= paddleX + PADDLE_WIDTH) {

		// Reverse vertical velocity
		ballDY = -ballDY;

		// Reset score multiplier
		scoreFactor = 0;

		// Move ball above paddle to prevent sticking
		ballY = paddleY - BALL_SIZE - 1;
	}

	// If ball goes below the paddle, decrease lives
	if (ballY >= paddleY + PADDLE_HEIGHT) {
		lives -= 1;
		if (lives == 0) {
			game_over();
			return;
		}
		init_ball();
	}

	// Check for collision with bricks
	for (int i = 0; i < BRICK_ROW; i++) {
		for (int j = 0; j < BRICK_COL; j++) {
			if (brick[i][j]) {
				const int x = j * (BRICK_WIDTH + BRICK_GAP) + BRICK_GAP;
				const int y = i * (BRICK_HEIGHT + BRICK_GAP) + TOP_GAP + BRICK_GAP;
				if (ballY <= y + BRICK_HEIGHT && ballY + BALL_SIZE >= y && ballX + BALL_SIZE >= x && ballX <= x + BRICK_WIDTH) {

					// Calculate collision points
					const float ballCenterX = ballX + BALL_SIZE / 2;
					const float ballCenterY = ballY + BALL_SIZE / 2;
					const float brickCenterX = x + BRICK_WIDTH / 2;
					const float brickCenterY = y + BRICK_HEIGHT / 2;

					// Determine collision side by comparing centers
					if (abs(ballCenterY - brickCenterY) * BRICK_WIDTH >
						abs(ballCenterX - brickCenterX) * BRICK_HEIGHT) {
						// Top/bottom collision - reverse vertical velocity
						ballDY = -ballDY;
					} else {
						// Side collision - reverse horizontal velocity
						ballDX = -ballDX;
					}

					frame.fill_rect(x, y, BRICK_WIDTH, BRICK_HEIGHT, 0);

					++scoreFactor;
					score += scoreFactor * 10;
					update_top_score(score);

					brick[i][j] = false;
					--brickNum;
					if (brickNum <= 0) {
						game_over();
						return;
					}
				}
			}
		}
	}

	// Check for collision with screen edges
	if (ballX <= 0 || ballX + BALL_SIZE >= BRICK_BRICKER_WIDTH) {
		ballDX = -ballDX; // Reverse direction on x-axis
	}
	if (ballY <= TOP_GAP || ballY + BALL_SIZE >= BRICK_BRICKER_HEIGHT) {
		ballDY = -ballDY; // Reverse direction on y-axis
	}

	ballX += ballDX;
	ballY += ballDY;
	ball.move_xy(ballX, ballY);
}

static void update_info() {
	const char LIVES[4][8] = {
		"     ",
		"\x03    ",
		"\x03 \x03  ",
		"\x03 \x03 \x03"
	};
	//	osd.clr_screen();
	osd.wr_str(1, 1, 0,
			   "  %5s    Score: %d    %d hit%c    ",
			   LIVES[lives],
			   score,
			   scoreFactor,
			   scoreFactor == 1 ? ' ' : 's');
}

static void update() {
	static unsigned long lastUpdatePaddle = 0;
	static unsigned long lastUpdateBall = 0;
	static unsigned long lastUpdateInfo = 0;
	const unsigned long now = now_us();

	if (now - lastUpdateInfo >= 2000) {
		update_info();
	}

	if (now - lastUpdatePaddle >= paddleRefreshUs) {
		lastUpdatePaddle = now;
		update_paddle();
	}

	if (now - lastUpdateBall >= ballRefreshUs) {
		lastUpdateBall = now;
		update_ball();
	}
}

extern BarTestCore bar;
static void hide_brick_bricker(int hidden) {
	bar.bypass(1);

	ball.bypass(hidden);
	paddle.bypass(hidden);
	frame.bypass(hidden);
	osd.bypass(hidden);
}

void control_brick_bricker(BrickBrickerCMD cmd) {
	switch (cmd) {
	case NO_CMD:
		if (gameState == PLAYING) {
			update();
		}
		break;

	case START:
		if (gameState == NOT_PLAYING) {
			init_brick_bricker();
			hide_brick_bricker(0);
			gameState = PLAYING;
		} else if (gameState == PLAYING) {
			update();
		}
		break;

	case PLAY:
		hide_brick_bricker(0);
		gameState = PLAYING;
		update();
		break;

	case QUIT:
		if (gameState != NOT_PLAYING) {
			hide_brick_bricker(1);
		}
		gameState = NOT_PLAYING;
		break;

	default:
		break;
	}

	if (gameState == GAME_OVER && input.read(3)) {
		gameState = NOT_PLAYING;
	}
}
