#include "t_extension.h"
#include <wiringPi.h>
#include <stdio.h>

#define PHOTO_RESISTOR B22
#define TOUCH_BUTTON B17
#define AUTO_FLASH_1 B18
#define AUTO_FLASH_2 B27
#define RED_COLOR B23
#define GREEN_COLOR B24
#define LIGHT_SEQUENCE_LENGTH 4;

void resetPins() {
  digitalWrite(AUTO_FLASH_1, HIGH);
  digitalWrite(AUTO_FLASH_2, HIGH);
  digitalWrite(RED_COLOR, HIGH);
  digitalWrite(GREEN_COLOR, HIGH);
}

int main(void) {
    wiringPiSetup();
    pinMode(TOUCH_BUTTON, INPUT);
    pinMode(PHOTO_RESISTOR, INPUT);
    pullUpDnControl(TOUCH_BUTTON, PUD_UP);
    pinMode(AUTO_FLASH_1, OUTPUT);
    pinMode(AUTO_FLASH_2, OUTPUT);
    pinMode(RED_COLOR, OUTPUT);
    pinMode(GREEN_COLOR, OUTPUT);
    resetPins();

    int lights_sequence[LIGHT_SEQUENCE_LENGTH] = {AUTO_FLASH_1, AUTO_FLASH_2, GREEN_COLOR, RED_COLOR};
    int touch_status = 0;
    int touch_changed = 0;
    int sequence_position = 0;
    while(1) {
	if (!digitalRead(PHOTO_RESISTOR)) {
	    resetPins();
	}
	int tmp_status = digitalRead(TOUCH_BUTTON);
	touch_changed = (tmp_status != touch_status);
	touch_status = tmp_status;
        if (touch_changed) {
	  printf("Touched!\n");
	  digitalWrite(AUTO_FLASH_1, LOW);
	  digitalWrite(AUTO_FLASH_2, LOW);
        }
	delay(500);
    }
    return 0;
}
