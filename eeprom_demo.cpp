#include "eeprom_demo.h"
#include <EEPROM.h>

#define RESET_EEPROM_BUTTON 9
#define CHANGE_PASSWORD_BUTTON 10
#define CHANGE_BUTTON 11
#define CONFIRM_BUTTON 12

#define FAILURE_LED 4
#define SUCCESS_LED 3
#define RESET_PASSWORD_LED 2

#define PASSWORD_LED_1 8
#define PASSWORD_LED_2 7
#define PASSWORD_LED_3 6
#define PASSWORD_LED_4 5

#define EEPROM_CONFIG_START 0
#define EEPROM_PASSWORD_START (int)sizeof(ConfigurationMarker)
#define PASSWORD_LENGTH (int)sizeof(Password)

#define EEPROM_END_INDEX EEPROM_CONFIG_START + EEPROM_PASSWORD_START + PASSWORD_LENGTH

struct ConfigurationMarker {
public:
	char val1;
	char val2;

	ConfigurationMarker(): val1(0), val2(0) {};
	ConfigurationMarker(int v1, int v2): val1(v1), val2(v2) {}
};

struct Password {
public:
	char val1;
	char val2;
	char val3;
	char val4;

	Password(): val1(0), val2(0), val3(0), val4(0) {};
	Password(char v1, char v2, char v3, char v4): val1(v1), val2(v2), val3(v3), val4(v4) {}
};

enum State {
	Init,
	ResetPassword,
	EnterPassword,
	LoggedIn
};

State programState = Init;
int currentIndex = 0;
int info[PASSWORD_LENGTH];
static int ledInfo[PASSWORD_LENGTH] = {PASSWORD_LED_1, PASSWORD_LED_2, PASSWORD_LED_3, PASSWORD_LED_4};

void clearEEPROM() {
	for (uint16_t i = EEPROM_CONFIG_START; i < EEPROM_END_INDEX; i++) {
		EEPROM.write(i, 0);
	}
}

void storeConfigurationMarker() {
	ConfigurationMarker m = ConfigurationMarker(127, 127);
	EEPROM.put(EEPROM_CONFIG_START, m);
}

void storeNewPassword() {
	Password p = Password(info[0], info[1], info[2], info[3]);
	EEPROM.put(EEPROM_PASSWORD_START, p);
}

bool checkPassword() {
	Password p;
	EEPROM.get(EEPROM_PASSWORD_START, p);
	if (info[0] != p.val1 ||
		info[1] != p.val2 ||
		info[2] != p.val3 ||
		info[3] != p.val4) {
		return false;
	}

	return true;
}

void setupSystemIfNeeded() {
	// Check configuration
	ConfigurationMarker marker;
	EEPROM.get(EEPROM_CONFIG_START, marker);

	if ((marker.val1 != 127 || marker.val2 != 127)) {
		Serial.println("No config");
		// Not configured, clear memory
		clearEEPROM();

		Password p = Password(0, 0, 0, 0);
		EEPROM.put(EEPROM_PASSWORD_START, p);
		Serial.println("Stored default password");

		storeConfigurationMarker();
		Serial.println("Stored configuration marker");
	} else {
		Serial.println("Configuration marker exists");

		Password p;
		EEPROM.get(EEPROM_PASSWORD_START, p);
		Serial.print("Current password: ");
		Serial.print(p.val1, DEC);
		Serial.print(p.val2, DEC);
		Serial.print(p.val3, DEC);
		Serial.println(p.val4, DEC);
	}
}

void resetInfo() {
	for (int i = 0; i < PASSWORD_LENGTH; i++) {
		info[i] = 0;
	}
	Serial.println("Cleared password input");
}

void blinkLed(int pin) {
	delay(100);
	digitalWrite(pin, HIGH);
	delay(100);
	digitalWrite(pin, LOW);
}

void blinkAllPasswordLEDs() {
	for (int i = 0; i < PASSWORD_LENGTH; i++) {
		blinkLed(ledInfo[i]);
	}
}

void setup() {
	Serial.begin(9600);
	pinMode(RESET_EEPROM_BUTTON, INPUT);
	pinMode(CHANGE_PASSWORD_BUTTON, INPUT);
	pinMode(CHANGE_BUTTON, INPUT);
	pinMode(CONFIRM_BUTTON, INPUT);

	pinMode(FAILURE_LED, OUTPUT);
	pinMode(SUCCESS_LED, OUTPUT);
	pinMode(RESET_PASSWORD_LED, OUTPUT);

	pinMode(PASSWORD_LED_1, OUTPUT);
	pinMode(PASSWORD_LED_2, OUTPUT);
	pinMode(PASSWORD_LED_3, OUTPUT);
	pinMode(PASSWORD_LED_4, OUTPUT);

	Serial.print("Password length: ");
	Serial.println(PASSWORD_LENGTH);

	setupSystemIfNeeded();
	programState = EnterPassword;
	resetInfo();
	blinkAllPasswordLEDs();
}

void processButtonPress(int pin, void (*callback)(void)) {
	delay(50);
	if (digitalRead(pin) != HIGH) { return; }
	callback();
	while (digitalRead(pin) == HIGH);
}

void didTypeAllNumbers() {
	if (programState == EnterPassword) {
		if (checkPassword() == true) {
			Serial.println("Password correct. Access granted");
			programState = LoggedIn;
		} else {
			Serial.println("Password incorrect");
			blinkLed(FAILURE_LED);
			currentIndex = 0;
			resetInfo();
		}
	} else if (programState == ResetPassword) {
		Serial.println("New password stored");
		storeNewPassword();
		programState = LoggedIn;
	}

	if (programState == LoggedIn) {
		digitalWrite(SUCCESS_LED, HIGH);
		digitalWrite(RESET_PASSWORD_LED, LOW);
	}
}

void didTapResetEEPROMButton() {
	clearEEPROM();
	resetInfo();
	programState = EnterPassword;
	setupSystemIfNeeded();
	digitalWrite(SUCCESS_LED, LOW);
	digitalWrite(RESET_PASSWORD_LED, LOW);
}

void didTapChangeButton() {
	if (programState == LoggedIn) { return; }
	blinkLed(ledInfo[currentIndex]);
	info[currentIndex] = (info[currentIndex] + 1) % 10;
}

void didTapConfirmButton() {
	if (currentIndex != PASSWORD_LENGTH - 1) {
		currentIndex += 1;
	} else {
		Serial.println("All digits typed");
		didTypeAllNumbers();
		currentIndex = 0;
	}

	blinkLed(ledInfo[currentIndex]);
}

void didTapChangePasswordButton() {
	if (programState == LoggedIn) {
		Serial.println("Entered password reset");
		programState = ResetPassword;
		currentIndex = 0;
		resetInfo();
		digitalWrite(RESET_PASSWORD_LED, HIGH);
		digitalWrite(SUCCESS_LED, LOW);
	}
}

void loop() {
	processButtonPress(CHANGE_BUTTON, didTapChangeButton);
	processButtonPress(CONFIRM_BUTTON, didTapConfirmButton);
	processButtonPress(CHANGE_PASSWORD_BUTTON, didTapChangePasswordButton);
	processButtonPress(RESET_EEPROM_BUTTON, didTapResetEEPROMButton);
}
