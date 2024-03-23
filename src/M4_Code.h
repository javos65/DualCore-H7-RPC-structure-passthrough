/*
* RPC example to pass complete data structures over RCP link from M7 to M4
*  
*
* Build by : Jay Fox
* Date : 2024-03-2024
* Platform: Portenta H7
*
* License: open source MIT
*
*/

#ifdef CORE_CM4

#include "RPC.h"
#include "Structures.h"
#include <Arduino.h>

M7Data InData;
M4Data OutData = {false,0,1,true,false,1,2,3,4,5,6,false,false,0x66,0x3A,"OK"};
uint8_t *Inpointer = (uint8_t *) &InData ;  // create pointer to data 
uint8_t *Outpointer = (uint8_t *) &OutData ;  // create pointer to data structure
uint8_t  RPCmsg[64];


// Error routine
void fatal_error() {
    while (true) {
        digitalWrite(LEDR, LOW);
        delay(500);
        digitalWrite(LEDR, HIGH);
        delay(500);
    }
}

// Call back function when RCP receives data
void recv_callback(const uint8_t *buf, size_t len) 
{
  if(len == sizeof(M7Data)) {                                                         // check expected length of data
    for(int t = 0; t< len ;++t) *( (uint8_t*) Inpointer+t) =  *( (uint8_t*) buf+t);   // dont use the RPC.read functions, doest not work properly
    RPC.flush();      
    OutData.error = false;     
    OutData.pcounter = InData.pcounter +1;                                                           
  }
  else{ 
    OutData.error = true;
  }
}


void setup() {

    if (!RPC.begin()) {fatal_error(); }
    RPC.attach(recv_callback);

    pinMode(LEDG, OUTPUT);digitalWrite(LEDG, LOW);
    InData.pcounter=0;
}

void loop(){
uint32_t time=millis();
while(1) {
  if(millis()-time > 2000) { // every 2 seconds send packge 
    OutData.timestamp = millis();
    OutData.posx=random(1,320);
    OutData.posy=random(1,480);
    OutData.key=random(1,128);
    sprintf(OutData.message,"Hello #ID%x",InData.identity);
    RPC.write(Outpointer, sizeof(M4Data), true); // send raw data
    time=millis();
    }
  } // while 1 
}



#endif // CORE_CM4