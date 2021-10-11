#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define encoderDT 0
#define encoderCLK 1 // Interrupt
#define encoderSW 2
int previousDT;
int previousCLK;
int previousSW;


int actualSW;

bool clicked = false;
int coordX = 0;
int scale = 2;
int dir = 0;
int encoderPos = 0;
int previousEncoderPos = 0;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  Serial.begin(9600);
  ////////rotaryEncoder//////////
  pinMode(encoderDT, INPUT_PULLUP);
  pinMode(encoderCLK, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderCLK), doEncoder, CHANGE);
  previousDT = digitalRead(encoderDT);
  previousCLK = digitalRead(encoderCLK);
  previousSW = digitalRead(encoderSW);
  ////////rotaryEncoder//////////

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
}

void loop() {
    
  actualSW = digitalRead(encoderSW); // Without debouncing
  if (actualSW == LOW && previousSW == HIGH) //check for click
   {
     clicked = !clicked;
     if (clicked)
     {
        coordX += 6;
     }
   }
  previousSW = digitalRead(encoderSW);

  dir = rotaryDir();
  if(dir != 0)
    {
      nextCharacter();
      Serial.println(dir);
      Serial.println(encoderPos);
    }
}

void nextCharacter() {
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // Draw white text
  display.setCursor(coordX, 0);
  display.write(encoderPos);
  display.display();
}

//Rotary Encoder Logic
int rotaryDir()
{
  dir = previousEncoderPos - encoderPos;
  previousEncoderPos = encoderPos;
  delay(40);
  return dir;
}

void doEncoder() //rotary encoder interupt
{
int actualCLK = digitalRead(encoderCLK);
int actualEncoderDT = digitalRead(encoderDT);

if ((actualCLK == 1) and (previousCLK == 0))
{
if (actualEncoderDT == 1){
encoderPos--;}
else{
encoderPos++;}
}

if ((actualCLK == 0) and (previousCLK == 1))
{
if (actualEncoderDT == 1){
encoderPos++;}
else{
encoderPos--;}
}

previousCLK = actualCLK;
}
