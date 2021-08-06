//
/* generic console for arduino leonardo to test general config
*/  
#include <Joystick.h>
#include <LiquidCrystal_I2C.h>

// max counts for input types depend on the arduino board - leonardo assumed
int POVCount = 2; // max 2
int POTCount = 4; // max 4 in order defined by joystick true options matching sets
int SWCount = 4; // max probably 10 for practicle use - 32 if display changed
/*
* name id 
* constant for type 
* number of buttons (limit 16 on drivestation) 
* number of POV hats (limit 2)
* x axis t/f 
* y axis t/f 
* z axis t/f 
* rudder / paddle / other names of no meaning to me 4 * t/f  
* throttle t/f 
* 3 * t/f 
* see arduino joystick on github for more info 
*/
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
SWCount, POVCount,
true, true, false, // axis - x y z
false, false, true, // rotate - x y z
false, true,      // rudder throttle (slider)
false, false, false);

LiquidCrystal_I2C lcd(0x3F, 20, 4);

int POVPin [] = {A0, A1};
int POVMax = 1023;
int POVMin = 0;
int POVSegments [] = {7, 7};
int POVIncrement [] = {0, 0};
int POVMidpoint [] = {0, 0};
int iPOVIndex = 0;

int POTPin [] = {A2, A3, A4, A5};
int POTMax [] = {1023, 1023, 1023, 1023};
int POTMin [] = {0, 0, 0, 0};
int TurnRatePin = A2;
int TurnRateMax = 1023;
int TurnRateMin = 0;

int IntakeCameraPin = A3;
int IntakeCameraMax = 1023;
int IntakeCameraMin = 0;

int ShootSpeedPin = A4;
int ShootSpeedMax = 1023;
int ShootSpeedMin = 0;
int ShootOffPin = A5;
int ShootOffMax = 1023;
int ShootOffMin = 0;

int SWPin [] = {7, 6, 5, 4};
//int SWButton [] = {0, 1, 2, 3};
byte SWLastState [] = {0, 0, 0, 0};
uint8_t iSWIndex = 0;

char POVLine [11];
char SWLine [2];
char println [4] [21];
int iPrintLnIndx = 0;
int iPrintLnCnt = 4;
char line1 [21];
char line2 [21];

void setup() {
  // put your setup code here, to run once:

  Joystick.begin(false);
  Joystick.setXAxisRange(0,1023);
  Joystick.setYAxisRange(0,1023);
  Joystick.setRzAxisRange(0,1023);
  Joystick.setThrottleRange(0,1023);

  for (iPOVIndex=0; iPOVIndex<POVCount; iPOVIndex++) {
    POVIncrement[iPOVIndex] = POVMax / POVSegments[iPOVIndex];
    POVMidpoint[iPOVIndex] = POVIncrement[iPOVIndex] / 2;
  }

  for (iSWIndex = 0; iSWIndex < SWCount; iSWIndex++) {
    pinMode(SWPin[iSWIndex], INPUT_PULLUP);
    Joystick.releaseButton(iSWIndex);
  }
  
  lcd.init();
  lcd.clear();
  lcd.setBacklight(255);
  
}                                 
  String strPosSwitchStatement = "V1 Generic 8/1/2021";
                                 
void loop() {
  
  lcd.home();

  for (iPrintLnIndx=0; iPrintLnIndx < iPrintLnCnt; iPrintLnIndx++) {
    strcpy(println[iPrintLnIndx], "");
  }

  for (iPOVIndex=0; iPOVIndex < POVCount; iPOVIndex++) {
    int POVValue = analogRead(POVPin[iPOVIndex]);
    int POVPosition = (POVValue * POVSegments[iPOVIndex] + POVMidpoint[iPOVIndex]) / POVMax;
    int POVHeading = POVPosition * 45;
    Joystick.setHatSwitch(iPOVIndex, POVHeading);
    sprintf(POVLine, "POV%1u: %3u ", iPOVIndex, POVHeading);
    strcat(println[0], POVLine);
  }
  
  int TurnRate = map(analogRead(TurnRatePin), 0, 1023, TurnRateMin, TurnRateMax);
  int CameraAngle = map(analogRead(IntakeCameraPin), 0, 1023, IntakeCameraMin, IntakeCameraMax);
  int ShootSpeed = map(analogRead(ShootSpeedPin), 0, 1023, ShootSpeedMin, ShootSpeedMax);
  int ShootOff = map(analogRead(ShootOffPin), 0, 1023, ShootOffMin, ShootOffMax);
  Joystick.setXAxis(TurnRate);
  Joystick.setYAxis(CameraAngle);
  Joystick.setRzAxis(ShootSpeed);
  Joystick.setThrottle(ShootOff);

  for (iSWIndex=0; iSWIndex < SWCount; iSWIndex++) {
    byte SWCurrState = !digitalRead(SWPin[iSWIndex]);
    if (SWCurrState != SWLastState[iSWIndex]) {
      Joystick.setButton(iSWIndex, SWCurrState);
      SWLastState[iSWIndex] = SWCurrState;
    }
    sprintf(SWLine,"%1u%1u", iSWIndex, SWLastState[iSWIndex]);
    strcat(println[3], SWLine);
  }

//  Joystick.setButton(0, HIGH);
//  Joystick.setButton(0, LOW);

  lcd.setCursor(0,0);
  lcd.print(println[0]);
  lcd.setCursor(0, 1);
  sprintf(line1,"XA: %4u  YA: %4u", TurnRate, CameraAngle);
  lcd.print(line1);
  lcd.setCursor(0,2);
  sprintf(line2,"ZR: %4u  SL: %4u", ShootSpeed, ShootOff);
  lcd.print(line2);
  lcd.setCursor(0,3);
  lcd.print(println[3]);
   
  Joystick.sendState();
  delay(50);
}