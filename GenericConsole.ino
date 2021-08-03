//
/* generic console for arduino leonardo to test general config
*/  
#include <Joystick.h>
#include <LiquidCrystal_I2C.h>
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
4, 2,
true, true, false, 
false, false, true, 
false, true,
false, false, false);

LiquidCrystal_I2C lcd(0x3F, 20, 4);

int Sw1Pin = A0;
int Sw1Max = 1023;
int Sw1Min = 0;
int Sw1Segments = 7;
int Sw1Increment = Sw1Max / Sw1Segments;
int Sw1MidPoint = Sw1Increment / 2;
int Sw1Position = 0;
int Sw2Pin = A1;
int Sw2Max = 1023;
int Sw2Min = 0;
int Sw2Segments = 7;
int Sw2Increment = Sw2Max / Sw2Segments;
int Sw2MidPoint = Sw2Increment / 2;
int Sw2Position = 0;
int POVCount = 2;
int POVPin [] = {A0, A1};
int POVMax = 1023;
int POVMin = 0;
int POVSegments [] = {7, 7};

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

int SWCount = 4;
int SWPin [] = {7, 6, 5, 4};
int SWButton [] = {0, 1, 2, 3};
int SWLastState [] = {0, 0, 0, 0};
int iSWIndex = 0;

char line0 [21];
char line1 [21];
char line2 [21];
char line3 [21];

void setup() {
  // put your setup code here, to run once:

  Joystick.begin();
  Joystick.setXAxisRange(0,1023);
  Joystick.setYAxisRange(0,1023);
  Joystick.setRzAxisRange(0,1023);
  Joystick.setThrottleRange(0,1023);

  for (iSWIndex = 0; iSWIndex<SWCount; iSWIndex++) {
    pinMode(SWPin[iSWIndex], INPUT_PULLUP);
    Joystick.releaseButton(SWButton[iSWIndex]);
  }
  
  lcd.init();
  lcd.clear();
  lcd.setBacklight(255);
  
}                                 
  String strPosSwitchStatement = "V1 Generic 8/1/2021";
                                 
void loop() {
    lcd.home();

    int Sw1Value = analogRead(Sw1Pin);
    int Sw2Value = analogRead(Sw2Pin);

    int Sw1Position = (Sw1Value * Sw1Segments + Sw1MidPoint) / Sw1Max;
    int Sw2Position = (Sw2Value * Sw2Segments + Sw2MidPoint) / Sw2Max;

    int TurnRate = map(analogRead(TurnRatePin), 0, 1023, TurnRateMin, TurnRateMax);
    int CameraAngle = map(analogRead(IntakeCameraPin), 0, 1023, IntakeCameraMin, IntakeCameraMax);
    int ShootSpeed = map(analogRead(ShootSpeedPin), 0, 1023, ShootSpeedMin, ShootSpeedMax);
    int ShootOff = map(analogRead(ShootOffPin), 0, 1023, ShootOffMin, ShootOffMax);

    int iPOV1 = Sw1Position * 45;
    int iPOV2 = Sw2Position * 45;

    for (iSWIndex=0; iSWIndex<SWCount; iSWIndex++) {
      int SWCurrState = !digitalRead(SWPin[iSWIndex]);
      //if (SWCurrState != SWLastState[iSWIndex]) {
        Joystick.setButton(SWButton[iSWIndex], SWCurrState);
        SWLastState[iSWIndex] = SWCurrState;
      //}
    }

   lcd.setCursor(0,0);
   sprintf(line0,"POV1: %3u POV2: %3u", iPOV1, iPOV2);
   lcd.print(line0);
   lcd.setCursor(0, 1);
   sprintf(line1,"XA: %4u  YA: %4u", TurnRate, CameraAngle);
   lcd.print(line1);
   lcd.setCursor(0,2);
   sprintf(line2,"ZR: %4u  SL: %4u", ShootSpeed, ShootOff);
   lcd.print(line2);
   lcd.setCursor(0,3);
   
   Joystick.setHatSwitch(0, iPOV1);
   Joystick.setHatSwitch(1, iPOV2);
   Joystick.setXAxis(TurnRate);
   Joystick.setYAxis(CameraAngle);
   Joystick.setRzAxis(ShootSpeed);
   Joystick.setThrottle(ShootOff);
  
}