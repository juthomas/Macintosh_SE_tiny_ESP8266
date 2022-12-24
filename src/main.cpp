#include "./se_tiny.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)

/**
 * === HARDWARE ===
 * ESP8266 D1 Mini
 * 0.96" OLED I2C Screen
 * USB-C Female Breakout Connector JRC-B008
 * SMD Button 3X6X5mm 2Pin
 * 
 * ==== PINOUT ====
 * D1 => SCL
 * D2 => SDA
 * D5 => Button
 * 5V => Usb
 * 3V => Screen VCC
 * GND => Usb, Button, Screen GND
 * 
 * USB D- to Pin 7 of CH430G
 * USB D+ to Pin 6 of CH340G
 */

WiFiUDP ntpUDP;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

uint8_t currentPage = 0;

IRAM_ATTR void incrementEEPROMValue()
{
	// Incrémenter la valeur stockée dans l'EEPROM et la réécrire
	EEPROM.get(0, currentPage);
	currentPage = currentPage < MODES_ALL - 1 ? currentPage + 1 : 0;
	EEPROM.put(0, currentPage);
	EEPROM.commit();
	Serial.println("Interrupt.");
	Serial.println(currentPage);
}

void setup()
{
	Serial.begin(115200);
	EEPROM.begin(256);
	pinMode(D5, INPUT_PULLUP);
	EEPROM.get(0, currentPage);
	attachInterrupt(D5, incrementEEPROMValue, FALLING);
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
	{
		Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}
	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(WHITE); // Draw white text
	display.setCursor(0, 0);	 // Start at top-left corner
	display.cp437(true);

	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	display.display();
	// delay(1000); // Pause for 2 seconds

	// Clear the buffer
	display.clearDisplay();

	WiFi.begin(ssid, password);

	Serial.println("Connecting to WiFi.");
	int _try = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print("..");
		displayLoading();
		delay(1000 + 1000 * _try);
		_try++;
		if (_try >= NB_TRYWIFI)
		{
			display.clearDisplay();
			Serial.println("Impossible to connect WiFi network, go to deep sleep");
			display.println("Impossible to connect WiFi network, go to deep sleep");
			display.display();
			ESP.deepSleep(10e6);
			ESP.restart();
		}
	}
	while (_try < 5)
	{
		displayLoading();
		delay(600);
		_try++;
	}

	Serial.println("Connected to the WiFi network");

	Serial.println("NTP client starting...");
	displayLoading();
	NTP.begin("pool.ntp.org", 1, true);
	NTP.setInterval(1000);

	Serial.println("NTP client started!");
	display.display();

	client->setInsecure();
}
void loop()
{
	display.clearDisplay();
	displayBackground();
	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(BLACK); // Draw white text
	display.setCursor(1, 12);	 // Start at top-left corner
	display.cp437(true);

	switch (currentPage)
	{
	case DESKTOP:
		viewDesktopMode();
		break;
	case PONG:
		playPongMode();
		break;
	case MOUSE:
		moveMouseMode();
		break;

	case TIME:
		displayTimeMode();
		break;

	default:
		break;
	}

	print_time();
	display.display();
	delay(500);
}

void displayBackground()
{
	display.fillRect(0, 0, display.width(), 10, SSD1306_WHITE);
	display.fillRect(0, 11, display.width(), display.height(), SSD1306_WHITE);
	display.drawBitmap(1, 1, mac_logo3_bmp, LOGO3_WIDTH, LOGO3_HEIGHT, SSD1306_BLACK);
	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(BLACK); // Draw white text
	display.setCursor(13, 1);
	display.cp437(true);
	display.print("File Edit");
}

