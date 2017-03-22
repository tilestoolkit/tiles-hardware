/*
  Tiles Squares modules working firmware. Support Square modules prototype 2 Rev B,C.
  Apache 2.0 license.
  Authors: Simone Mora, Francesco Gianni
*/

#include <Arduino.h>
#include <RFduinoBLE.h>
#include <FastLED.h>
#include <stdlib.h>

FASTLED_USING_NAMESPACE


#define NUM_LEDS 16
#define DATA_PIN 6
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          200
#define FRAMES_PER_SECOND  120

String adv_name;
String mac;
uint8_t *deviceADDR0 = (uint8_t *)0x100000a4; // location of MAC address last byte
char adv_name_c[8];
String event_name;
String payload;
char c_payload[19];

bool isRainbow = false;

void setup() {
  //Define adv name
  mac = String(*deviceADDR0, HEX);
  adv_name = "Tile_" + mac;
  adv_name.toCharArray(adv_name_c, 8);


  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(BRIGHTNESS);
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  //Setup Bluetooth Connectivity
  //set the device name
  RFduinoBLE.deviceName = adv_name_c;
  //set the data we want to appear in the advertisement (max 31bytes)
  RFduinoBLE.advertisementData = adv_name_c;
  //set advertising interval in ms (low-longer battery life)
  RFduinoBLE.advertisementInterval = 200;
  //set tx signal strenght (value between -30 and +4dDm in 4dBm increments)
  RFduinoBLE.txPowerLevel = -20;
  //start the BLE stack
  RFduinoBLE.begin();

 fill_solid(leds, 2, CRGB::Red); FastLED.show();
 for(int i=0;i<16;i++)
 {
 leds[i] = CRGB::Blue;
 }
 FastLED.show();
 delay(500);
 for(int i=0;i<16;i++)
 {
 leds[i] = CRGB::Black;
 }
 FastLED.show();

}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void loop() {
  if(isRainbow)
  {
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();
    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);
    // do some periodic updates
    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  }
}

void RFduinoBLE_onConnect()
{
}

void RFduinoBLE_onDisconnect()
{
}

void RFduinoBLE_onAdvertisement() {
}

//Callback when a data chunk is received. OBS! Data chunks must be 20KB (=20 ASCII characters) maximum!
void RFduinoBLE_onReceive(char *data, int len)
{
  //TODO verify whether the payload variable has to be deallocated to avoid memory leaks
  // char payload[6];
  // strncpy(payload,data,len);
  //debugging data packet received
  //Serial.println("Data received");
  //Serial.print("Packet lenght: "); Serial.println(len);
  //Serial.print("Payload: "); Serial.println(data);
  parseCommand(data,len);
}



void generateEvent(String payload)
{
  payload.toCharArray(c_payload, 19);
  RFduinoBLE.send((char*) c_payload, 19);
}

void parseCommand(char *data,int len){
  String command;
  command = data;
  command = command.substring(0, len);

  int commaIndex = command.indexOf(',');
  //  Search for the next comma just after the first
  int secondCommaIndex = command.indexOf(',', commaIndex + 1);

  String firstValue = command.substring(0, commaIndex);
  String secondValue = command.substring(commaIndex + 1, secondCommaIndex);
  String thirdValue = command.substring(secondCommaIndex + 1);
  //  Serial.print("Command: "); Serial.println(command);
  //  Serial.print("FirstValue: "); Serial.println(firstValue);
  //  Serial.print("SecondValue: "); Serial.println(secondValue);
  //  Serial.print("ThirdValue: "); Serial.println(thirdValue);

  if (firstValue == "led") {
    if (secondValue == "off") {
      setColor("off");
    }
    else if (secondValue == "on")
    {
      setColor(thirdValue);
    }
    else if (secondValue == "blink")
    {
    }
    else if (secondValue == "fade") {
    }
  }
}

// LED Functions
void setColor(String color)
{
  if (color == "off")
  {
    isRainbow = false;
    for(int i=0;i<16;i++)
    {
    leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
  else if (color == "red"){
    for(int i=0;i<16;i++)
    {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(80);
    }
    //FastLED.show();
  }
  else if (color == "green"){
  for(int i=0;i<16;i++)
  {
  leds[i] = CRGB::Green;
  FastLED.show();
  delay(80);
}}
  else if (color == "blue"){
  for(int i=0;i<16;i++)
  {
  leds[i] = CRGB::Blue;
  FastLED.show();
  delay(80);
}}
  else if (color == "white"){
  for(int i=0;i<16;i++)
  {
  leds[i] = CRGB::White;
  FastLED.show();
  delay(80);
}}
  else if (color == "rainbow"){isRainbow=true;}
  else if (color.length() == 6) {
    unsigned int colorHex = strtol(&color[0], NULL, 16);
    for(int i=0;i<16;i++)
    {
    leds[i] = colorHex;
    FastLED.show();
    delay(80);
    }
    int red, green, blue;
    parseColorString(color, red, green, blue);
    setColorRGB(red, green, blue);
  }
}

void parseColorString(String color, int& red, int& green, int& blue) {
  unsigned int colorHex = strtol(&color[0], NULL, 16);
  red = (int)((colorHex >> 16) & 0xFF);  // Extract the RR byte
  green = (int)((colorHex >> 8) & 0xFF); // Extract the GG byte
  blue = (int)((colorHex) & 0xFF);       // Extract the BB byte
}

void setColorRGB(int red, int green, int blue)
{
//  analogWrite(RED_LED_PIN, red);
//  analogWrite(GREEN_LED_PIN, green);
//  analogWrite(BLUE_LED_PIN, blue);
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
