#include "Arduino.h"
#include "IRremote.h"
#include "SPI.h"
#include "MD_MAX72xx.h"
#include "MD_Parola.h"
#include "RemoteDefinition.h"
#include "enhanced_led_board.h"
#include "TimerOne.h"
#include "TimedAction.h"
//#include "String.h"
#define IR_CARMP3 // Define our remote

#define LEFT_HIT_PIN 40 //Define pin to sense left opponent hit
#define LEFT_FALSE_HIT_PIN 42 //Define pin to sense left opponent  false hit
#define RIGHT_FALSE_HIT_PIN 44 //Define pin to sense right opponent  false hit
#define RIGHT_HIT_PIN 46 //Define pin to sense right opponent hit

#define LOCK_HIT_PIN 48 //Define output pin to lock another Arduino

#define	MAX_DEVICES	8 //Number of led matrixes
#define	MAX_ZONES	4
#define ZONE_SIZE (MAX_DEVICES/MAX_ZONES)   // integer multiple works best

#define	CLK_PIN		52  // or SCK for LED matrix - Mega 2560
#define	DATA_PIN	51  // or MOSI  LED matrix - Mega 2560
#define	CS_PIN		53  // or SS  LED matrix -  Mega 2560

#define IR_RCV_PIN 11 //Define input pin for IR receiver - Mega 2560
#define MAX_TIME 150 // max ms between codes for IR debouncing
long lastPressTime = 0; //for IR receiver debouncing

IRrecv irrecv(IR_RCV_PIN); //Define IR receiver
decode_results IRresult; //Variable for keeping results of IR receiver

// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);

#define	SPEED_TIME	0
#define	PAUSE_TIME	0

String minsTens, minsUnits, secsTens, secsUnits;
String scoreLeft, scoreRight;
int mins10, mins1, secs10, secs1;
int minutes, seconds;
int clockRunning = 0;
char secs[6];
char mins[6];
char scoreL[6];
char scoreR[6];
String strsec;
String strmin;
String strScoreLeft;
String strScoreRight;
char s1[3];
char s10[6];
int hitState;
//States of the machine
bool timerRunning = false;
bool justRearmed = true;
bool timeOut = false;
bool setupMachine = false;
bool maxScoreReached = false;

//Setup of timer default values
int startMinutes = 3;
int startSeconds = 0;
int startPeriods = 1;
int startMaxPoints = 5;
int startPoints = 0;
int leftPoints = startPoints;
int rightPoints = startPoints;

int period;

void clockUpdate();

TimedAction timer = TimedAction(1000, clockUpdate); //run clockUpdate every 1000 ticks
char maliBroj[10] = { 137, 128, 129, 130, 131, 132, 133, 134, 135, 136 }; //Small numbers
String clockSuffix = "";
String clockPrefix = "";

