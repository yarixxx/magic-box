#include "t_extension.h"
#include <wiringPi.h>
#include <stdio.h>

#define DHTPIN B25
#define PHOTO_RESISTOR B22
#define TOUCH_BUTTON B17
#define AUTO_FLASH_1 B18
#define AUTO_FLASH_2 B27
#define RED_COLOR B23
#define GREEN_COLOR B24
#define LIGHT_SEQUENCE_LENGTH 4

void initializeDht() {
    pinMode(DHTPIN, OUTPUT);
    digitalWrite(DHTPIN, LOW);
    delay(18);
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40); 
    pinMode(DHTPIN, INPUT);
}

int read_dht11_dat() {
    initializeDht();
    int dht11_dat[5];
    dht11_dat[0] = dht11_dat[2] = dht11_dat[4] = 0;
    int laststate = HIGH;
	int counter = 0;
    int j = 0;
    int i;
	for (i=0; i< 85; i++) {
		counter = 0;
		while (digitalRead(DHTPIN) == laststate) {
			counter++;
			delayMicroseconds(1);
			if (counter == 255) break;
		}
		laststate = digitalRead(DHTPIN);

		if (counter == 255) break;

		if ((i >= 4) && (i%2 == 0)) {
			dht11_dat[j/8] <<= 1;
			if (counter > 16)
				dht11_dat[j/8] |= 1;
			j++;
		}
	}

    int sum = dht11_dat[0] + dht11_dat[2];
    printf("Hey: %d\n", dht11_dat[0]);
    return (sum == dht11_dat[4]) ? dht11_dat[0] : 1;
}

void greenOn() {
  digitalWrite(RED_COLOR, LOW);
  digitalWrite(GREEN_COLOR, HIGH);
}

void redOn() {
  digitalWrite(GREEN_COLOR, LOW);
  digitalWrite(RED_COLOR, HIGH);
}

void colorsOff() {
  digitalWrite(RED_COLOR, LOW);
  digitalWrite(GREEN_COLOR, LOW);
}

void autoFlashOn() {
  digitalWrite(AUTO_FLASH_1, LOW);
  digitalWrite(AUTO_FLASH_2, LOW);
}

void autoFlashOff() {
  digitalWrite(AUTO_FLASH_1, HIGH);
  digitalWrite(AUTO_FLASH_2, HIGH);
}

void resetPins() {
  autoFlashOff();
  colorsOff();
  delay(100);
}

void initialize() {
  wiringPiSetup();
  pinMode(TOUCH_BUTTON, INPUT);
  pinMode(PHOTO_RESISTOR, INPUT);
  pullUpDnControl(TOUCH_BUTTON, PUD_UP);
  pinMode(AUTO_FLASH_1, OUTPUT);
  pinMode(AUTO_FLASH_2, OUTPUT);
  pinMode(RED_COLOR, OUTPUT);
  pinMode(GREEN_COLOR, OUTPUT);
}

int main(void) {
    initialize();
    resetPins();

    int touch_status = 0;
    int touch_changed = 0;
    int sequence_status = AUTO_FLASH_1;
    while(1) {
	if (!digitalRead(PHOTO_RESISTOR)) {
	  resetPins();
	} else {
	  int humidity = read_dht11_dat();
	  printf("Humidity: %d\n", humidity);
	  int tmp_status = digitalRead(TOUCH_BUTTON);
	  touch_changed = (tmp_status != touch_status);
	  touch_status = tmp_status;
          if (touch_changed) {
	    printf("Touch!\n");
	    autoFlashOn();
	    redOn();
	    greenOn(); 
          }
	  if (humidity > 70) {
	    greenOn();
	  } else {
	    redOn();
	  }
	}
	delay(1000);
    }
    return 0;
}
