#ifndef BLEHANDLER_h
#define BLEHANDLER_h

#include <RFduinoBLE.h>
#include <StackArray.h>
#include "CONFIG.h"
//#include "TokenFeedback_Handler.h"

class Token;

class BLE_Handler
{
    public:
      BLE_Handler(); //  Default constructor
      void Transmit();
      void SendEvent(Token* Event);    
      void ReceiveEvent(char *Data, int Lenght);
      void ProcessEvents();
      void Emit(Token *Event);

      bool Connected;
      bool EventReceived;   // true if an event has been Received, cleared with a call to ProcessEvents()
      bool EventToSend;     // true if there are events waiting to be send

      String AdvertiseName;
      
    private:            
      StackArray<Token*> ReceivedStack;   
      StackArray<Token*> SendStack;   
};

class Token
{
  public:
    Token();
    Token(char* Data, uint8_t Length);

    String getEventString();
    void set(char* Data, uint8_t Length);
    void set(String pFirstValue, String pSecondValue = String(""), String pThirdValue = String(""));
     
    String FirstValue;
    String SecondValue;
    String ThirdValue;
     
};

#endif
