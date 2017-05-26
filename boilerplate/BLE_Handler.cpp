#include "BLE_Handler.h"

BLE_Handler::BLE_Handler()//  Default constructor
{
    Connected = false;
    String MAC = String(*(uint8_t *)0x100000a4, HEX);
    String suffix = SUFFIX;
    AdvertiseName = suffix + MAC;    
}

// Code that executes everytime token is being connected to
void RFduinoBLE_onConnect()
{
    extern BLE_Handler BLE;
    BLE.Connected = true;
}

// Code that executes everytime token is being disconnected from
void RFduinoBLE_onDisconnect()
{
    extern BLE_Handler BLE;
    BLE.Connected = false;
}

//Add a token to the stack
void BLE_Handler::Emit(Token *Event)
{
    if(Event != NULL)
    {
      Serial.println("Added Event !");
      ReceivedStack.push(Event);  
    }
}

//Send a token via BT
void BLE_Handler::SendEvent(Token* Event)
{
    String payload = Event->getEventString();
    char sendData[20] = {0};
    payload.toCharArray(sendData, payload.length()+1);    
    RFduinoBLE.send(sendData, payload.length());
    Serial.print("Token sent: "); Serial.println(payload);
}

// BT Token Received Callback
void RFduinoBLE_onReceive(char *data, int length)
{
    extern BLE_Handler BLE;
    BLE.ReceiveEvent(data, length);
}

// Add a token to the stack
void BLE_Handler::ReceiveEvent(char *Data, int Lenght)
{ 
    Token *NewEvent = new Token(Data, Lenght);
    ReceivedStack.push(NewEvent);
}

// Token handling loop
void BLE_Handler::ProcessEvents()
{
   // extern TokenFeedback_Handler TokenFeedback;
    
    if(ReceivedStack.count() == 0)
      return;
      
    Token *Event = NULL;
    Event = ReceivedStack.pop();

    if(Event == NULL)
      return;
    else
        {
            String temp = Event->getEventString();
            Serial.print("Token received: ");
            Serial.println(temp);
        }
    
    if(Event->FirstValue == String("led"))
    {
        if(Event->SecondValue == String("on"))
        {
          //  TokenFeedback.setColor(Event->ThirdValue);
        }
        else if(Event->SecondValue == String("off"))
        {
          //  TokenFeedback.setColor(Event->SecondValue);
        }
    }
    if(Event->FirstValue == String("haptic"))
    {
      //  TokenFeedback.Vibrate(Event->SecondValue);
    }
}

/*******************************************************************************************************/

Token::Token()
{
}

Token::Token(char* Data, uint8_t Length)
{
    String Command;
    Command = Data;
    Command = Command.substring(0, Length);
    
    int CommaIndex = Command.indexOf(',');
    if(CommaIndex > 0)
    {
    int SecondCommaIndex = Command.indexOf(',', CommaIndex + 1);
    FirstValue = Command.substring(0, CommaIndex);
    SecondValue = Command.substring(CommaIndex + 1, SecondCommaIndex);
    ThirdValue = "";
    if(SecondCommaIndex > 0)
    {
      ThirdValue = Command.substring(SecondCommaIndex + 1);
    }
    }
    else
    {
        FirstValue = Command.substring(0, CommaIndex);
        SecondValue = "";
        ThirdValue = "";        
    }    
}


String Token::getEventString()
{
    char sendData[20] = {0};
    String Payload;
    extern BLE_Handler BLE;
    
    //Payload = BLE.AdvertiseName + "," + FirstValue;
    Payload = BLE.AdvertiseName + "," + FirstValue;
    
    if(SecondValue.length() != 0)
    {
      Payload = Payload + "," + SecondValue;

      if(ThirdValue.length() != 0)
        Payload = Payload + "," + ThirdValue;
    }

    return Payload;
}

void Token::set(char* Data, uint8_t Length)
{
    String Command;
    Command = Data;
    Command = Command.substring(0, Length);
  
    int CommaIndex = Command.indexOf(',');
    //  Search for the next comma just after the first
    int SecondCommaIndex = Command.indexOf(',', CommaIndex + 1);
  
    FirstValue = Command.substring(0, CommaIndex);
    SecondValue = Command.substring(CommaIndex + 1, SecondCommaIndex);
    ThirdValue = Command.substring(SecondCommaIndex + 1);
}

void Token::set(String pFirstValue, String pSecondValue, String pThirdValue)
{
    FirstValue = pFirstValue;
    SecondValue = pSecondValue;
    ThirdValue = pThirdValue;
}

