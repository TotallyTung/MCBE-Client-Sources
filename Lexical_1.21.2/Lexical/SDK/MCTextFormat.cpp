#include "MCTextFormat.h"

char MCTF::BASE_CHARACTER[2];
char MCTF::BLACK[4];
char MCTF::DARK_BLUE[4];
char MCTF::DARK_GREEN[4];
char MCTF::DARK_AQUA[4];
char MCTF::DARK_RED[4];
char MCTF::DARK_PURPLE[4];
char MCTF::GOLD[4];
char MCTF::GRAY[4];
char MCTF::DARK_GRAY[4];
char MCTF::BLUE[4];
char MCTF::GREEN[4];
char MCTF::ORANGE[4];
char MCTF::AQUA[4];
char MCTF::RED[4];
char MCTF::LIGHT_PURPLE[4];
char MCTF::YELLOW[4];
char MCTF::WHITE[4];

char MCTF::OBFUSCATED[4];
char MCTF::BOLD[4];
char MCTF::STRIKETHROUGH[4];
char MCTF::UNDERLINE[4];
char MCTF::ITALIC[4];
char MCTF::RESET[4];

void MCTF::initColor(char* variable, char color) {
	variable[0] = BASE_CHARACTER[0];
	variable[1] = BASE_CHARACTER[1];
	variable[2] = color;
	variable[3] = 0x0;
}

void MCTF::initMCTF() {
	BASE_CHARACTER[0] = (char)(0xC2);
	BASE_CHARACTER[1] = (char)(0xA7);
	initColor(BLACK, '0');
	initColor(DARK_BLUE, '1');
	initColor(DARK_GREEN, '2');
	initColor(DARK_AQUA, '3');
	initColor(DARK_RED, '4');
	initColor(DARK_PURPLE, '5');
	initColor(GOLD, '6');
	initColor(GRAY, '7');
	initColor(DARK_GRAY, '8');
	initColor(BLUE, '9');
	initColor(GREEN, 'a');
	initColor(AQUA, 'b');
	initColor(RED, 'c');
	initColor(LIGHT_PURPLE, 'd');
	initColor(YELLOW, 'e');
	initColor(WHITE, 'f');
	initColor(ORANGE, 'g');
	initColor(OBFUSCATED, 'k');
	initColor(BOLD, 'l');
	initColor(STRIKETHROUGH, 'm');
	initColor(UNDERLINE, 'n');
	initColor(ITALIC, 'o');
	initColor(RESET, 'r');
}