void displayLoading()
{
	display.clearDisplay();
	static int loadingStep = 0;

	display.drawBitmap(64 - 8, 20, mac_logo2_bmp, LOGO2_WIDTH, LOGO2_HEIGHT, SSD1306_WHITE);

	for (int i = 0; i < loadingStep; i++)
	{
		display.drawBitmap(64 - 28 + 12 * i, 50, mac_logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
	}
	display.display();
	loadingStep++;
	if (loadingStep > 5)
	{
		loadingStep = 0;
	}
}

void drawDestopIcons()
{
	display.drawBitmap(115, 15, floppy_logo_bmp, FLOPPY_LOGO_WIDTH, FLOPPY_LOGO_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(115, 25, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	display.drawBitmap(100, 15, folder_logo_bmp, FOLDER_LOGO_WIDTH, FOLDER_LOGO_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(100, 25, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	display.drawBitmap(100, 30, folder_logo_bmp, FOLDER_LOGO_WIDTH, FOLDER_LOGO_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(100, 40, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	display.drawBitmap(115, 30, folder_logo_bmp, FOLDER_LOGO_WIDTH, FOLDER_LOGO_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(115, 40, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);

	display.drawBitmap(115, 45, trash_logo_bmp, TRASH_LOGO_WIDTH, TRASH_LOGO_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(115, 55, fake_text_bmp, FAKE_TEXT_WIDTH, FAKE_TEXT_HEIGHT, SSD1306_BLACK);
}

void viewDesktopMode()
{
	drawDestopIcons();
	display.drawBitmap(30, 50, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);
}

void displayTimeMode()
{
	display.drawLine(0, 18, display.width(), 18, SSD1306_BLACK);
	display.drawRect(1, 12, 5, 5, SSD1306_BLACK);

	display.drawLine(8, 12, 40, 12, SSD1306_BLACK);
	display.drawLine(8, 14, 40, 14, SSD1306_BLACK);
	display.drawLine(8, 16, 40, 16, SSD1306_BLACK);

	display.drawBitmap(44, 12, date_title_bmp, DATE_TITLE_WIDTH, DATE_TITLE_HEIGHT, SSD1306_BLACK);

	display.drawLine(81, 12, 113, 12, SSD1306_BLACK);
	display.drawLine(81, 14, 113, 14, SSD1306_BLACK);
	display.drawLine(81, 16, 113, 16, SSD1306_BLACK);

	display.drawRect(116, 12, 5, 5, SSD1306_BLACK);
	display.drawRect(116, 12, 3, 3, SSD1306_BLACK);
	display.drawRect(122, 12, 5, 5, SSD1306_BLACK);
	display.drawLine(122, 14, 126, 14, SSD1306_BLACK);

	display.drawLine(120, 19, 120, display.height(), SSD1306_BLACK);
	display.fillRect(122, 20, 5, 17, SSD1306_BLACK);
	display.drawLine(120, 48, 128, 48, SSD1306_BLACK);
	display.drawLine(120, 56, 128, 56, SSD1306_BLACK);
	display.drawBitmap(121, 51, arrow_up_bmp, ARROW_UP_WIDTH, ARROW_UP_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(121, 58, arrow_down_bmp, ARROW_DOWN_WIDTH, ARROW_DOWN_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(100, 52, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);

	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(BLACK); // Draw white text
	display.cp437(true);
	struct tm test = {0};
	sscanf(NTP.getTimeDateString().c_str(), "%d:%d:%d %d/%d/%d",
		   &test.tm_hour, &test.tm_min, &test.tm_sec, &test.tm_mday, &test.tm_mon, &test.tm_year);
	test.tm_isdst = -1; // Assume local daylight setting per date/time
	test.tm_mon--;		// Months since January
	if (test.tm_year >= 0 && test.tm_year < 100)
	{
		test.tm_year += 2000;
	}
	test.tm_year -= 1900; // Years since 1900
	time_t currentTime = mktime(&test);
	display.setCursor(6, 28);
	display.printf("%s,", dayStr(dayOfWeek(currentTime)));
	display.setCursor(6, 39);
	display.printf("%d", day(currentTime));
	if (day(currentTime) == 1 || day(currentTime) == 21 || day(currentTime) == 31)
	{
		display.print("st");
	}
	else if (day(currentTime) == 2 || day(currentTime) == 22)
	{
		display.print("nd");
	}
	else if (day(currentTime) == 3 || day(currentTime) == 23)
	{
		display.print("rd");
	}
	else
	{
		display.print("th");
	}
	display.printf(" %s %04d", monthStr(month(currentTime)), year(currentTime));
}

void playPongMode()
{
	display.drawBitmap(60, 14, pong_middle_bmp, PONG_MIDDLE_WIDTH, PONG_MIDDLE_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(5, 40, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(116, 30, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
	display.drawBitmap(30, 30, pong_ball_bmp, PONG_BALL_WIDTH, PONG_BALL_HEIGHT, SSD1306_BLACK);

	static int y_actual = 30;
	int y_desired;
	int y_current;
	int x_current;
	int x_min = 8 + 2;
	int x_max = 128 - 8 - 2 - 3;
	static bool playerTurn = false;
	static int y_player_1 = 30 - 7;
	static int y_player_1_old = 30;
	static int y_player_2 = 30 - 7;
	static int y_player_2_old = 30;

	for (int positions = 0; positions < 20; positions++)
	{
		y_desired = random(17, 55);
		for (int percentage = 0; percentage < 100; percentage++)
		{
			if (currentPage != PONG)
			{
				return;
			}

			x_current = map(percentage, playerTurn == true ? 0 : 100, playerTurn == true ? 100 : 0, x_min, x_max);

			y_current = map(percentage, 0, 100, y_actual, y_desired);
			display.clearDisplay();
			displayBackground();
			print_time();

			if (!playerTurn)
			{
				y_player_1 = map(percentage, 0, 100, y_player_1_old, y_desired) - 7;
			}
			else
			{
				y_player_2 = map(percentage, 0, 100, y_player_2_old, y_desired) - 7;
			}
			// drawDestopIcons();
			// display.drawBitmap(x_current, y_current, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);
			display.drawBitmap(60, 14, pong_middle_bmp, PONG_MIDDLE_WIDTH, PONG_MIDDLE_HEIGHT, SSD1306_BLACK);
			display.drawBitmap(5, y_player_1, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
			display.drawBitmap(116, y_player_2, pong_player_bmp, PONG_PLAYER_WIDTH, PONG_PLAYER_HEIGHT, SSD1306_BLACK);
			display.drawBitmap(x_current, y_current, pong_ball_bmp, PONG_BALL_WIDTH, PONG_BALL_HEIGHT, SSD1306_BLACK);
			display.display();
		}

		if (!playerTurn)
		{
			y_player_1_old = y_desired;
		}
		else
		{
			y_player_2_old = y_desired;
		}
		y_actual = y_desired;
		playerTurn = playerTurn == true ? false : true;
	}
}

void moveMouseMode()
{
	static int x_actual = 30;
	static int y_actual = 30;
	int x_desired;
	int y_desired;
	int x_current;
	int y_current;

	for (int positions = 0; positions < 10; positions++)
	{
		x_desired = random(1, display.width() - 8);
		y_desired = random(1, display.height() - 8);
		for (int percentage = 0; percentage < 100; percentage++)
		{
			if (currentPage != MOUSE)
			{
				return;
			}
			x_current = map(percentage, 0, 100, x_actual, x_desired);
			y_current = map(percentage, 0, 100, y_actual, y_desired);

			display.clearDisplay();
			displayBackground();
			print_time();
			drawDestopIcons();
			display.drawBitmap(x_current, y_current, mouse_bmp, MOUSE_WIDTH, MOUSE_HEIGHT, SSD1306_BLACK);
			display.display();
		}
		x_actual = x_desired;
		y_actual = y_desired;
	}
}

void print_time()
{

	display.setTextSize(1);		 // Normal 1:1 pixel scale
	display.setTextColor(BLACK); // Draw white text
	display.setCursor(77, 1);	 // Start at top-left corner
	display.cp437(true);
	display.println(NTP.getTimeStr());
}