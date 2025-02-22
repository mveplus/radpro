# Bosean FS-600 128x64 LCD display (ST7567)

## Initialization sequence

* display_clear_en();
* sleep(20);
* display_set_en();
* sleep(20);
* display_send_command(0xe2); // Software reset
* display_send_command(0xa4); // Display all points: normal display
* display_send_command(0xe6); // Sets the LCD display normal/reverse: normal
* display_send_command(0xa2); // Set LCD bias: 1/9 bias
* display_send_command(0xf8); // Set internal booster...
* display_send_command(0x00); // ... to 4x
* display_send_command(0xa0); // Set SEG output direction: normal
* display_send_command(0xc8); // COM output scan direction: reverse direction
* display_send_command(0x2c); // Power control: 100, enable internal voltage booster
* display_send_command(0x2e); // Power control: 110, enable internal voltage regulator
* display_send_command(0x2f); // Power control: 111, enable internal voltage follower
* display_send_command(0x24); // Select internal resistor ratio(Rb/Ra) mode (0 to 7) [4]
* display_send_command(0x81); // Set contrast
* display_send_command(0x20); // Set contrast value: 00-3f contrast
* display_send_command(0x40); // Specify DDRAM line x for COM0 [0]
* display_send_command(0xae); // Turn LCD display: off
