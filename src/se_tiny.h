#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <NtpClientLib.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <EEPROM.h>
#define NB_TRYWIFI 20

const char *ssid = "Livebox-75C0";
const char *password = "ipW2j3EzJQg6LF9Er6";

enum e_modes {
	DESKTOP = 0,
	PONG,
	MOUSE,
	TIME,
	MODES_ALL
};

#define LOGO_HEIGHT 8
#define LOGO_WIDTH 8
static const unsigned char PROGMEM mac_logo_bmp[] =
	{
		B00000000,
		B01111110,
		B01111110,
		B01111110,
		B01111110,
		B01111110,
		B01111110,
		B00000000,
};

#define LOGO2_HEIGHT 16
#define LOGO2_WIDTH 16
static const unsigned char PROGMEM mac_logo2_bmp[] =
	{
		B00111111, B11111100,
		B00100000, B00000100,
		B00100010, B01000100,
		B00100000, B10000100,
		B00100000, B10000100,
		B00100000, B00000100,
		B00100011, B11000100,
		B00100000, B00000100,
		B00111111, B11111100,
		B00111111, B11111100,
		B00101111, B10000100,
		B00111111, B11111100,
		B00111111, B11111100,
		B00000000, B00000000,
		B00111111, B11111100,
		B00111111, B11111100};

#define LOGO3_HEIGHT 8
#define LOGO3_WIDTH 8
static const unsigned char PROGMEM mac_logo3_bmp[] =
	{
		B00000100,
		B00001000,
		B00111110,
		B01111100,
		B01111100,
		B01111110,
		B01111110,
		B00111100,
};

#define TRASH_LOGO_HEIGHT 8
#define TRASH_LOGO_WIDTH 8
static const unsigned char PROGMEM trash_logo_bmp[] =
	{
		B00011000,
		B01111110,
		B00000000,
		B01011010,
		B01011010,
		B01011010,
		B01011010,
		B01011010,
};

#define FOLDER_LOGO_HEIGHT 8
#define FOLDER_LOGO_WIDTH 8
static const unsigned char PROGMEM folder_logo_bmp[] =
	{
		B01100000,
		B11110000,
		B11111111,
		B10000001,
		B10000001,
		B10000001,
		B10000001,
		B11111111,
};

#define FLOPPY_LOGO_HEIGHT 8
#define FLOPPY_LOGO_WIDTH 8
static const unsigned char PROGMEM floppy_logo_bmp[] =
	{
		B11001010,
		B11000011,
		B11111111,
		B11111111,
		B10000001,
		B10000001,
		B10000001,
		B11111111,
};

#define FAKE_TEXT_HEIGHT 3
#define FAKE_TEXT_WIDTH 8
static const unsigned char PROGMEM fake_text_bmp[] =
	{
		B11010010,
		B11010011,
		B11011010,
};

#define MOUSE_HEIGHT 8
#define MOUSE_WIDTH 8
static const unsigned char PROGMEM mouse_bmp[] =
	{
		B10000000,
		B11000000,
		B11100000,
		B11110000,
		B11111000,
		B11111100,
		B11110000,
		B10010000,
};

#define PONG_MIDDLE_HEIGHT 48
#define PONG_MIDDLE_WIDTH 8
static const unsigned char PROGMEM pong_middle_bmp[] =
	{
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
		B00011000,
		B00011000,
		B00011000,
		B00000000,
};

#define PONG_PLAYER_HEIGHT 16
#define PONG_PLAYER_WIDTH 8
static const unsigned char PROGMEM pong_player_bmp[] =
	{
		B00000000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00111000,
		B00000000,
};

#define PONG_BALL_HEIGHT 8
#define PONG_BALL_WIDTH 8
static const unsigned char PROGMEM pong_ball_bmp[] =
	{
		B11100000,
		B11100000,
		B11100000,
		B00000000,
		B00000000,
		B00000000,
		B00000000,
		B00000000,

};

#define DATE_TITLE_HEIGHT 5
#define DATE_TITLE_WIDTH 40
static const unsigned char PROGMEM date_title_bmp[] =
	{
		B11100000,B00100000,B00000010,B00000001,B00000000,
		B10010000,B01110000,B00000111,B00000011,B10000000,
		B10010010,B00100001,B00000010,B00101001,B00000000,
		B10010101,B00100010,B10000010,B00010001,B00000000,
		B11110010,B10011001,B11010001,B10101000,B10000000
};


#define ARROW_UP_HEIGHT 4
#define ARROW_UP_WIDTH 8
static const unsigned char PROGMEM arrow_up_bmp[] =
	{
		B00010000,
		B00111000,
		B01111100,
		B00000000
};

#define ARROW_DOWN_HEIGHT 4
#define ARROW_DOWN_WIDTH 8
static const unsigned char PROGMEM arrow_down_bmp[] =
	{
		B00000000,
		B01111100,
		B00111000,
		B00010000,
};

void displayBackground();

void displayLoading();

void drawDestopIcons();

void viewDesktopMode();

void playPongMode();

void displayTimeMode();

void moveMouseMode();

void customPrintStringZone(String str, int x, int y, int lineSpacing);

void printHttpPageContent(String link);

String getHttpsPagePayload(String link);

void getHttpsPageStream(String link);

void print_time();