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

void setup() {
  //Define adv name
  mac = String(*deviceADDR0, HEX);
  adv_name = "Tile_" + mac;
  adv_name.toCharArray(adv_name_c, 8);


  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
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


void loop() {
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
    }
    FastLED.show();
  }
  else if (color == "green")
    setColorRGB(0, 255, 0);
  else if (color == "blue")
    setColorRGB(0, 0, 255);
  else if (color == "white")
    setColorRGB(255, 255, 255);
  else if (color.length() == 6) {
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
