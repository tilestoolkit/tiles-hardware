#include <Arduino.h>

/*
  TILES Printer
  Simone Mora (simonem@ntnu.no) 19/10/16
  Francesco Gianni (francg@idi.ntnu.no) 20/10/16

  Protocol: printer,content
  Content has a 12 ASCII characters limit

  Wiring, use hardware serial UART and Rfduino LED/BUTTON shield
  Rfduino PIN 1 <-> Printer YELLOW WIRE
  Rfduino PIN 0 <-> Printer GREEN WIRE
  Rfduino GND   <-> Printer BLACK WIRE
*/

#include <RFduinoBLE.h>

//Required for Thermal Printer
#include <Adafruit_Thermal.h>
Adafruit_Thermal printer(&Serial);

//Defines name of the Tile
String adv_name;
String mac;
uint8_t *deviceADDR0 = (uint8_t *)0x100000a4; // location of MAC address last byte
char adv_name_c[8];

#define RED_LED_PIN 2
#define GREEN_LED_PIN 3
#define BLUE_LED_PIN 4

void setup() {
  override_uart_limit = true;
  Serial.begin(19200);    //Serial Interface to the printer
//  Serial.begin(9600);
  printer.begin();        //Initialises printer
  printer.justify('L');   // Justifies text left by default (options: L, C, R)
  printer.setSize('M');   // Sets size of text to medium (options: L, M, S)

 //Define adv name
  mac = String(*deviceADDR0,HEX);
  adv_name = "Tile_" + mac;
  adv_name.toCharArray(adv_name_c,8);

  //Setup IO PINs
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  //Printer test
  printer.justify('C');
  printer.setSize('L');        // Set type size, accepts 'S', 'M', 'L'
  printer.println(F("Tiles Printer Ready"));
  printer.feed(2);

  //blink the LEDS to test they are actually working
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(250);
  digitalWrite(BLUE_LED_PIN, HIGH);
  delay(500);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

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
}


void loop() {


  delay(50); // Important delay, do not delete it !
}

void RFduinoBLE_onConnect()
{
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(500);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void RFduinoBLE_onDisconnect()
{
  digitalWrite(GREEN_LED_PIN, LOW);
}

void RFduinoBLE_onAdvertisement(){
  digitalWrite(RED_LED_PIN, HIGH);
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

  if(firstValue == "printer"){
    Serial.println();
    Serial.println(secondValue);
    Serial.println();
    Serial.println();
    Serial.println();
  }
}


void print(String content){
  char buffer[10];
  content.toCharArray(buffer,10);
  printer.print((char *) buffer);
  printer.feed(2);
  for(int i = 0; i<10; i++){
   buffer[i] = 0;}
}
