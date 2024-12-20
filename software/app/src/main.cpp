// Peripheral
#include "drivers/platform/init.h"
#include "drivers/platform/gpio_cores.h"

// Graphics
#include "drivers/platform/bar_test_core.h"
#include "drivers/platform/frame_core.h"
#include "drivers/platform/text_core.h"

// LCD
#include "drivers/device/LCD_IC2.h"

// Game
#include "game/brick_bicker.h"

GpoCore led(get_slot_addr(BRIDGE_BASE, S2_GPO));
GpiCore input(get_slot_addr(BRIDGE_BASE, S2_GPI));
BarTestCore bar(get_sprite_addr(BRIDGE_BASE, V4_TEST_COLOR));
TextCore osd(get_sprite_addr(BRIDGE_BASE, V1_TEXT));
FrameCore frame(FRAME_BASE);

void update_lcd(const char *mode) {
	static const char *lastMode = 0;
	if (lastMode == mode) {
		return;
	}
	lastMode = mode;
	terminal.send("Che do [%s]\n", mode);
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_printf("Che Do: ");
	lcd_gotoxy(2, 1);
	lcd_printf(mode);
}

void terminal_echo() {
	if (terminal.rx_fifo_empty()) {
		return;
	}

	while (!terminal.rx_fifo_empty()) {
		char ch = terminal.rx_byte();
		terminal.send(ch);
	}
}

int main() {
	static int bypass = -1;

	lcd_init(0x27);
	bar.bypass(1);

	while (1) {
		bypass = -1;
		led.write(input.read() >> 4);
		terminal_echo();

		update_lcd("Choi Game");
		control_brick_bricker(BrickBrickerCMD::START);

		while (input.read(4)) { // show bar
			update_lcd("K.Tra Man Hinh");
			terminal_echo();
			led.write(input.read() >> 4);
			control_brick_bricker(BrickBrickerCMD::QUIT);

			if (bypass != input.read(5)) {
				sleep_ms(100);
				bypass = input.read(5);
				bar.bypass(bypass);
				if (bypass) {
					terminal.send("[KT] Khong hien thi dai mau\n");
				} else {
					terminal.send("[KT] Hien thi dai mau\n");
				}
			}
		}
	}
}
