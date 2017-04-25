#include "BLE_Handler.h"

BLE_Handler::BLE_Handler()//  Default constructor
{
        Connected = false;

        String MAC = String(*(uint8_t *)0x100000a4, HEX);
        AdvertiseName = "Tile_" + MAC;
}

// Code that executes everytime token is being connected to
void SimbleeBLE_onConnect()
{
        extern BLE_Handler BLE;
        BLE.Connected = true;
        extern TokenFeedback_Handler TokenFeedback;
        TokenFeedback.setColor("off");

}

// Code that executes everytime token is being disconnected from
void SimbleeBLE_onDisconnect()
{
        extern BLE_Handler BLE;
        BLE.Connected = false;
}

void BLE_Handler::Emit(TokenEvent *Event)
{
        if(Event != NULL)
        {
                Serial.println("Added Event !");
                ReceivedStack.push(Event);
        }
}


void BLE_Handler::SendEvent(TokenEvent* Event)
{
        String Payload = Event->getEventString();
        char sendData[20] = {0};

        Payload.toCharArray(sendData, Payload.length()+1);

        SimbleeBLE.send(sendData, Payload.length());
}

// Code to run upon receiving data over bluetooth
void SimbleeBLE_onReceive(char *data, int length)
{
        extern BLE_Handler BLE;
        BLE.ReceiveEvent(data, length);
}

void BLE_Handler::ReceiveEvent(char *Data, int Lenght)
{
        TokenEvent *NewEvent = new TokenEvent(Data, Lenght);

        //Adding Event to the stack
        ReceivedStack.push(NewEvent);
}

// Executes command
void BLE_Handler::ProcessEvents()
{
        extern TokenFeedback_Handler TokenFeedback;

        if(ReceivedStack.count() == 0)
                return;

        TokenEvent *Event = NULL;
        Event = ReceivedStack.pop();

        if(Event == NULL)
                return;

        if(Event->FirstValue == String("led"))
        {
                if(Event->SecondValue == String("on"))
                {
                        TokenFeedback.setColor(Event->ThirdValue);
                }

                else if(Event->SecondValue == String("off"))
                {
                        TokenFeedback.setColor(Event->SecondValue);
                }
        }

        if(Event->FirstValue == String("haptic"))
        {
                TokenFeedback.Vibrate(Event->SecondValue);
        }

         if(Event->FirstValue == String("new_haptic"))
        {
                TokenFeedback.VibrateCustom(Event->SecondValue);
        }
}




/*******************************************************************************************************/

TokenEvent::TokenEvent()
{
}

TokenEvent::TokenEvent(char* Data, uint8_t Length)
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


String TokenEvent::getEventString()
{
        char sendData[20] = {0};
        String Payload;

        extern BLE_Handler BLE;

        Payload = BLE.AdvertiseName + FirstValue;

        if(SecondValue.length() != 0)
        {
                Payload = Payload + "," + SecondValue;

                if(ThirdValue.length() != 0)
                        Payload = Payload + "," + ThirdValue;
        }

        return Payload;
}

void TokenEvent::set(char* Data, uint8_t Length)
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


void TokenEvent::set(String pFirstValue, String pSecondValue, String pThirdValue)
{
        FirstValue = pFirstValue;
        SecondValue = pSecondValue;
        ThirdValue = pThirdValue;
}
