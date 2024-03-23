/*
* RPC example to pass complete data structures over RCP link from M7 to M4
*  
*
* Build by : Jay Fox
* Date : 2024-03-2024
* Platform : portenta H7
*
* License: open source MIT
*
*/


#include "M4_Code.h"

#ifdef CORE_CM7
#include "RPC.h"
#include "Structures.h"
#include <Arduino.h>


M7Data MyData = {0, 932223235, 1,true,  false,  100,  100, 0x55, "OK"};
M4Data RcvData;
uint8_t *Mpointer = (uint8_t *) &MyData ;  // create pointer to data 
uint8_t *Rpointer = (uint8_t *) &RcvData ;  // create pointer to data structure
uint8_t  txtmsg[256];
uint8_t BUF1[256];
uint8_t BUF2[256];


void fatal_error() {
    while (true) {
        digitalWrite(LEDR, LOW);
        delay(500);
        digitalWrite(LEDR, HIGH);
        delay(500);
    }
}

// Call back function : recieve M4 message int  structue
void recv_callback(const uint8_t *buf, size_t len) {
  if(len == sizeof(M4Data)) {                                                         // check expected length of data
    for(int t = 0; t< len ;++t) *( (uint8_t*) Rpointer+t) =  *( (uint8_t*) buf+t);    // dont use the RPC.read functions, doest not work properly
    RPC.flush();                                                                      // flush pipe
    printstructureM4();                                                               // put date into a string
    sprintf((char*) txtmsg,"Received packet with len %i : %s\0",len, BUF2);            // send to Serial
    Serial.println((char*) txtmsg);
    MyData.error = false;
    MyData.pcounter=RcvData.pcounter+1;
  }
  else{
    sprintf((char*) txtmsg,"Received wrong packet size %i\0",len); Serial.println((char*) txtmsg);
    MyData.error = true;
  }
}

void setup() {
    Serial.begin(115200); while (!Serial) {}
    if (!RPC.begin()) {fatal_error(); }
    RPC.attach(recv_callback);
    pinMode(LEDB, OUTPUT);digitalWrite(LEDB, LOW);
    delay(1000);
}

void loop() {
uint32_t time=millis();
while(1) {
  if(millis()-time > 1000) { // every 2 seconds send packge 

    //change your data
    MyData.timestamp = millis();
    MyData.kills=random(1,100);
    MyData.points=random(1,300);
    sprintf(MyData.message,"All ok ?");
    // change your data end

    Serial.print(" * Send packet#");Serial.print(MyData.pcounter);Serial.print(", content : ");
    printstructureM7();Serial.println( (char *) BUF1 );
    RPC.write(Mpointer, sizeof(M7Data), true); // send raw data
    time=millis();
    }
  } // while 1 
}



// print Structures to BUFfer
void printstructureM7()
{
sprintf((char*) BUF1,"Strc[%x,%i,%i,0x%x,0x%x,%i,%i,0x%x,%s]",MyData.error,MyData.timestamp,MyData.pcounter,MyData.alive,MyData.init,
                                                             MyData.points,MyData.kills,MyData.identity,MyData.message);
}

void printstructureM4()
{
sprintf((char*) BUF2,"Strc[%x,%i,%i,0x%x,0x%x,%i,%i,,%i,%i,%i,%i,%i,%i,0x%x,0x%x,%s]",RcvData.error,RcvData.timestamp,RcvData.pcounter,RcvData.alive,RcvData.init,
                                                                                       RcvData.posx,RcvData.posy,RcvData.bul1x,RcvData.bul1y,RcvData.bul2x,RcvData.bul2y,
                                                                                       RcvData.shoot1,RcvData.shoot2,RcvData.identity,RcvData.key,RcvData.message);
}

    #endif