void setup() {

	//Define pins for sensing signals from another board
	pinMode(LEFT_HIT_PIN, INPUT);
	pinMode(LEFT_FALSE_HIT_PIN, INPUT);
	pinMode(RIGHT_HIT_PIN, INPUT);
	pinMode(RIGHT_FALSE_HIT_PIN, INPUT);
	pinMode(LOCK_HIT_PIN, OUTPUT);
	Serial.begin(9600);
	irrecv.enableIRIn(); // Start the IR receiver
	//Serial.begin(57600);
	minutes = startMinutes;
	seconds = startSeconds;
	period = startPeriods;
	Timer1.initialize(4000);
	//Timer1.attachInterrupt( clockUpdate);

	//Definition of display zones
	P.begin(MAX_ZONES);
	P.setFont(0, enhanced_led_board);
	P.setFont(1, enhanced_led_board);
	P.setFont(2, enhanced_led_board);
	P.setFont(3, enhanced_led_board);
		//Set display zones
	for (uint8_t i = 0; i < MAX_ZONES; i++) {
		P.setZone(i, ZONE_SIZE * i, (ZONE_SIZE * (i + 1)) - 1);

	}

	//P.setInvert(false);
	P.displayReset();

	//Small intro :-)
	P.displayZoneText(2, "\x08c", PA_RIGHT, 35, 0, PA_SCROLL_RIGHT, PA_NO_EFFECT);
	while (!P.getZoneStatus(2))
		P.displayAnimate();
	P.displayZoneText(1, "\x08d", PA_LEFT, 35, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
	while (!P.getZoneStatus(1))
		P.displayAnimate();
	delay(2000);
	P.displayZoneText(2, "MK", PA_RIGHT, 35, 0, PA_SCROLL_RIGHT, PA_NO_EFFECT);
	while (!P.getZoneStatus(2))
		P.displayAnimate();
	P.displayZoneText(1, "BL", PA_LEFT, 35, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
	while (!P.getZoneStatus(1))
		P.displayAnimate();
	blinkClock();
	delay(1500);
	P.displayClear();

	timer.disable();
	clockReset();

	//TODO: Add default max points and max periods

}

void loop() {
/*We are looking for signal state.
1 - Left opponent hit
2 - Left opponent faulty hit
4 - Right opponent hit
8 - Righ opponent faulty hit
5 - Double hit
*/
if (timerRunning == true ) {
hitState= digitalRead(LEFT_HIT_PIN)*1 + digitalRead(LEFT_FALSE_HIT_PIN) * 2 + digitalRead(RIGHT_HIT_PIN)*4 + digitalRead(RIGHT_FALSE_HIT_PIN)*8 ;
//Serial.println (hitState);
   if (hitState > 0) {

									timer.disable();
									timerRunning = false;

																switch (hitState) {
																case 1:

																								increaseLeftScore();
																								displayUpdate();
																								blinkZone(3);
																								break;

																case 2:
																								//timer.disable();
																								//timerRunning = false;
																								break;

																case 4:
																								increaseRightScore();
																								displayUpdate();
																								blinkZone(0);
																								break;

																case 5:

																								//increaseLeftScore();
																								//increaseRightScore();
																								increaseDouble();
																								displayUpdate();
																								blinkTwoZones(0,3);
																								break;

																case 8:
																								break;

																default:
																								break;



																}
																//delay(1000);
								}
								//delay(1200);
}


//clockUpdate();
	if (irrecv.decode(&IRresult)) {
		//Serial.println(IRresult.value, HEX);
		switch (IRresult.value) {

		//Start/Stop/Pause pressed
		case IR_START_STOP: //Start/Stop/Pause pressed
			if (timerRunning == false && setupMachine == false && maxScoreReached == false) {
				if (timeOut == true) {
					rearmMachine();
				}
				timeOut = false;
				timer.enable();
				timerRunning = true;
			} else if (timerRunning == true) {
				timer.disable();
				timerRunning = false;
			}
			break;
			//*************************************

			//Rearm pressed- Resets all values to setup values
		case IR_REARM: {
			if (timerRunning == true) {
				break;
			} else if (timerRunning == false) {
				rearmMachine();
			}
		}
			break;
			//*************************************************

			// Settings pressed
		case IR_SETTINGS: {
			if (timerRunning == false && justRearmed == true
					&& setupMachine == false) {
				setupMachine = true;
				clockSuffix = "=";
				displayUpdate();
			} else if (setupMachine == true) {
				setupMachine = false;
				clockSuffix = "";
				displayUpdate();

			}
			break;
		}

			//Minutes per period +
		case IR_MINUTES_PLUS: {
			if (setupMachine == true) {
				minutes++;
				startMinutes=minutes;
				clockReset();
			}
			break;
		}
			//***************************

			//Minutes per period -
		case IR_MINUTES_MINUS: {
			if (setupMachine == true) {
				if (minutes > 1) {
					minutes--;
					startMinutes=minutes;
					clockReset();
				}

			}
			break;
		}

		case IR_L_TOUCH_PLUS: {
			if (setupMachine == true){
					leftPoints++;
				startMaxPoints = leftPoints;
			}

			else if (timerRunning == false) {
				increaseLeftScore();

			}
			displayUpdate();

			break;
		}

		case IR_L_TOUCH_MINUS: {
			if (setupMachine == true){
				if (leftPoints>0){
					leftPoints--;
				}
				startMaxPoints = leftPoints;
			}

			else if (timerRunning == false) {
				decreaseLeftScore();

			}
			displayUpdate();

			break;
		}

		case IR_R_TOUCH_PLUS: {
			if (timerRunning == false) {
				increaseRightScore();
				displayUpdate();
			}
			break;
		}

		case IR_R_TOUCH_MINUS: {
			if (timerRunning == false) {
				decreaseRightScore();
				displayUpdate();
			}
			break;
		}

		default:
			break;

		}
		irrecv.resume(); // Receive the next value

	}

	//P.displayZoneText(2, "\x080 2", RIGHT, SPEED_TIME, PAUSE_TIME, PRINT,	NO_EFFECT);
	//P.displayZoneText(1, "30\x08a", LEFT, SPEED_TIME, PAUSE_TIME, PRINT,NO_EFFECT);
	//P.displayZoneText(0, "15", RIGHT, SPEED_TIME, PAUSE_TIME, PRINT, NO_EFFECT);
	//P.displayZoneText(3, "15", LEFT, SPEED_TIME, PAUSE_TIME, PRINT, NO_EFFECT);
	//blinkZone(0);
	//blinkZone(3);
	//blinkClock();
	//while (!P.getZoneStatus(2))
	//P.displayAnimate();
	displayUpdate();
	timer.check();
}

void blinkZone(int z) {
	for (int var = 0; var < 15; ++var) {
		P.setIntensity(z, 0);
		delay(100);
		P.setIntensity(z, 10);
		delay(100);
	}
}

void blinkTwoZones(int z, int y) {
	for (int var = 0; var < 15; ++var) {
		P.setIntensity(z, 0);
		P.setIntensity(y, 0);
		delay(100);
		P.setIntensity(z, 10);
		P.setIntensity(y, 10);
		delay(100);
	}
}

void blinkClock() {
	for (int var = 0; var < 10; ++var) {
		P.setIntensity(1, 0);
		P.setIntensity(2, 0);

		delay(100);
		P.setIntensity(1, 10);
		P.setIntensity(2, 10);
		delay(100);
	}
}

void displayUpdate() {

	minsTens = (String) mins10;
	minsUnits = (String) mins1;
	secsTens = (String) secs10;
	secsUnits = (String) secs1;
	//secsUnits.toCharArray(s1, 3);
	//secsTens.toCharArray(s10, 6);
	strsec = secsTens + secsUnits;
	strsec.toCharArray(secs, 4);
	strmin = maliBroj[period];
	scoreLeft = (String) leftPoints;
	scoreRight = (String) rightPoints;
	scoreLeft.toCharArray(scoreL,4);
	scoreRight.toCharArray(scoreR,4);


	if (mins10 > 0) {
		strmin = strmin + minsTens + minsUnits;
	} else {
		strmin = strmin + " " + minsUnits;
	}

	strmin.toCharArray(mins, 4);

	strsec = secsTens + secsUnits + clockSuffix;
	//strsec= secsTens+secsUnits+ "\x080";

	strsec.toCharArray(secs, 4);

	//P.displayClear(3);
	P.displayZoneText(1, secs, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
	while (!P.getZoneStatus(1))
		P.displayAnimate();
	P.displayZoneText(2, mins, PA_RIGHT, 0, 0, PA_PRINT, PA_NO_EFFECT);
	while (!P.getZoneStatus(2))
		P.displayAnimate();
   P.displayZoneText(3, scoreL, PA_RIGHT, 0, 0, PA_PRINT, PA_NO_EFFECT);
 while (!P.getZoneStatus(3))
    P.displayAnimate();
    P.displayZoneText(0, scoreR, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
  while (!P.getZoneStatus(0))
    P.displayAnimate();
}

void clockUpdate() {
	if (justRearmed == true) {
		seconds = 1;
		justRearmed = false;
	}
	seconds--;
	if (seconds < 0) {
		seconds = 59;
		minutes--;
	}
	secs10 = seconds / 10;
	secs1 = seconds % 10;
	mins10 = minutes / 10;
	mins1 = minutes % 10;
	displayUpdate();

	if (minutes == 0 && seconds == 0) {
		blinkClock();
		timerRunning = false;
		timeOut = true;
		timer.disable();
		delay(3000);
		//minutes = 3;
		//timer.reset();
		//timer.enable();
		digitalWrite(2, HIGH);
	}
}

void mdp() {
	P.begin(MAX_ZONES);
}

void rearmMachine() {
	minutes = startMinutes;
	seconds = startSeconds;
	leftPoints = startPoints;
	rightPoints = startPoints;
	clockReset();
	pointsReset();
	justRearmed = true;

}
void clockReset() {
	secs10 = seconds / 10;
	secs1 = seconds % 10;
	mins10 = minutes / 10;
	mins1 = minutes % 10;

	minsTens = (String) mins10;
	minsUnits = (String) mins1;
	secsTens = (String) secs10;
	secsUnits = (String) secs1;
	//secsUnits.toCharArray(s1, 3);
	//secsTens.toCharArray(s10, 6);
	strsec = secsTens + secsUnits;
	strsec.toCharArray(secs, 4);
	strmin = maliBroj[period];
	if (mins10 > 0) {
		strmin = strmin + minsTens + minsUnits;
	} else {
		strmin = strmin + " " + minsUnits;
	}

	//strmin=minsTens+minsUnits;
	strmin.toCharArray(mins, 4);

	//P.displayClear(2);
//strcat(temp,s10);
//strcat(s10,s1);
	P.displayZoneText(1, secs, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
	while (!P.getZoneStatus(1))
		P.displayAnimate();
	P.displayZoneText(2, mins, PA_RIGHT, 0, 0, PA_PRINT, PA_NO_EFFECT);
	while (!P.getZoneStatus(2))
		P.displayAnimate();
}

void increaseLeftScore() {
	if (leftPoints < startMaxPoints) {
		leftPoints++;
		if (leftPoints==startMaxPoints){
		maxScoreReached =true;
	}
}
}

void decreaseLeftScore() {
	if (leftPoints > 0) {
		leftPoints--;

		}
	}


void increaseRightScore() {
	if (rightPoints < startMaxPoints) {
		rightPoints++;
		if (rightPoints==startMaxPoints){
			maxScoreReached =true;
		}
	}
}

void decreaseRightScore() {
	if (rightPoints > 0) {
		rightPoints--;
	}
}

void increaseDouble() {
	if (leftPoints == rightPoints && rightPoints+1 == startMaxPoints){

	//
	}
	else {
		leftPoints++;
		rightPoints++;
		if (leftPoints == startMaxPoints || rightPoints == startMaxPoints) {
			maxScoreReached = true;
		}
	}

}

void pointsReset(){
	scoreLeft = (String) leftPoints;
	scoreRight = (String) rightPoints;
	scoreLeft.toCharArray(scoreL,4);
	scoreRight.toCharArray(scoreR,4);
	maxScoreReached =false;


	P.displayZoneText(3, scoreL, PA_RIGHT, 0, 0, PA_PRINT, PA_NO_EFFECT);
while (!P.getZoneStatus(3))
	 P.displayAnimate();
	 P.displayZoneText(0, scoreR, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
 while (!P.getZoneStatus(0))
	 P.displayAnimate();
}
